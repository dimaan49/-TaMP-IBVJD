#ifndef RELATION
#define RELATION
#include <QString>

/**
 * @brief Структура, представляющая пользователя системы
 * 
 * Содержит основную информацию о пользователе: идентификатор,
 * имя, пароль и электронную почту
 */
struct User {
    /**
     * @brief Конструктор по умолчанию
     * 
     * Создает пользователя с пустыми полями и id = -1
     */
    User ();

    /**
     * @brief Конструктор с параметрами
     * @param id Идентификатор пользователя
     * @param name Имя пользователя
     * @param password Пароль пользователя
     * @param email Электронная почта пользователя
     */
    User (int id, QString name, QString password, QString email);

    int id;         ///< Идентификатор пользователя
    QString name;    ///< Имя пользователя
    QString password; ///< Пароль пользователя
    QString email;    ///< Электронная почта пользователя
};


#endif
