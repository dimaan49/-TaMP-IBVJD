#include "questionwindow.h"
#include <QVBoxLayout>
#include <QLabel>

QuestionWindow::QuestionWindow(Client *client, int questionId, QWidget *parent)
    : QWidget(parent), client(client), questionId(questionId) {
    // Установка фиолетового фона для окна вопроса
    this->setStyleSheet("QuestionWindow { background-color: #aa00ff; }");
    
    QVBoxLayout *layout = new QVBoxLayout(this);

    QString questionText;
    switch (questionId) {
        case 1: questionText = "2 + 2 = ?"; break;
        case 2: questionText = "3 * 5 = ?"; break;
        case 3: questionText = "10 / 2 = ?"; break;
        case 4: questionText = "7 - 4 = ?"; break;
    }

    QLabel *questionLabel = new QLabel(questionText, this);
    layout->addWidget(questionLabel);

    answerEdit = new QLineEdit(this);
    answerEdit->setPlaceholderText("Ответ");
    layout->addWidget(answerEdit);

    QPushButton *submitButton = new QPushButton("OK", this);
    connect(submitButton, &QPushButton::clicked, this, &QuestionWindow::onSubmitClicked);
    layout->addWidget(submitButton);
}

void QuestionWindow::onSubmitClicked() {
    QJsonObject request;
    request["type"] = "submit_answer";
    request["question_id"] = questionId;
    request["answer"] = answerEdit->text();

    client->sendRequest(request);
    emit backToTasks();
}