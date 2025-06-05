#ifndef DECODE_MUSIC_HPP
#define DECODE_MUSIC_HPP

#include <QString>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QStringList>


inline QByteArray messageInMusic(QString message, QString musicFilePath) {
    qDebug() << "Hiding message in music file:" << musicFilePath;

    QByteArray messageData = message.toUtf8();
    quint32 messageSize = messageData.size();

    QFile audioFile(musicFilePath);
    if (!audioFile.open(QIODevice::ReadWrite)) {
        return QByteArray("Error: Could not open audio file\r\n");
    }

    QByteArray audioData = audioFile.readAll();

    // WAV файл должен иметь минимум 44 байта заголовка
    if (audioData.size() < 44) {
        audioFile.close();
        return QByteArray("Error: Invalid WAV file format\r\n");
    }

    // Проверка достаточного размера файла
    quint32 maxMessageSize = (audioData.size() - 44) / 8;
    if (messageSize > maxMessageSize) {
        audioFile.close();
        return QByteArray("Error: Message too large for this audio file\r\n");
    }

    // Кодирование размера сообщения (32 бита)
    for (int i = 0; i < 32; i++) {
        quint32 pos = 44 + i;
        if (pos >= audioData.size()) break;
        audioData[pos] = (audioData.at(pos) & 0xFE) | ((messageSize >> (31 - i)) & 1);
    }

    // Кодирование самого сообщения
    for (quint32 i = 0; i < messageSize; i++) {
        for (int bit = 0; bit < 8; bit++) {
            quint32 pos = 44 + 32 + i * 8 + bit;
            if (pos >= audioData.size()) break;
            char byte = messageData.at(i);
            audioData[pos] = (audioData.at(pos) & 0xFE) | ((byte >> (7 - bit)) & 1);
        }
    }

    // Сохранение изменений
    audioFile.seek(0);
    audioFile.write(audioData);
    audioFile.close();

    return QByteArray("Message successfully hidden in audio file\r\n");
}

/**
 * @brief Извлекает скрытое сообщение из WAV-аудиофайла
 * @param musicFilePath Путь к WAV-файлу
 * @return Извлеченное сообщение или текст ошибки
 */
inline QString extractMessageFromMusic(QString musicFilePath) {
    QFile audioFile(musicFilePath);
    if (!audioFile.open(QIODevice::ReadOnly)) {
        return "Error: Could not open audio file";
    }

    QByteArray audioData = audioFile.readAll();
    audioFile.close();

    // Проверка минимального размера файла
    if (audioData.size() < 44 + 32) {
        return "Error: File too small to contain message";
    }

    // Извлечение размера сообщения (первые 32 бита после заголовка)
    quint32 messageSize = 0;
    for (int i = 0; i < 32; i++) {
        quint32 pos = 44 + i;
        if (pos >= audioData.size()) break;
        messageSize = (messageSize << 1) | (audioData.at(pos) & 1);
    }

    // Извлечение самого сообщения
    QByteArray messageData;
    for (quint32 i = 0; i < messageSize; i++) {
        char byte = 0;
        for (int bit = 0; bit < 8; bit++) {
            quint32 pos = 44 + 32 + i * 8 + bit;
            if (pos >= audioData.size()) break;
            byte = (byte << 1) | (audioData.at(pos) & 1);
        }
        messageData.append(byte);
    }

    return QString::fromUtf8(messageData);
}

#endif // DECODE_MUSIC_HPP
