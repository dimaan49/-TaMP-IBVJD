#include "database.h"

DataBase *DataBase::instance = nullptr;
DataBaseDestroyer DataBase::destroyer;

DataBase::DataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/Admin/git/Tamp/models.db");
    if (!db.open()) {
        qDebug() << "openDatabaseError\r\n";
    }
    else
    {
        qDebug() << "DataBase is started\n;";
    }
}
QSqlDatabase &DataBase::get_db() {
    return this->db;
}

bool DataBase::createTable()
{
    QSqlQuery query(db);
    return query.exec("CREATE TABLE users(id INTEGER PRIMARY KEY NOT NULL, name TEXT NOT NULL UNIQUE, password TEXT NOT NULL, email TEXT NOT NULL UNIQUE);");
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

User getUserById(int id) {
    qDebug() << "Try to write";
}

DataBaseDestroyer::~DataBaseDestroyer()
{
    delete instance;
}

void DataBaseDestroyer::initialize(DataBase *link)
{
    instance = link;
}
