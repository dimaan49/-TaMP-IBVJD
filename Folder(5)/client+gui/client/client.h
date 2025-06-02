#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    void connectToServer(const QString &ip, quint16 port);
    void sendRequest(const QJsonObject &request);

signals:
    void responseReceived(const QJsonObject &response);
    void connectionError(const QString &error);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H