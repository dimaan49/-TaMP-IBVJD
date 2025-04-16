#include "mytcpserver.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    QCoreApplication::setApplicationName("my-copy-program");
    parser.setApplicationDescription("The application with a server side of client-server");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption portOption({"p", "port"}, "Enter the port of the server", "Server port", "33333");
    parser.addOption(portOption);
    parser.addPositionalArgument("IP","Server IP address, default is 127.0.0.1");

    parser.process(a);
    QStringList pos_args = parser.positionalArguments();
    QHostAddress host_ip;
    if (pos_args.isEmpty()) {
        host_ip = QHostAddress("0.0.0.0");
    } else {
        host_ip = QHostAddress(pos_args.at(0));
    }
    int server_port = parser.value(portOption).toInt();
    MyTcpServer myserv(nullptr, server_port, host_ip);
    return a.exec();
}
