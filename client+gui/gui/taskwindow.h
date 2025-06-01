#ifndef TASKWINDOW_H
#define TASKWINDOW_H

#include <QWidget>
#include <QPushButton>
#include "client.h"

class TaskWindow : public QWidget {
    Q_OBJECT
public:
    explicit TaskWindow(Client *client, QWidget *parent = nullptr);

signals:
    void showQuestion(int questionId);
    void showStats();
    void exit();

private slots:
    void onQuestionClicked(int id);
    void onStatsClicked();
    void onExitClicked();

private:
    Client *client;
};

#endif // TASKWINDOW_H