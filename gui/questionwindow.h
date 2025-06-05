#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "client.h"


/**
 * @class QuestionWindow
 * @brief Класс окна для отображения и ответа на вопрос
 *
 * Окно отображает вопрос в зависимости от переданного идентификатора
 * и предоставляет интерфейс для ввода ответа пользователем.
 */
class QuestionWindow : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса QuestionWindow
     * @param client Указатель на объект Client для связи с сервером
     * @param questionId Идентификатор вопроса (1-4)
     * @param parent Родительский виджет (по умолчанию nullptr)
     */
    explicit QuestionWindow(Client *client, int questionId, QWidget *parent = nullptr);

signals:
    /**
     * @brief Сигнал возврата к окну задач
     */
    void backToTasks();

private slots:
    /**
     * @brief Обработчик нажатия кнопки отправки ответа
     */
    void onSubmitClicked();

private:
    Client *client;         ///< Указатель на объект Client для связи с сервером
    int questionId;         ///< Идентификатор текущего вопроса
    QLineEdit *answerEdit;  ///< Поле для ввода ответа
};

#endif // QUESTIONWINDOW_H
