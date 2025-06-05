#include "server.h"
#include <QTcpSocket>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QRegularExpression>

Server::Server(QObject *parent) : QTcpServer(parent)
{
    if (!initDatabase()) {
        qCritical() << "Database initialization error!";
    }
}

bool Server::startServer(const QHostAddress &address, quint16 port)
{
    if (!listen(address, port)) {
        qCritical() << "Не удалось запустить сервер:" << errorString();
        return false;
    }
    qInfo() << "The server is running on" << address.toString() << "port" << port;
    return true;
}

void Server::stopServer()
{
    close();
    m_db.close();
    qInfo() << "The server has stopped";
}

void Server::printUsersTable()
{
    QSqlQuery query(m_db);
    if (!query.exec("SELECT id, login, password, email, created_at FROM users ORDER BY created_at")) {
        qCritical() << "Ошибка при запросе пользователей:" << query.lastError().text();
        return;
    }

    QVector<QStringList> rows;
    QStringList headers = {"ID", "Login", "Password (hashed)", "Email", "Created At"};
    rows.append(headers);

    while (query.next()) {
        QStringList row;
        row << query.value(0).toString();
        row << query.value(1).toString();
        row << query.value(2).toString();
        row << query.value(3).toString();
        row << (query.value(4).isNull() ? "NULL" : query.value(4).toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        rows.append(row);
    }

    QVector<int> widths;
    for (int i = 0; i < headers.size(); ++i) {
        int max_len = 0;
        for (const auto& row : rows) {
            max_len = qMax(max_len, row[i].length());
        }
        widths << max_len + 2; // +2 для отступов
    }

    QString header_line = "|";
    for (int i = 0; i < headers.size(); ++i) {
        header_line += " " + headers[i].leftJustified(widths[i]-1) + "|";
    }
    qDebug().noquote() << header_line;

    QString separator = "+";
    for (int w : widths) {
        separator += QString("-").repeated(w) + "+";
    }
    qDebug().noquote() << separator;

    for (int i = 1; i < rows.size(); ++i) {
        QString data_line = "|";
        for (int j = 0; j < rows[i].size(); ++j) {
            data_line += " " + rows[i][j].leftJustified(widths[j]-1) + "|";
        }
        qDebug().noquote() << data_line;
    }

    qDebug() << "Total users:" << rows.size()-1;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    QTcpSocket *socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Server::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::onDisconnected);
    
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qWarning() << "Socket error:" << socket->errorString();
        delete socket;
        return;
    }
    
    m_buffers.insert(socket, QByteArray());
    qInfo() << "New connection:" << socket->peerAddress().toString();
}

void Server::onReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    m_buffers[socket].append(socket->readAll());

    while (m_buffers[socket].contains('\n')) {
        QByteArray data = m_buffers[socket].left(m_buffers[socket].indexOf('\n')).trimmed();
        m_buffers[socket].remove(0, m_buffers[socket].indexOf('\n') + 1);
        
        if (!data.isEmpty()) {
            handleRequest(socket, data);
        }
    }
}

void Server::onDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        m_buffers.remove(socket);
        socket->deleteLater();
        qInfo() << "The client has disconnected";
    }
}

bool Server::initDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QDir::currentPath() + "/task_manager.db";
    qDebug() << "Path to DB:" << dbPath;
    m_db.setDatabaseName(dbPath);
    
    if (!m_db.open()) {
        qCritical() << "Error opening DB:" << m_db.lastError().text();
        return false;
    }
    
    QSqlQuery query;
    QString createUsersTable = 
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "login TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL, "
        "email TEXT UNIQUE NOT NULL, "
        "created_at DATETIME DEFAULT CURRENT_TIMESTAMP)";
    
    if (!query.exec(createUsersTable)) {
        qCritical() << "Error creating table:" << query.lastError().text();
        return false;
    }
    
    return true;
}

QString Server::hashPassword(const QString &password)
{
    return QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
}

QByteArray Server::errorResponse(const QString &message)
{
    QJsonObject response;
    response["type"] = "error";
    response["message"] = message;
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

void Server::handleRequest(QTcpSocket *clientSocket, const QByteArray &jsonData)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &error);
    
    if (error.error != QJsonParseError::NoError) {
        clientSocket->write(errorResponse("Invalid JSON format"));
        return;
    }
    
    QJsonObject request = doc.object();
    QString type = request["type"].toString();
    
    QByteArray response;
    if (type == "register") {
        response = processRegister(request);
    } else if (type == "auth") {
        response = processAuth(request);
    } else {
        // Убрано сообщение о неизвестном типе запроса - просто ничего не отвечаем
        return;
    }
    
    clientSocket->write(response + '\n');
}

QByteArray Server::processRegister(const QJsonObject &request)
{
    // Проверка полей
    QStringList requiredFields = {"login", "password", "email"};
    for (const QString &field : requiredFields) {
        if (!request.contains(field) || request[field].toString().isEmpty()) {
            return errorResponse("Не заполнено поле: " + field);
        }
    }

    QString login = request["login"].toString().trimmed();
    QString password = request["password"].toString();
    QString email = request["email"].toString().trimmed().toLower();

    // Валидация
    if (login.length() < 4) {
        return errorResponse("Логин должен быть не менее 4 символов");
    }

    if (password.length() < 6) {
        return errorResponse("Пароль должен быть не менее 6 символов");
    }

    if (!QRegularExpression("^\\S+@\\S+\\.\\S+$").match(email).hasMatch()) {
        return errorResponse("Некорректный email");
    }

    // Проверка существующего пользователя
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare("SELECT id FROM users WHERE login = :login OR email = :email");
    checkQuery.bindValue(":login", login);
    checkQuery.bindValue(":email", email);
    
    if (!checkQuery.exec()) {
        return errorResponse("Ошибка базы данных: " + checkQuery.lastError().text());
    }
    
    if (checkQuery.next()) {
        return errorResponse("Пользователь с таким логином или email уже существует");
    }

    // Хеширование пароля
    QString hashedPassword = hashPassword(password);

    // Добавление пользователя
    QSqlQuery insertQuery(m_db);
    insertQuery.prepare(
        "INSERT INTO users (login, password, email) "
        "VALUES (:login, :password, :email)"
    );
    insertQuery.bindValue(":login", login);
    insertQuery.bindValue(":password", hashedPassword);
    insertQuery.bindValue(":email", email);
    
    QJsonObject response;
    response["type"] = "register_response";
    
    if (insertQuery.exec()) {
        response["success"] = true;
        response["message"] = "Регистрация успешна";
    } else {
        response["success"] = false;
        response["message"] = "Ошибка регистрации: " + insertQuery.lastError().text();
    }
    
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}

QByteArray Server::processAuth(const QJsonObject &request)
{
    if (!request.contains("login") || !request.contains("password")) {
        return errorResponse("Не указан логин или пароль");
    }

    QString login = request["login"].toString();
    QString hashedPassword = hashPassword(request["password"].toString());

    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM users WHERE login = :login AND password = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", hashedPassword);
    
    QJsonObject response;
    response["type"] = "auth_response";
    
    if (!query.exec()) {
        response["success"] = false;
        response["message"] = "Ошибка базы данных: " + query.lastError().text();
    } else if (query.next()) {
        response["success"] = true;
        response["message"] = "Авторизация успешна";
    } else {
        response["success"] = false;
        response["message"] = "Неверный логин или пароль";
    }
    
    return QJsonDocument(response).toJson(QJsonDocument::Compact);
}
