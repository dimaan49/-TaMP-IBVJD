#include "database.h"

DataBase *DataBase::instance = nullptr;
DataBaseDestroyer DataBase::destroyer;

DataBase::DataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QCoreApplication::applicationDirPath() + "/database.db";
    db.setDatabaseName(dbPath);
    if (!db.open()) {
        qDebug() << "Ошибка открытия базы данных:" << db.lastError().text();
    }
    else {
        qDebug() << "База данных успешно открыта";
    }
}
QSqlDatabase &DataBase::get_db() {
    return this->db;
}

bool DataBase::createTables() {
    QSqlQuery query;
    // Создаем основную таблицу пользователей
    if (!query.exec("CREATE TABLE IF NOT EXISTS users("
                   "id INTEGER PRIMARY KEY NOT NULL, "
                   "name TEXT NOT NULL UNIQUE, "
                   "password TEXT NOT NULL, "
                   "email TEXT NOT NULL UNIQUE);")) {
        qDebug() << "Error creating users table:" << query.lastError().text();
        return false;
    }

    // Создаем таблицы для ролей и статистики
    if (!createRolesAndStatsTable()) {
        return false;
    }

    // Создаем администратора по умолчанию, если его нет
    query.prepare("SELECT id FROM users WHERE name = 'admin'");
    if (query.exec() && !query.next()) {
        query.prepare("INSERT INTO users (name, password, email) VALUES ('admin', 'admin', 'admin@localhost')");
        if (!query.exec()) {
            qDebug() << "Error creating default admin:" << query.lastError().text();
            return false;
        }

        // Получаем ID созданного админа
        int adminId = query.lastInsertId().toInt();

        // Устанавливаем роль администратора
        query.prepare("INSERT INTO user_roles (user_id, role) VALUES (:id, :role)");
        query.bindValue(":id", adminId);
        query.bindValue(":role", static_cast<int>(UserRole::ADMIN));
        if (!query.exec()) {
            qDebug() << "Error setting admin role:" << query.lastError().text();
            return false;
        }
    }

    return true;
}

bool DataBase::createRolesAndStatsTable() {
    QSqlQuery query;
    // Создаем таблицу ролей если её нет
    if (!query.exec("CREATE TABLE IF NOT EXISTS user_roles ("
                   "user_id INTEGER PRIMARY KEY,"
                   "role INTEGER NOT NULL DEFAULT 1,"
                   "FOREIGN KEY(user_id) REFERENCES users(id))")) {
        qDebug() << "Error creating user_roles table:" << query.lastError().text();
        return false;
    }

    // Создаем таблицу статистики если её нет
    if (!query.exec("CREATE TABLE IF NOT EXISTS user_stats ("
                   "user_id INTEGER PRIMARY KEY,"
                   "login_count INTEGER NOT NULL DEFAULT 0,"
                   "last_login DATETIME,"
                   "FOREIGN KEY(user_id) REFERENCES users(id))")) {
        qDebug() << "Error creating user_stats table:" << query.lastError().text();
        return false;
    }

    return true;
}

UserRole DataBase::getUserRole(int userId) {
    QSqlQuery query;
    query.prepare("SELECT role FROM user_roles WHERE user_id = :id");
    query.bindValue(":id", userId);
    
    if (query.exec() && query.next()) {
        return static_cast<UserRole>(query.value(0).toInt());
    }
    return UserRole::USER; // По умолчанию обычный пользователь
}

bool DataBase::setUserRole(int userId, UserRole role) {
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO user_roles (user_id, role) VALUES (:id, :role)");
    query.bindValue(":id", userId);
    query.bindValue(":role", static_cast<int>(role));
    
    return query.exec();
}

QList<UserStats> DataBase::getAllUsersStats() {
    QList<UserStats> stats;
    QSqlQuery query;
    
    if (query.exec("SELECT u.id, u.name, r.role, s.login_count, s.last_login "
                  "FROM users u "
                  "LEFT JOIN user_roles r ON u.id = r.user_id "
                  "LEFT JOIN user_stats s ON u.id = s.user_id")) {
        
        while (query.next()) {
            UserStats stat;
            stat.userId = query.value(0).toInt();
            stat.userName = query.value(1).toString();
            stat.role = static_cast<UserRole>(query.value(2).toInt());
            stat.loginCount = query.value(3).toInt();
            stat.lastLogin = query.value(4).toDateTime();
            stats.append(stat);
        }
    }
    
    return stats;
}

bool DataBase::updateLoginStats(int userId) {
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO user_stats (user_id, login_count, last_login) "
                 "VALUES (:id, "
                 "COALESCE((SELECT login_count + 1 FROM user_stats WHERE user_id = :id2), 1), "
                 "CURRENT_TIMESTAMP)");
    query.bindValue(":id", userId);
    query.bindValue(":id2", userId);
    
    return query.exec();
}

bool DataBase::deleteUser(int userId) {
    // Проверяем, не пытаемся ли удалить последнего администратора
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM user_roles WHERE role = :admin_role");
    checkQuery.bindValue(":admin_role", static_cast<int>(UserRole::ADMIN));
    
    if (!checkQuery.exec() || !checkQuery.next()) {
        return false;
    }
    
    int adminCount = checkQuery.value(0).toInt();
    
    // Проверяем, является ли удаляемый пользователь администратором
    checkQuery.prepare("SELECT role FROM user_roles WHERE user_id = :id");
    checkQuery.bindValue(":id", userId);
    
    if (!checkQuery.exec() || !checkQuery.next()) {
        return false;
    }
    
    UserRole userRole = static_cast<UserRole>(checkQuery.value(0).toInt());
    
    // Если это последний администратор, запрещаем удаление
    if (adminCount <= 1 && userRole == UserRole::ADMIN) {
        qDebug() << "Невозможно удалить последнего администратора";
        return false;
    }

    QSqlQuery query;
    // Начинаем транзакцию
    if (!query.exec("BEGIN TRANSACTION")) return false;
    
    // Удаляем записи из всех связанных таблиц
    query.prepare("DELETE FROM user_stats WHERE user_id = :id");
    query.bindValue(":id", userId);
    if (!query.exec()) {
        query.exec("ROLLBACK");
        return false;
    }
    
    query.prepare("DELETE FROM user_roles WHERE user_id = :id");
    query.bindValue(":id", userId);
    if (!query.exec()) {
        query.exec("ROLLBACK");
        return false;
    }
    
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", userId);
    if (!query.exec()) {
        query.exec("ROLLBACK");
        return false;
    }
    
    return query.exec("COMMIT");
}

bool DataBase::updateUser(int userId, const QString& name, const QString& password, const QString& email) {
    QStringList updates;
    QSqlQuery query;
    
    if (!name.isEmpty()) updates << "name = :name";
    if (!password.isEmpty()) updates << "password = :password";
    if (!email.isEmpty()) updates << "email = :email";
    
    if (updates.isEmpty()) return true; // Нечего обновлять
    
    QString queryStr = "UPDATE users SET " + updates.join(", ") + " WHERE id = :id";
    query.prepare(queryStr);
    
    if (!name.isEmpty()) query.bindValue(":name", name);
    if (!password.isEmpty()) query.bindValue(":password", password);
    if (!email.isEmpty()) query.bindValue(":email", email);
    query.bindValue(":id", userId);
    
    return query.exec();
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
            instance->createTables();
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
