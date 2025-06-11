#include "serverfunctions.h"

QString AUTHMESSAGE = "<auth> - Команда для аутентификации пользователя в системе\r\nСинтаксис: <auth> <login> <password>\r\n\r\n";
QString REGMESSAGE = "<reg> - Команда для создания нового пользователя в системе\r\nСинтаксис: <reg> <login> <password> <email>\r\n\r\n";
QString STATMESSAGE = "<lookall> - Команда для просмотра статистики всех пользователей (только для админа)\r\nСинтаксис: <lookall> <admin_login> <admin_password>\r\n\r\n";
QString UPDATEMESSAGE = "<updateuser> - Команда для обновления данных пользователя (только для админа)\r\nСинтаксис: <updateuser> <admin_login> <admin_password> <target_user> <new_name> <new_password> <new_email>\r\n\r\n";
QString VIGMESSAGE = "<vigener> - Команда для шифрования сообщения методом Виженера\r\nСинтаксис: <vigener> <text_message> <key>\r\n\r\n";
QString SHAMESSAGE = "<hash> - Команда для получения SHA1-хеша сообщения\r\nСинтаксис: <hash> <text_message>\r\n\r\n";
QString MUSMESSAGE = "<music> - Команда для скрытия сообщения в музыкальном файле\r\nСинтаксис: <music> <text_message> <music_file>\r\n\r\n";
QString NEWMESSAGE = "<newton> - Команда для поиска корней уравнения\r\nСинтаксис: <newton> <equation>\r\n\r\n";
QString HELPMESSAGE = AUTHMESSAGE + REGMESSAGE + STATMESSAGE + UPDATEMESSAGE + VIGMESSAGE + SHAMESSAGE + MUSMESSAGE + NEWMESSAGE;

// Аутентификация пользователя по имени и паролю
QByteArray authentication(QString name, QString password) {
    if (name.isEmpty() || password.isEmpty()) {
        return QByteArray("Имя пользователя и пароль не могут быть пустыми\n");
    }

    QSqlDatabase db = DataBase::get_instance().get_db();
    QSqlQuery query(db);
    
    // Сначала находим пользователя
    query.prepare("SELECT id, name, password FROM users WHERE name = :name;");
    query.bindValue(":name", name);
    
    if (!query.exec()) {
        return QByteArray("Ошибка базы данных при аутентификации\n");
    }
    
    if (!query.next()) {
        return QByteArray("Пользователь не найден\n");
    }
    
    // Проверяем пароль
    if (query.value("password").toString() != password) {
        return QByteArray("Неверный пароль\n");
    }
    
    // Обновляем статистику входа
    int userId = query.value("id").toInt();
    DataBase::get_instance().updateLoginStats(userId);
    
    // Получаем роль пользователя
    UserRole role = DataBase::get_instance().getUserRole(userId);
    QString roleStr;
    switch(role) {
        case UserRole::ADMIN: roleStr = "администратор"; break;
        case UserRole::USER: roleStr = "пользователь"; break;
        default: roleStr = "гость";
    }
    
    QString successMsg = QString("Аутентификация успешна! Вы вошли как %1 (роль: %2)\n")
                        .arg(name)
                        .arg(roleStr);
    return successMsg.toUtf8();
}

// Регистрация нового пользователя
QByteArray registration(QString name, QString password, QString email) {
    QSqlDatabase db = DataBase::get_instance().get_db();
    QSqlQuery query(db);
    query.prepare("SELECT name FROM users WHERE name = :name;");
    query.bindValue(":name", name);
    query.exec();
    if (query.isActive()) {
    if (query.next()) {
        return QByteArray("This is user is already exist");
    }
    else {
        query.prepare("INSERT INTO users (name, password, email) VALUES (:name, :password, :email);");
        query.bindValue(":name", name);
        query.bindValue(":password", password);
        query.bindValue(":email", email);
        query.exec();
        query.clear();
        QString mes = "you are " + name + " !\r\n";
        QByteArray message = mes.toUtf8();
        return QByteArray(message);
    }
    }

}

