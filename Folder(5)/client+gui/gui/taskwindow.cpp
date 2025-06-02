#include "taskwindow.h"
#include <QVBoxLayout>

TaskWindow::TaskWindow(Client *client, QWidget *parent) : QWidget(parent), client(client) {
    // Установка фиолетового фона для окна задач
    this->setStyleSheet("TaskWindow { background-color: #aa00ff; }");
    
    QVBoxLayout *layout = new QVBoxLayout(this);

    for (int i = 1; i <= 4; ++i) {
        QPushButton *button = new QPushButton("Вопрос " + QString::number(i), this);
        connect(button, &QPushButton::clicked, [this, i]() { onQuestionClicked(i); });
        layout->addWidget(button);
    }

    QPushButton *statsButton = new QPushButton("Статистика", this);
    connect(statsButton, &QPushButton::clicked, this, &TaskWindow::onStatsClicked);
    layout->addWidget(statsButton);

    QPushButton *exitButton = new QPushButton("Выход", this);
    connect(exitButton, &QPushButton::clicked, this, &TaskWindow::onExitClicked);
    layout->addWidget(exitButton);
}

void TaskWindow::onQuestionClicked(int id) {
    emit showQuestion(id);
}

void TaskWindow::onStatsClicked() {
    emit showStats();
}

void TaskWindow::onExitClicked() {
    emit exit();
}