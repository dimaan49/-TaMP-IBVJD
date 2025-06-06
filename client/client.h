#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

/**
 * @class Client
 * @brief Класс для сетевого взаимодействия с сервером
 *
 * Обеспечивает:
 * - Подключение к серверу по TCP
 * - Отправку JSON-запросов
 * - Получение и парсинг JSON-ответов
 * - Обработку ошибок соединения
 */
class Client : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Конструктор класса Client
     * @param parent Родительский QObject
     */
    explicit Client(QObject *parent = nullptr);
    
    /**
     * @brief Подключение к серверу
     * @param ip IP-адрес сервера
     * @param port Порт сервера
     */
    void connectToServer(const QString &ip, quint16 port);
    
    /**
     * @brief Отправка запроса на сервер
     * @param request JSON-объект с данными запроса
     */
    void sendRequest(const QJsonObject &request);

signals:
    /**
     * @brief Сигнал получения ответа от сервера
     * @param response JSON-объект с ответом сервера
     */
    void responseReceived(const QJsonObject &response);
    
    /**
     * @brief Сигнал ошибки соединения
     * @param error Текст ошибки
     */
    void connectionError(const QString &error);

private slots:
    /**
     * @brief Слот обработки входящих данных
     */
    void onReadyRead();

private:
    QTcpSocket *socket; ///< Сокет для TCP-соединения
};

#endif // CLIENT_H
