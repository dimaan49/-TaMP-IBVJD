#ifndef RELTATION
#define RELATION
#include <QString>

struct User {
    User ();
    User (int, QString, QString, QString);
    int id;
    QString name;
    QString password;
    QString email;
};


#endif
