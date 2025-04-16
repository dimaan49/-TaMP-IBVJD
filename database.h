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

class DataBaseDestroyer
{
private:
    DataBase *instance;
public:
    ~DataBaseDestroyer();
    void initialize(DataBase *link);
};

class DataBase: public QObject
{
    Q_OBJECT
private:
    static DataBase *instance;
    static DataBaseDestroyer destroyer;
protected:
    QSqlDatabase db;
    DataBase();
    ~DataBase();
    DataBase(const DataBase &) = delete;
    DataBase &operator = (const DataBase &) = delete;
    friend class DataBaseDestroyer;
public:
    QSqlDatabase &get_db();
    bool createTable();
    static DataBase& get_instance();
    User getUserById(int id);
};


#endif
