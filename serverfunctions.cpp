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

QByteArray messageToSha1(QString message) {
    qDebug() << "it`s funcition for transform message in sha1 hash\n";
    QByteArray byteArray = message.toUtf8();

    QByteArray hash = QCryptographicHash::hash(byteArray, QCryptographicHash::Sha1);

    QByteArray result;
    result.append("SHA-1 hash (hex): " + hash.toHex() + "\r\n");
    result.append("Hashing message: " + message.toUtf8() + "\r\n");

    return result;
}

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

//декодирование сообщения из музыкального файла
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
        else if (parts.at(0) == "music" && parts.length() > 2)
        {
            return messageInMusic(parts.at(1), parts.at(2));
        }
        //декодирование сообщения из музыкального файла
        else if (parts.at(0) == "extract" && parts.length() > 1) {
            return extractMessageFromMusic(parts.at(1)).toUtf8();
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
