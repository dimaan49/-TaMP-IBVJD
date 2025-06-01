#ifndef QUESTIONWINDOW_H
#define QUESTIONWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include "client.h"

class QuestionWindow : public QWidget {
    Q_OBJECT
public:
    explicit QuestionWindow(Client *client, int questionId, QWidget *parent = nullptr);

signals:
    void backToTasks();

private slots:
    void onSubmitClicked();

private:
    Client *client;
    int questionId;
    QLineEdit *answerEdit;
};

#endif // QUESTIONWINDOW_H