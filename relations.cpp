#include <relations.h>


User::User() {
    id = -1;
    name = "";
    password = "";
    email = "";
}
User::User (int id, QString name, QString password, QString email) {
    this->id = id;
    this->name = name;
    this->password = password;
    this->email = email;
}