// Получить полную статистику по всем пользователям системы (доступно только администратору)
QByteArray lookallstat(QString adminName, QString adminPassword) {
    QSqlDatabase db = DataBase::get_instance().get_db();
    QSqlQuery query(db);
    
    // Проверяем права администратора
    query.prepare("SELECT id FROM users WHERE name = :name AND password = :password");
    query.bindValue(":name", adminName);
    query.bindValue(":password", adminPassword);
    
    if (!query.exec() || !query.next()) {
        return QByteArray("Ошибка аутентификации администратора\n");
    }
    
    int adminId = query.value(0).toInt();
    UserRole role = DataBase::get_instance().getUserRole(adminId);
    
    if (role != UserRole::ADMIN) {
        return QByteArray("Недостаточно прав для просмотра статистики\n");
    }
    
    // Получаем статистику всех пользователей
    QList<UserStats> stats = DataBase::get_instance().getAllUsersStats();
    QString result = "Статистика пользователей:\n\n";
    
    for (const UserStats& stat : stats) {
        result += QString("ID: %1\n").arg(stat.userId);
        result += QString("Имя: %1\n").arg(stat.userName);
        result += QString("Роль: %1\n").arg(static_cast<int>(stat.role));
        result += QString("Количество входов: %1\n").arg(stat.loginCount);
        if (stat.lastLogin.isValid()) {
            result += QString("Последний вход: %1\n").arg(stat.lastLogin.toString("dd.MM.yyyy hh:mm:ss"));
        }
        result += "\n";
    }
    
    return result.toUtf8();
}

// Обновление данных пользователя (имени, пароля, email) (доступно только администратору)
QByteArray updateUserData(QString adminName, QString adminPassword, QString targetUser, 
                         QString newName, QString newPassword, QString newEmail) {
    QSqlDatabase db = DataBase::get_instance().get_db();
    QSqlQuery query(db);
    
    // Проверяем права администратора
    query.prepare("SELECT id FROM users WHERE name = :name AND password = :password");
    query.bindValue(":name", adminName);
    query.bindValue(":password", adminPassword);
    
    if (!query.exec() || !query.next()) {
        return QByteArray("Ошибка аутентификации администратора\n");
    }
    
    int adminId = query.value(0).toInt();
    UserRole role = DataBase::get_instance().getUserRole(adminId);
    
    if (role != UserRole::ADMIN) {
        return QByteArray("Недостаточно прав для обновления данных пользователей\n");
    }
    
    // Находим ID целевого пользователя
    query.prepare("SELECT id FROM users WHERE name = :name");
    query.bindValue(":name", targetUser);
    
    if (!query.exec() || !query.next()) {
        return QByteArray("Пользователь не найден\n");
    }
    
    int userId = query.value(0).toInt();
    
    // Обновляем данные пользователя
    if (DataBase::get_instance().updateUser(userId, newName, newPassword, newEmail)) {
        return QByteArray("Данные пользователя успешно обновлены\n");
    } else {
        return QByteArray("Ошибка при обновлении данных пользователя\n");
    }
}

// Шифрование текста с помощью шифра Виженера
QByteArray vigenereCipher(QString text, QString key) {
    QString encrypted = Encrypt(text, key);
    return QByteArray(encrypted.toUtf8());
}

// Преобразование сообщения в SHA1-хеш
QByteArray messageToSha1(QString message) {
    qDebug() << "it`s funcition for transform message in sha1 hash\n";
    QByteArray byteArray = message.toUtf8();

    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);

    QByteArray result;
    result.append("SHA-1 hash (hex): " + hash.toHex() + "\r\n");
    result.append("Hashing message: " + message.toUtf8() + "\r\n");

    return result;
}

