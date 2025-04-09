#ifndef serverh
#define serverh
#include <serverfunctions.h>

QByteArray queryAnalyzer(QString message) {
        QStringList parts = message.split("&");
        if (parts.at(0) == "auth" && parts.length() > 2)
        {
            return authentication(parts.at(1), parts.at(2));
        }
        else if (parts.contains("reg") && parts.length() > 2)
        {
            return registration(parts.at(1), parts.at(2), parts.at(3));
        }
        else if (parts.contains("lookall") && parts.length() > 2)
        {
            return lookallstat(parts.at(1), parts.at(2));
        }
        else if (parts.contains("vigener") && parts.length() == 2)
        {
            return vigenereCipher(parts.at(1));
        }
        else if (parts.contains("hash") && parts.length() == 2)
        {
            return messageToSha1(parts.at(1));
        }
        else if (parts.contains("music") && parts.length() > 0)
        {
            return messageInMusic();
        }
        else if (parts.contains("newtoon") && parts.length() > 0)
        {
            QByteArray ret;
            ret.setNum(rootByNewton());
            return ret;
        }

}
QByteArray authentication(QString a, QString b) {
    qDebug() << "it`s authentication funcition\n";
}
QByteArray registration(QString a, QString b, QString c) {
    qDebug() << "it`s registration funcition\n";
}
QByteArray lookallstat(QString a, QString b) {
    qDebug() << "it`s funcition for viewing all stat\n";
}
QByteArray vigenereCipher(QString a) {
    qDebug() << "it`s Vegener`s cipher function\n";
}
QByteArray messageToSha1(QString a) {
    qDebug() << "it`s funcition for transform message in sha1 hash\n";
}
QByteArray messageInMusic() {
    qDebug() << "it`s function for hide text message in the music file\n";
}
QString Encrypt(QString a, QString b , QString c) {
    qDebug() << "it`s ecnrypt funcition\n";
}
QString Decrypt(QString a, QString b, QString c) {
    qDebug() << "it`s decrypt funcition\n";
}
double rootByNewton() {
    return 1;
}
#endif
