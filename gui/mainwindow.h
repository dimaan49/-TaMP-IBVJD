#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include "loginwindow.h"
#include "registerwindow.h"
#include "taskwindow.h"
#include "questionwindow.h"
#include "client.h"


/**
 * @class MainWindow
 * @brief Класс главного окна приложения, использующий QStackedWidget
 *
 * Управляет переключением между различными экранами приложения:
 * - Окно входа (LoginWindow)
 * - Окно регистрации (RegisterWindow)
 * - Окно задач (TaskWindow)
 * - Окно вопроса (QuestionWindow)
 */
class MainWindow : public QStackedWidget {
    Q_OBJECT
public:
    /**
     * @brief Конструктор MainWindow
     * @param client Указатель на объект Client для сетевого взаимодействия
     * @param parent Родительский виджет
     */
    explicit MainWindow(Client *client, QWidget *parent = nullptr);

private slots:
    /**
     * @brief Обработчик успешного входа
     */
    void onLoginSuccess();
    
    /**
     * @brief Обработчик перехода к окну регистрации
     */
    void onOpenRegister();
    
    /**
     * @brief Обработчик возврата к окну входа
     */
    void onBackToLogin();
    
    /**
     * @brief Обработчик отображения окна вопроса
     * @param id Идентификатор вопроса
     */
    void onShowQuestion(int id);
    
    /**
     * @brief Обработчик отображения статистики
     */
    void onShowStats();
    
    /**
     * @brief Обработчик выхода из приложения
     */
    void onExit();

private:
    Client *client;                 ///< Указатель на клиент для сетевого взаимодействия
    LoginWindow *loginWindow;       ///< Указатель на окно входа
    RegisterWindow *registerWindow; ///< Указатель на окно регистрации
    TaskWindow *taskWindow;         ///< Указатель на окно задач
    QuestionWindow *questionWindow; ///< Указатель на окно вопроса (создается динамически)
};

#endif // MAINWINDOW_H
