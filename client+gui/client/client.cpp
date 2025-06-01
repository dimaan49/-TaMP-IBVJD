#include "client.h"
#include <QDebug>
#include <QJsonParseError>
#include <QMessageBox>

Client::Client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(socket, &QTcpSocket::errorOccurred, this, [this](){
        emit connectionError(socket->errorString());
    });
}

void Client::connectToServer(const QString &ip, quint16 port)
{
    socket->connectToHost(ip, port);
    if (!socket->waitForConnected(3000)) {
        emit connectionError("Не удалось подключиться к серверу");
    }
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
    
    if (!socket->waitForBytesWritten(1000)) {
        emit connectionError("Таймаут отправки данных");
    }
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