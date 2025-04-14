#include <serverfunctions.h>

QString AUTHMESSAGE = "<auth> - Command to authenticate user in the system\r\nCommon syntax: <auth> <login> <pasword>\r\n\r\n";
QString REGMESSAGE = "<reg> - Command to create new user in the system\r\nCommon syntax: <reg> <login> <password> <email>\r\n\r\n";
QString STATMESSAGE = "<lookall> - Command to view all stat in the system\r\nCommon syntax: <lookall> \r\n\r\n";
QString VIGMESSAGE = "<vigener> - Command to transform message in cipher with Vigenere incryption\r\nCommon syntax: <vigener> <text_message>\r\n\r\n";
QString SHAMESSAGE = "<hash> - Command to view sha1 hash of the message\r\nCommon syntax: <hash> <text_message>\r\n\r\n";
QString MUSMESSAGE = "<music> - Command to hide message in the music file\r\nCommon syntax: <music> <text_message> <music_file>\r\n\r\n";
QString NEWMESSAGE = "<newton> - Command to find all root of the equation\r\nCommon syntax: <equation>\r\n\r\n";
QString HELPMESSAGE = AUTHMESSAGE + REGMESSAGE + STATMESSAGE + VIGMESSAGE  + SHAMESSAGE + MUSMESSAGE + NEWMESSAGE;

QByteArray authentication(QString name, QString password) {
    QSqlDatabase db = DataBase::get_instance().get_db();
    QSqlQuery query(db);
    query.prepare("SELECT name, password FROM users WHERE name = :name and password = :password;");
    query.bindValue(":name", name);
    query.bindValue(":password", password);
    query.exec();
    if (!query.next()) {
        return  QByteArray("User isn`t found\n\r;");
    }
    else {
        return QByteArray("Authentication is access!\r\n");
    }
}

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


QByteArray lookallstat(QString a, QString b) {
    qDebug() << "it`s funcition for viewing all stat\n";
    return QByteArray();
}
QByteArray vigenereCipher(QString a) {
    qDebug() << "it`s Vegener`s cipher function\n";
    return QByteArray();
}
QByteArray messageToSha1(QString a) {
    qDebug() << "it`s funcition for transform message in sha1 hash\n";
    return QByteArray();
}
QByteArray messageInMusic() {
    qDebug() << "it`s function for hide text message in the music file\n";
    return QByteArray();
}
QString Encrypt(QString a, QString b , QString c) {
    qDebug() << "it`s ecnrypt funcition\n";
    return QByteArray();
}
QString Decrypt(QString a, QString b, QString c) {
    qDebug() << "it`s decrypt funcition\n";
    return QByteArray();
}
double rootByNewton() {
    return 1;
}

QByteArray queryAnalyzer(QString message) {
        QStringList parts = message.split("&", Qt::SkipEmptyParts);
        parts.last().remove("\r\n");
        int parts_quantity = parts.size();

        if (parts.at(0) == "help") {
            return QByteArray(HELPMESSAGE.toUtf8());
        }
        else if (parts.at(0) == "auth" && parts.length() > 2)
        {
            return authentication(parts.at(1), parts.at(2));
        }
        else if (parts.at(0) == "reg" && parts.length() > 3)
        {
            return registration(parts.at(1), parts.at(2), parts.at(3));
        }
        else if (parts.at(0) == "lookall" && parts.length() > 2)
        {
            return lookallstat(parts.at(1), parts.at(2));
        }
        else if (parts.at(0) == "vigener" && parts.length() > 1)
        {
            return vigenereCipher(parts.at(1));
        }
        else if (parts.at(0) == "hash" && parts.length() > 1)
        {
            return messageToSha1(parts.at(1));
        }
        else if (parts.at(0) == "music" && parts.length() > 0)
        {
            return messageInMusic();
        }
        else if (parts.at(0) == "newton" && parts.length() > 0)
        {
            QByteArray ret;
            ret.setNum(rootByNewton());
            return ret;
        } else {
            return QByteArray();
        }
}
