#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include "loginwindow.h"
#include "registerwindow.h"
#include "taskwindow.h"
#include "questionwindow.h"
#include "client.h"

class MainWindow : public QStackedWidget {
    Q_OBJECT
public:
    explicit MainWindow(Client *client, QWidget *parent = nullptr);

private slots:
    void onLoginSuccess();
    void onOpenRegister();
    void onBackToLogin();
    void onShowQuestion(int id);
    void onShowStats();
    void onExit();

private:
    Client *client;
    LoginWindow *loginWindow;
    RegisterWindow *registerWindow;
    TaskWindow *taskWindow;
    QuestionWindow *questionWindow;
};

#endif // MAINWINDOW_H