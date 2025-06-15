#include "client.h"
#include <QDebug>
#include <QJsonParseError>
#include <QMessageBox>

//  Инициализация статической переменной
Client* Client::p_instance = nullptr;

//  Реализация приватного конструктора
Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::connected, this, &Client::onSocketConnected);
    connect(socket, &QTcpSocket::errorOccurred, this, &Client::onSocketError);
}

void Client::connectToServer(const QString &ip, quint16 port)
{
    if (socket->state() != QAbstractSocket::UnconnectedState) {
        socket->abort();
    }
    socket->connectToHost(ip, port);
}

void Client::onSocketConnected()
{
    emit connected();
}

void Client::onSocketError(QAbstractSocket::SocketError error)
{
    Q_UNUSED(error);
    emit connectionError(socket->errorString());
}

void Client::sendRequest(const QJsonObject &request)
{
    if (socket->state() != QAbstractSocket::ConnectedState) {
        emit connectionError("Нет подключения к серверу");
        return;
    }

    QJsonDocument doc(request);
    QByteArray data = doc.toJson(QJsonDocument::Compact) + '\n';
    
    if (socket->write(data) == -1) {
        emit connectionError("Ошибка отправки данных");
    }
}

bool Client::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}

void Client::onReadyRead()
{
    while (socket->canReadLine()) {
        QByteArray data = socket->readLine().trimmed();
        
        if (data.isEmpty()) continue;
        
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        
        if (error.error != QJsonParseError::NoError) {
            emit connectionError("Ошибка формата ответа сервера");
            continue;
        }
        
        if (doc.isObject()) {
            emit responseReceived(doc.object());
        }
    }
}