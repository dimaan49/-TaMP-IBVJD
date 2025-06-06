#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../client/client.h"
#include <QPalette>


/**
 * @class LoginWindow
 * @brief Класс окна авторизации пользователя
 *
 * Обеспечивает интерфейс для ввода учетных данных (логин/пароль),
 * выполняет базовую валидацию и взаимодействует с сервером через Client.
 */
class LoginWindow : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Конструктор LoginWindow
     * @param client Указатель на объект Client для сетевого взаимодействия
     * @param parent Родительский виджет
     */
    explicit LoginWindow(Client *client, QWidget *parent = nullptr);

signals:
    /**
     * @brief Сигнал успешной авторизации
     */
    void loginSuccess();
    
    /**
     * @brief Сигнал запроса открытия окна регистрации
     */
    void openRegister();

private slots:
    /**
     * @brief Слот обработки нажатия кнопки входа
     */
    void onLoginClicked();
    
    /**
     * @brief Слот обработки нажатия кнопки регистрации
     */
    void onRegisterClicked();
   
    /**
     * @brief Слот обработки ответа сервера
     * @param response JSON-объект с ответом сервера
     */
    void handleServerResponse(const QJsonObject &response);

private:
    QLineEdit *loginEdit;    ///< Поле ввода логина
    QLineEdit *passwordEdit; ///< Поле ввода пароля
    Client *client;          ///< Указатель на объект Client
};

#endif // LOGINWINDOW_H
