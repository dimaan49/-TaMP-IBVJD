#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "client.h"

/**
 * @class TaskWindow
 * @brief Класс, представляющий окно выбора задач/вопросов.
 * 
 * Окно содержит кнопки для выбора вопроса, просмотра статистики и выхода.
 */
class TaskWindow : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса TaskWindow.
     * @param client Указатель на объект клиента (для возможного взаимодействия).
     * @param parent Родительский виджет (по умолчанию nullptr).
     */
    explicit TaskWindow(Client *client, QWidget *parent = nullptr);

signals:
    /**
     * @brief Сигнал запроса на отображение вопроса.
     * @param questionId Идентификатор вопроса.
     */
    void showQuestion(int questionId);

    /**
     * @brief Сигнал запроса на отображение статистики.
     */
    void showStats();

    /**
     * @brief Сигнал запроса на выход из приложения.
     */
    void exit();

private slots:
    /**
     * @brief Слот обработки нажатия на кнопку вопроса.
     * @param id Идентификатор вопроса.
     */
    void onQuestionClicked(int id);

    /**
     * @brief Слот обработки нажатия на кнопку статистики.
     */
    void onStatsClicked();

    /**
     * @brief Слот обработки нажатия на кнопку выхода.
     */
    void onExitClicked();

private:
    Client *client; ///< Указатель на клиентский объект.
};

#endif // TASKWINDOW_H
