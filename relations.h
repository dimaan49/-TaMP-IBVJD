#ifndef RELTATION
#define RELATION
#include <QString>

struct user {
    int id;
    QString password;
    QString login;
    QString email;
};

struct admin{
    int id;
    QString password;
    QString login;
    QString email;
};

#endif
