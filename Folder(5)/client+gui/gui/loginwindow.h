#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../client/client.h"
#include <QPalette>

class LoginWindow : public QWidget {
    Q_OBJECT
public:
    explicit LoginWindow(Client *client, QWidget *parent = nullptr);

signals:
    void loginSuccess();
    void openRegister();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void handleServerResponse(const QJsonObject &response);  // Объявлен один раз

private:
    QLineEdit *loginEdit;
    QLineEdit *passwordEdit;
    Client *client;
};

#endif // LOGINWINDOW_H