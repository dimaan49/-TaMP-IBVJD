#include <QtTest>
#include <QTemporaryFile>
#include "decode_music.hpp"

class MusicSteganographyTest : public QObject
{
    Q_OBJECT

private slots:
    void testHideAndExtractMessage();
    void testInvalidWavFile();
    void testMessageTooLarge();
    void testEmptyMessage();

private:
    void createTestWavFile(QTemporaryFile& file);
};

void MusicSteganographyTest::createTestWavFile(QTemporaryFile& file)
{
    file.open();

    // Создаем минимальный WAV заголовок (44 байта)
    QByteArray wavHeader(44, '\0');
    wavHeader.replace(0, 4, "RIFF");
    wavHeader.replace(8, 4, "WAVE");
    wavHeader.replace(36, 4, "data");

    // Добавляем аудиоданные (1 КБ)
    QByteArray audioData(1024, '\x7F');

    file.write(wavHeader + audioData);
    file.flush();
}

void MusicSteganographyTest::testHideAndExtractMessage()
{
    QTemporaryFile testFile;
    createTestWavFile(testFile);

    const QString testMessage = "Test secret message 123! @#$%^&*()";

    // Тестируем скрытие сообщения
    QByteArray hideResult = messageInMusic(testMessage, testFile.fileName());
    QVERIFY2(hideResult.contains("successfully"), "Message hiding failed");

    // Тестируем извлечение
    QString extracted = extractMessageFromMusic(testFile.fileName());
    QCOMPARE(extracted, testMessage);
}

void MusicSteganographyTest::testInvalidWavFile()
{
    QTemporaryFile invalidFile;
    invalidFile.open();
    invalidFile.write("NOT_A_WAV_FILE", 14);
    invalidFile.close();

    QByteArray result = messageInMusic("test", invalidFile.fileName());
    QVERIFY(result.contains("Invalid WAV file format"));

    QString extractResult = extractMessageFromMusic(invalidFile.fileName());
    QVERIFY(extractResult.contains("too small"));
}

void MusicSteganographyTest::testMessageTooLarge()
{
    QTemporaryFile testFile;
    createTestWavFile(testFile);

    // Создаем слишком большое сообщение
    QString largeMessage(2000, 'X');
    QByteArray result = messageInMusic(largeMessage, testFile.fileName());
    QVERIFY(result.contains("too large"));
}

void MusicSteganographyTest::testEmptyMessage()
{
    QTemporaryFile testFile;
    createTestWavFile(testFile);

    // Тест с пустым сообщением
    QByteArray result = messageInMusic("", testFile.fileName());
    QVERIFY(result.contains("successfully"));

    QString extracted = extractMessageFromMusic(testFile.fileName());
    QVERIFY(extracted.isEmpty());
}

QTEST_APPLESS_MAIN(MusicSteganographyTest)
#include "test_music.moc"
