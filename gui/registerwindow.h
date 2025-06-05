#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "../client/client.h"

/**
 * @class RegisterWindow
 * @brief Класс окна регистрации пользователя
 *
 * Окно позволяет зарегистрировать нового пользователя в системе.
 * Содержит поля для ввода логина, пароля и email, выполняет валидацию
 * введенных данных и отправляет запрос на сервер через класс Client.
 */
class RegisterWindow : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса RegisterWindow
     * @param client Указатель на объект Client для связи с сервером
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit RegisterWindow(Client *client, QWidget *parent = nullptr);

signals:
    /**
     * @brief Сигнал успешной регистрации
     * 
     * Испускается при успешном завершении регистрации
     */
    void registrationSuccess();

    /**
     * @brief Сигнал возврата к окну входа
     * 
     * Испускается при нажатии кнопки "Назад"
     */
    void backToLogin();

private slots:
    /**
     * @brief Обработчик нажатия кнопки регистрации
     * 
     * Проверяет валидность введенных данных и отправляет запрос на сервер
     */
    void onRegisterClicked();

    /**
     * @brief Обработчик ответа от сервера
     * @param response JSON-объект с ответом сервера
     */
    void handleServerResponse(const QJsonObject &response);

private:
    QLineEdit *loginEdit;      ///< Поле ввода логина
    QLineEdit *passwordEdit;   ///< Поле ввода пароля
    QLineEdit *emailEdit;      ///< Поле ввода email
    Client *client;            ///< Указатель на объект Client
};

#endif // REGISTERWINDOW_H
