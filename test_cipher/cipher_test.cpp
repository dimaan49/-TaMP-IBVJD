#include "cipher.hpp"
#include <QtTest>
#include <QDebug>

class VigenereCipherTest : public QObject
{
    Q_OBJECT

private slots:
    void testEncryptDecrypt();
    void testEmptyInput();
    void testEmptyKey();
    void testInvalidCharacters();
    void testCasePreservation();
};

void VigenereCipherTest::testEncryptDecrypt()
{
    const QString alphabet = "abcdefghijklmnopqrstuvwxyz";
    const QString input = "Hello, World!";
    const QString key = "key";

    QString encrypted = VigenereCipher::encrypt(alphabet, input, key);
    QVERIFY(!encrypted.isEmpty());
    
    QString decrypted = VigenereCipher::decrypt(alphabet, encrypted, key);
    QCOMPARE(decrypted, input);
}

void VigenereCipherTest::testEmptyInput()
{
    const QString alphabet = "abcdefghijklmnopqrstuvwxyz";
    const QString key = "key";
    
    QVERIFY(VigenereCipher::encrypt(alphabet, "", key).isEmpty());
    QVERIFY(VigenereCipher::decrypt(alphabet, "", key).isEmpty());
}

void VigenereCipherTest::testEmptyKey()
{
    const QString alphabet = "abcdefghijklmnopqrstuvwxyz";
    const QString input = "test";
    
    QVERIFY_EXCEPTION_THROWN(
        VigenereCipher::encrypt(alphabet, input, ""), 
        std::invalid_argument);
}

void VigenereCipherTest::testInvalidCharacters()
{
    const QString alphabet = "abc";
    const QString input = "hello";
    const QString key = "a";
    
    QVERIFY_EXCEPTION_THROWN(
        VigenereCipher::encrypt(alphabet, input, key),
        std::invalid_argument);
}

void VigenereCipherTest::testCasePreservation()
{
    const QString alphabet = "abcdefghijklmnopqrstuvwxyz";
    const QString input = "HeLlO";
    const QString key = "key";
    
    QString encrypted = VigenereCipher::encrypt(alphabet, input, key);
    QString decrypted = VigenereCipher::decrypt(alphabet, encrypted, key);
    
    QCOMPARE(decrypted, input);
}

QTEST_APPLESS_MAIN(VigenereCipherTest)
#include "cipher_test.moc"
