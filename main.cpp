#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "mytcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    QCoreApplication::setApplicationName("my-copy-program");
    parser.setApplicationDescription("Тестовое приложение с парсером командной строки");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption portOption("port", "Enter the port of the server", "port", "33333");
    parser.addOption(portOption);
    parser.process(a);
    int server_port = parser.value(portOption).toInt();
    MyTcpServer myserv(nullptr, server_port);
    return a.exec();
}