// Внедрение сообщения в музыкальный файл
QByteArray messageInMusic(QString message, QString musicFilePath) {
    qDebug() << "it`s function for hide text message in the music file\n";
    qDebug() << "Hiding message in music file:" << musicFilePath;

    QByteArray messageData = message.toUtf8();
    quint32 messageSize = messageData.size();

    QFile audioFile(musicFilePath);
    if (!audioFile.open(QIODevice::ReadWrite)) {
        return QByteArray("Error: Could not open audio file\r\n");
    }

    QByteArray audioData = audioFile.readAll();

    if (audioData.size() < 44) {
        audioFile.close();
        return QByteArray("Error: Invalid WAV file format\r\n");
    }


    quint32 maxMessageSize = (audioData.size() - 44) / 8;
    if (messageSize > maxMessageSize) {
        audioFile.close();
        return QByteArray("Error: Message too large for this audio file\r\n");
    }

    for (int i = 0; i < 32; i++) {
        quint32 pos = 44 + i;
        if (pos >= audioData.size()) break;

        audioData[pos] = (audioData.at(pos) & 0xFE) | ((messageSize >> (31 - i)) & 1);
    }

    for (quint32 i = 0; i < messageSize; i++) {
        for (int bit = 0; bit < 8; bit++) {
            quint32 pos = 44 + 32 + i * 8 + bit;
            if (pos >= audioData.size()) break;

            char byte = messageData.at(i);
            audioData[pos] = (audioData.at(pos) & 0xFE) | ((byte >> (7 - bit)) & 1);
        }
    }

    audioFile.seek(0);
    audioFile.write(audioData);
    audioFile.close();

    return QByteArray("Message successfully hidden in audio file\r\n");
}

// Декодирование сообщения из музыкального файла
QString extractMessageFromMusic(QString musicFilePath) {
    QFile audioFile(musicFilePath);
    if (!audioFile.open(QIODevice::ReadOnly)) {
        return "Error: Could not open audio file";
    }

    QByteArray audioData = audioFile.readAll();
    audioFile.close();

    if (audioData.size() < 44 + 32) {
        return "Error: File too small to contain message";
    }

    quint32 messageSize = 0;
    for (int i = 0; i < 32; i++) {
        quint32 pos = 44 + i;
        if (pos >= audioData.size()) break;
        messageSize = (messageSize << 1) | (audioData.at(pos) & 1);
    }

    QByteArray messageData;
    for (quint32 i = 0; i < messageSize; i++) {
        char byte = 0;
        for (int bit = 0; bit < 8; bit++) {
            quint32 pos = 44 + 32 + i * 8 + bit;
            if (pos >= audioData.size()) break;
            byte = (byte << 1) | (audioData.at(pos) & 1);
        }
        messageData.append(byte);
    }

    return QString::fromUtf8(messageData);
}

// Реализация шифра Виженера 
QString Encrypt(QString text, QString key) {
    QString res;
    int tlen = text.length(), klen = key.length();
    for (int i = 0; i < tlen; ++i) {
        QChar tc = text[i], kc = key[i % klen];
        int t = tc.unicode() - (tc.isUpper() ? 'A' : 'a');
        int k = kc.unicode() - (kc.isUpper() ? 'A' : 'a');
        int c = (t + k) % 26;
        res += QChar(c + (tc.isUpper() ? 'A' : 'a'));
    }
    return res;
}

// Восстановление исходного текста
QString Decrypt(QString text, QString key) {
    QString res;
    int tlen = text.length(), klen = key.length();
    for (int i = 0; i < tlen; ++i) {
        QChar tc = text[i], kc = key[i % klen];
        int t = tc.unicode() - (tc.isUpper() ? 'A' : 'a');
        int k = kc.unicode() - (kc.isUpper() ? 'A' : 'a');
        int c = (t - k + 26) % 26;
        res += QChar(c + (tc.isUpper() ? 'A' : 'a'));
    }
    return res;
}

