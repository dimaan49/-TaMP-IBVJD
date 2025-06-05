#include <QtTest>
#include "sha1.hpp"

class Hash_Test : public QObject
{
    Q_OBJECT

private slots:
    void test_sha1_hashing();
};

void Hash_Test::test_sha1_hashing()
{
    // Тестируем функцию messageToSha1 из sha1.hpp
    QString input = "hello";
    QByteArray result = messageToSha1(input);

    // Проверяем, что результат содержит правильный хеш
    QVERIFY(result.contains("aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d"));

    // Проверяем, что результат содержит исходное сообщение
    QVERIFY(result.contains(input.toUtf8()));
}

QTEST_APPLESS_MAIN(Hash_Test)
#include "test_hash.moc"
