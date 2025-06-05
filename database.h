#ifndef DATABASE
#define DATABASE
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QtSql>
#include <relations.h>

class DataBase;

/**
 * @brief Класс-разрушитель для синглтона базы данных
 * 
 * Этот класс отвечает за корректное уничтожение единственного экземпляра класса DataBase
 */
class DataBaseDestroyer
{
private:
    DataBase *instance; ///< Указатель на экземпляр базы данных
public:
    /**
     * @brief Деструктор класса
     * 
     * Освобождает память, занятую экземпляром базы данных
     */
    ~DataBaseDestroyer();

    /**
     * @brief Инициализирует указатель на экземпляр базы данных
     * @param link Указатель на экземпляр базы данных
     */
    void initialize(DataBase *link);
};

/**
 * @brief Класс для работы с базой данных (реализация паттерна Singleton)
 * 
 * Этот класс обеспечивает единую точку доступа к базе данных SQLite
 * и реализует основные операции для работы с ней
 */
class DataBase: public QObject
{
    Q_OBJECT
private:
    static DataBase *instance;           ///< Единственный экземпляр класса
    static DataBaseDestroyer destroyer;  ///< Объект-разрушитель
protected:
    QSqlDatabase db;                     ///< Объект базы данных

    /**
     * @brief Защищённый конструктор (паттерн Singleton)
     */
    DataBase();

    /**
     * @brief Защищённый деструктор
     */
    ~DataBase();
    
    /**
     * @brief Запрет копирования
     */
    DataBase(const DataBase &) = delete;
    
    /**
     * @brief Запрет присваивания
     */
    DataBase &operator = (const DataBase &) = delete;

    friend class DataBaseDestroyer;

public:
    /**
     * @brief Получить объект базы данных
     * @return Ссылка на объект QSqlDatabase
     */
    QSqlDatabase &get_db();

    /**
     * @brief Создать таблицу пользователей
     * @return true в случае успеха, false при ошибке
     */
    bool createTable();

    /**
     * @brief Получить экземпляр базы данных (реализация паттерна Singleton)
     * @return Ссылка на единственный экземпляр класса DataBase
     */
    static DataBase& get_instance();

    /**
     * @brief Получить пользователя по ID
     * @param id Идентификатор пользователя
     * @return Объект пользователя
     */
    User getUserById(int id);

    /**
     * @brief Получить роль пользователя
     * @param userId ID пользователя
     * @return Роль пользователя
     */
    UserRole getUserRole(int userId);

    /**
     * @brief Установить роль пользователя
     * @param userId ID пользователя
     * @param role Новая роль
     * @return true в случае успеха
     */
    bool setUserRole(int userId, UserRole role);

    /**
     * @brief Получить статистику всех пользователей
     * @return Список статистики пользователей
     */
    QList<UserStats> getAllUsersStats();

    /**
     * @brief Обновить статистику входа пользователя
     * @param userId ID пользователя
     * @return true в случае успеха
     */
    bool updateLoginStats(int userId);

    /**
     * @brief Удалить пользователя
     * @param userId ID пользователя
     * @return true в случае успеха
     */
    bool deleteUser(int userId);

    /**
     * @brief Обновить данные пользователя
     * @param userId ID пользователя
     * @param name Новое имя (пустое если не меняется)
     * @param password Новый пароль (пустой если не меняется)
     * @param email Новый email (пустой если не меняется)
     * @return true в случае успеха
     */
    bool updateUser(int userId, const QString& name = "", 
                   const QString& password = "", const QString& email = "");
};


#endif
