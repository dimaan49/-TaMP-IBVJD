#include <QApplication>
#include "gui/mainwindow.h"
#include "../client/client.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Client client;
    client.connectToServer("127.0.0.1", 33333);

    MainWindow w(&client);
    w.resize(400, 300);
    w.show();

    return a.exec();
}