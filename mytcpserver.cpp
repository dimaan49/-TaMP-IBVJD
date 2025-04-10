#include "mytcpserver.h"
#include "serverfunctions.h"

MyTcpServer::~MyTcpServer()
{

    mTcpServer->close();
    server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent,  int server_port,  QHostAddress server_address) : QObject(parent){
    qDebug() << "The server is running on port " << server_port << "\n";
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(server_address, server_port)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    if(server_status==1){
        QTcpSocket *actualSocket;
        actualSocket = mTcpServer->nextPendingConnection();
        actualSocket->write("Hello, World!!! I am echo server!\r\n");
        connect(actualSocket, &QTcpSocket::readyRead,this,&MyTcpServer::slotServerRead);
        connect(actualSocket,&QTcpSocket::disconnected,this,&MyTcpServer::slotClientDisconnected);
        sockArray.insert(actualSocket->socketDescriptor(), actualSocket);
    }
}

void MyTcpServer::slotServerRead(){
    QString res = "";
    QTcpSocket *actualSocket = sockArray[((QTcpSocket *)sender())->socketDescriptor()];
    qDebug() << "New connection with " << ((QTcpSocket *)sender())->socketDescriptor() << "socket descriptor";

        QByteArray array = actualSocket->readAll();
        if (!array.isEmpty()) {
            qDebug() << array << "\n";
            res.append(array);
            QByteArray query = queryAnalyzer(res);
            res+=query;
            actualSocket->write(res.toUtf8());
        }
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket *actualSocket = (QTcpSocket*)sender();
    actualSocket->close();
    sockArray.remove(actualSocket->socketDescriptor());
}

