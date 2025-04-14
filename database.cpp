#include "database.h"

DataBase *DataBase::instance = nullptr;
DataBaseDestroyer DataBase::destroyer;

DataBase::DataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("models.db");
    if (!db.open()) {
        qDebug() << "openDatabaseError\r\n";
    }
    else
    {
        qDebug() << "DataBase is started\n;";
    }
}

bool DataBase::createTable()
{
    QSqlQuery query(db);
    return query.exec("create table users(id integer primary key not null, login text not null unique, password text not null, email text not null unique UNION CREATE table admins (id integer primary key not null, login text not null unique, password text not null, email text not null unique));");
}

DataBase::~DataBase()
{
    db.close();
}

DataBase &DataBase::get_instance()
{
    if (!instance)
    {
        instance = new DataBase();
        destroyer.initialize(instance);
        if (!instance->db.tables().contains(QStringLiteral("users"))) {
            instance->createTable();
        }
    }
    return *instance;
}

DataBaseDestroyer::~DataBaseDestroyer()
{
    delete instance;
}

void DataBaseDestroyer::initialize(DataBase *link)
{
    instance = link;
}
