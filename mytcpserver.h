#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>
#include <QDebug>
#include <QHostAddress>
#include <QMap>

/**
 * @brief Класс TCP-сервера
 * 
 * Реализует основную функциональность сервера:
 * - Прослушивание входящих подключений
 * - Обработка подключений клиентов
 * - Обработка входящих сообщений
 * - Отправка ответов клиентам
 */
class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор сервера
     * @param parent Родительский объект
     * @param server_port Порт для прослушивания (по умолчанию 33333)
     * @param server_address IP-адрес сервера (по умолчанию 127.0.0.1)
     */
    explicit MyTcpServer(QObject *parent = nullptr,  int server_port = 33333, QHostAddress server_address = QHostAddress("127.0.0.1"));
    /**
     * @brief Деструктор сервера
     * 
     * Закрывает все соединения и освобождает ресурсы
     */
    ~MyTcpServer();
public slots:
    /**
     * @brief Обработчик нового подключения
     * 
     * Вызывается при подключении нового клиента
     */
    void slotNewConnection();

    /**
     * @brief Обработчик отключения клиента
     * 
     * Вызывается при отключении клиента
     */
    void slotClientDisconnected();

    /**
     * @brief Обработчик входящих данных
     * 
     * Вызывается при получении данных от клиента
     */
    void slotServerRead();
private:
    QTcpServer * mTcpServer;  ///< Указатель на объект TCP-сервера
    QMap<int, QTcpSocket*> sockArray;  ///< Карта активных сокетов
};
#endif
