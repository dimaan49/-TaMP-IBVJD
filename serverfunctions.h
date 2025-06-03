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

/**
 * @brief Просмотр статистики всех пользователей (только для админа)
 * @param adminName Имя администратора
 * @param adminPassword Пароль администратора
 * @return Статистика в виде QByteArray
 */
QByteArray lookallstat(QString adminName, QString adminPassword);

/**
 * @brief Шифрование текста методом Виженера
 * @param text Исходный текст
 * @param key Ключ шифрования
 * @return Зашифрованный текст в виде QByteArray
 */
QByteArray vigenereCipher(QString text, QString key);

QByteArray messageToSha1(QString);

QByteArray messageInMusic(QString, QString);

/**
 * @brief Шифрование текста
 * @param text Исходный текст
 * @param key Ключ шифрования
 * @return Зашифрованный текст
 */
QString Encrypt(QString text, QString key);

/**
 * @brief Расшифровка текста
 * @param text Зашифрованный текст
 * @param key Ключ шифрования
 * @return Расшифрованный текст
 */
QString Decrypt(QString text, QString key);

/**
 * @brief Структура для хранения уравнения
 */
struct Equation {
    double a;     ///< Коэффициент при x^2
    double b;     ///< Коэффициент при x
    double c;     ///< Свободный член
    
    /**
     * @brief Вычисляет значение функции в точке x
     * @param x Точка, в которой вычисляется значение
     * @return Значение функции
     */
    double evaluate(double x) const {
        return a * x * x + b * x + c;
    }
    
    /**
     * @brief Вычисляет значение производной в точке x
     * @param x Точка, в которой вычисляется производная
     * @return Значение производной
     */
    double derivative(double x) const {
        return 2 * a * x + b;
    }
};

/**
 * @brief Парсит строку уравнения в структуру
 * @param equation_str Строка с уравнением вида "ax^2 + bx + c = 0"
 * @return Структура уравнения
 */
Equation parseEquation(const QString& equation_str);

/**
 * @brief Находит корень уравнения методом Ньютона
 * @param eq Уравнение
 * @param x0 Начальное приближение
 * @param epsilon Точность
 * @param max_iter Максимальное число итераций
 * @return Найденный корень или NaN, если корень не найден
 */
double findRoot(const Equation& eq, double x0 = 0, double epsilon = 1e-10, int max_iter = 100);

/**
 * @brief Обновление данных пользователя (только для админа)
 * @param adminName Имя администратора
 * @param adminPassword Пароль администратора
 * @param targetUser Имя пользователя для обновления
 * @param newName Новое имя пользователя (пустое если не меняется)
 * @param newPassword Новый пароль (пустой если не меняется)
 * @param newEmail Новый email (пустой если не меняется)
 * @return Результат операции в виде QByteArray
 */
QByteArray updateUserData(QString adminName, QString adminPassword, QString targetUser,
                         QString newName, QString newPassword, QString newEmail);


#endif
