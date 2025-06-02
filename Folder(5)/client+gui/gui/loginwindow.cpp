#include "loginwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

LoginWindow::LoginWindow(Client *client, QWidget *parent) 
    : QWidget(parent), client(client) 
{
    // Установка фиолетового фона только для основного окна
    this->setStyleSheet("QWidget#LoginWindow { background-color: #aa00ff; }");
    
    // Настройка layout и элементов интерфейса
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(15);
    layout->setContentsMargins(30, 30, 30, 30);

    // Поле для логина (системный стиль)
    loginEdit = new QLineEdit(this);
    loginEdit->setPlaceholderText("Введите логин");
    loginEdit->setMinimumHeight(40);
    loginEdit->setObjectName("loginEdit"); // Для возможного кастомного стиля
    layout->addWidget(loginEdit);

    // Поле для пароля (системный стиль)
    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Введите пароль");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(40);
    passwordEdit->setObjectName("passwordEdit"); // Для возможного кастомного стиля
    layout->addWidget(passwordEdit);

    // Кнопка входа (системный стиль)
    QPushButton *loginButton = new QPushButton("Войти", this);
    loginButton->setMinimumSize(100, 45); // Только размер, без стилей
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    layout->addWidget(loginButton);

    // Кнопка регистрации (системный стиль)
    QPushButton *registerButton = new QPushButton("Регистрация", this);
    registerButton->setMinimumSize(100, 45); // Только размер, без стилей
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
    layout->addWidget(registerButton);

    // Подключение сигналов клиента
    connect(client, &Client::responseReceived, this, &LoginWindow::handleServerResponse);
    connect(client, &Client::connectionError, this, [this](const QString &error) {
        QMessageBox::critical(this, "Ошибка соединения", error);
    });

    // Устанавливаем objectName для главного виджета
    this->setObjectName("LoginWindow");
}

void LoginWindow::onLoginClicked() {
    // Получаем и очищаем введенные данные
    QString login = loginEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();

    // Валидация полей
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены");
        return;
    }

    if (login.length() < 4 || password.length() < 6) {
        QMessageBox::warning(this, "Ошибка", "Логин должен быть не менее 4 символов\nПароль - не менее 6 символов");
        return;
    }

    // Формируем JSON запрос
    QJsonObject request;
    request["type"] = "auth";
    request["login"] = login;
    request["password"] = password;

    // Отправляем запрос
    client->sendRequest(request);
}

void LoginWindow::onRegisterClicked() {
    // Эмитим сигнал для открытия окна регистрации
    emit openRegister();
}

void LoginWindow::handleServerResponse(const QJsonObject &response) {
    // Проверяем наличие обязательного поля type
    if (!response.contains("type")) {
        QMessageBox::critical(this, "Ошибка", "Некорректный ответ сервера");
        return;
    }

    QString type = response["type"].toString();
    
    // Обработка ответа на авторизацию
    if (type == "auth_response") {
        if (!response.contains("success")) {
            QMessageBox::critical(this, "Ошибка", "Некорректный формат ответа");
            return;
        }

        bool success = response["success"].toBool();
        QString message = response.contains("message") 
                        ? response["message"].toString() 
                        : "Неизвестная ошибка сервера";
        
        if (success) {
            QMessageBox::information(this, "Успех", "Авторизация прошла успешно");
            emit loginSuccess();
        } else {
            QMessageBox::warning(this, "Ошибка авторизации", message);
        }
    }
    // Обработка ошибок сервера
    else if (type == "error") {
        QString errorMsg = response.contains("message") 
                         ? response["message"].toString() 
                         : "Неизвестная ошибка";
        QMessageBox::critical(this, "Ошибка сервера", errorMsg);
    }
    // Неизвестный тип ответа
    else {
        QMessageBox::warning(this, "Ошибка", "Получен ответ неизвестного типа: " + type);
    }
}