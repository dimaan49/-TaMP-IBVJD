#ifndef SERVERFUNCTIONS_H
#define SERVERFUNCTIONS_H

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <database.h>
#include <QCryptographicHash>

QByteArray queryAnalyzer(QString);
QByteArray authentication(QString, QString);
QByteArray registration(QString, QString, QString);
QByteArray lookallstat(QString, QString);
QByteArray vigenereCipher(QString);
QByteArray messageToSha1(QString);
QByteArray messageInMusic(QString, QString);
QString Encrypt(QString, QString);
QString Decrypt(QString, QString, QString);
double rootByNewton();

#endif