// Разбор строки уравнения 
Equation parseEquation(const QString& equation_str) {
    Equation eq = {0, 0, 0}; // По умолчанию все коэффициенты 0
    
    // Убираем пробелы и "= 0" из строки
    QString str = equation_str.simplified();
    str.remove(" ");
    str.remove("=0");
    
    // Добавляем '+' перед отрицательными числами для упрощения парсинга
    str.replace("-", "+-");
    if (str.startsWith("+")) {
        str.remove(0, 1);
    }
    
    // Разбиваем на члены
    QStringList terms = str.split("+", Qt::SkipEmptyParts);
    
    for (const QString& term : terms) {
        if (term.contains("x^2")) {
            QString coef = term.left(term.indexOf("x^2"));
            eq.a = coef.isEmpty() ? 1 : (coef == "-" ? -1 : coef.toDouble());
        }
        else if (term.contains("x")) {
            QString coef = term.left(term.indexOf("x"));
            eq.b = coef.isEmpty() ? 1 : (coef == "-" ? -1 : coef.toDouble());
        }
        else {
            eq.c = term.toDouble();
        }
    }
    
    return eq; // Возвращаем структуру с коэффициентами
}

// Реализация метода Ньютона для поиска корня уравнения
double findRoot(const Equation& eq, double x0, double epsilon, int max_iter) {
    double x = x0;
    
    for (int i = 0; i < max_iter; ++i) {
        double f = eq.evaluate(x);
        double df = eq.derivative(x);
        
        // Если производная близка к нулю, метод может не сойтись
        if (std::abs(df) < epsilon) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        
        double x_new = x - f / df;
        
        // Если достигнута требуемая точность
        if (std::abs(x_new - x) < epsilon) {
            return x_new;
        }
        
        x = x_new;
    }
    
    // Если за максимальное число итераций корень не найден
    return std::numeric_limits<double>::quiet_NaN();
}

// Используем ранее реализованные функции
double rootByNewton(const QString& equation_str) {
    Equation eq = parseEquation(equation_str); // Парсим уравнение
    return findRoot(eq); // Находим корень методом Ньютона
}

// Анализ входящих сообщений, возврат ответа в зависимости от команды
QByteArray queryAnalyzer(QString message) {
    QStringList parts = message.split("&", Qt::SkipEmptyParts);
    parts.last().remove("\r\n");
    int parts_quantity = parts.size();

    if (parts.at(0) == "help") {
        return QByteArray(HELPMESSAGE.toUtf8());
    }
    else if (parts.at(0) == "auth" && parts.length() > 2)
    {
        QByteArray authResult = authentication(parts.at(1), parts.at(2));
        if (authResult.startsWith("Authentication is access")) {
            // Обновляем статистику входа
            QSqlQuery query;
            query.prepare("SELECT id FROM users WHERE name = :name");
            query.bindValue(":name", parts.at(1));
            if (query.exec() && query.next()) {
                DataBase::get_instance().updateLoginStats(query.value(0).toInt());
            }
        }
        return authResult;
    }
    else if (parts.at(0) == "reg" && parts.length() > 3)
    {
        return registration(parts.at(1), parts.at(2), parts.at(3));
    }
    else if (parts.at(0) == "lookall" && parts.length() > 2)
    {
        return lookallstat(parts.at(1), parts.at(2));
    }
    else if (parts.at(0) == "updateuser" && parts.length() > 5) {
        return updateUserData(parts.at(1), parts.at(2), parts.at(3), 
                            parts.at(4), parts.at(5), parts.value(6));
    }
    else if (parts.at(0) == "vigener" && parts.length() > 2)
    {
        return vigenereCipher(parts.at(1), parts.at(2));
    }
    else if (parts.at(0) == "hash" && parts.length() > 1)
    {
        return messageToSha1(parts.at(1));
    }
    else if (parts.at(0) == "music" && parts.length() > 0)
    {
        return messageInMusic(parts.at(1), parts.at(2));
    }
    // Декодирование сообщения из музыкального файла
    else if (parts.at(0) == "extract" && parts.length() > 1) {
        return extractMessageFromMusic(parts.at(1)).toUtf8();
    }
    else if (parts.at(0) == "newton" && parts.length() > 1)
{
    double root = rootByNewton(parts.at(1));
    if (std::isnan(root)) {
        return QByteArray("Корень не найден или метод не сошелся");
    }
    QString result = QString("Найденный корень: %1").arg(root);
    return result.toUtf8();
} else {
    return QByteArray();
}
}
