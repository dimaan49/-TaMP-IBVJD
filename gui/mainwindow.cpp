#include "mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(Client *client, QWidget *parent) : QStackedWidget(parent), client(client) {
    // Установка фиолетового фона для главного окна
    this->setStyleSheet("MainWindow { background-color: #aa00ff; }");
    
    loginWindow = new LoginWindow(client);
    registerWindow = new RegisterWindow(client);
    taskWindow = new TaskWindow(client);
    questionWindow = nullptr;

    addWidget(loginWindow);
    addWidget(registerWindow);
    addWidget(taskWindow);

    connect(loginWindow, &LoginWindow::loginSuccess, this, &MainWindow::onLoginSuccess);
    connect(loginWindow, &LoginWindow::openRegister, this, &MainWindow::onOpenRegister);
    connect(registerWindow, &RegisterWindow::backToLogin, this, &MainWindow::onBackToLogin);
    connect(taskWindow, &TaskWindow::showQuestion, this, &MainWindow::onShowQuestion);
    connect(taskWindow, &TaskWindow::showStats, this, &MainWindow::onShowStats);
    connect(taskWindow, &TaskWindow::exit, this, &MainWindow::onExit);
}

void MainWindow::onLoginSuccess() {
    setCurrentWidget(taskWindow);
}

void MainWindow::onOpenRegister() {
    setCurrentWidget(registerWindow);
}

void MainWindow::onBackToLogin() {
    setCurrentWidget(loginWindow);
}

void MainWindow::onShowQuestion(int id) {
    if (questionWindow) delete questionWindow;
    questionWindow = new QuestionWindow(client, id);
    addWidget(questionWindow);
    setCurrentWidget(questionWindow);
    connect(questionWindow, &QuestionWindow::backToTasks, this, [this]() {
        setCurrentWidget(taskWindow);
    });
}

void MainWindow::onShowStats() {
    QMessageBox::information(this, "Статистика", "Здесь будет статистика.");
}

void MainWindow::onExit() {
    close();
}