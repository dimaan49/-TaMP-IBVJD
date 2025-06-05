#ifndef CIPHER_HPP
#define CIPHER_HPP

#include <QString>
#include <QHash>
#include <stdexcept>

class VigenereCipher
{
public:
    static QString encrypt(const QString& alphabet, const QString& input, const QString& key) {
        return process(alphabet, input, key, true);
    }

    static QString decrypt(const QString& alphabet, const QString& input, const QString& key) {
        return process(alphabet, input, key, false);
    }

private:
    static QHash<QChar, int> createAlphabetHash(const QString& alphabet) {
        QHash<QChar, int> hash;
        for (int i = 0; i < alphabet.length(); ++i) {
            hash[alphabet.at(i)] = i;
        }
        return hash;
    }

    static QString process(const QString& alphabet, const QString& input,
                         const QString& key, bool encrypt) {
        if (alphabet.isEmpty() || key.isEmpty()) {
            throw std::invalid_argument("Alphabet and key must not be empty");
        }

        const auto alphabetHash = createAlphabetHash(alphabet);
        const int n = alphabet.length();
        QString output;
        output.reserve(input.length());

        int key_index = 0;

        for (const QChar& ch : input) {
            if (ch.isLetter()) {
                const QChar lowerCh = ch.toLower();
                const int p = alphabetHash.value(lowerCh, -1);
                if (p == -1) {
                    throw std::invalid_argument("Input contains characters not in alphabet");
                }

                const QChar key_char = key.at(key_index % key.length()).toLower();
                const int k = alphabetHash.value(key_char, -1);
                if (k == -1) {
                    throw std::invalid_argument("Key contains characters not in alphabet");
                }

                int result_pos;
                if (encrypt) {
                    result_pos = (p + k) % n;
                } else {
                    result_pos = (p - k + n) % n;
                }

                output.append(ch.isUpper() ? alphabet.at(result_pos).toUpper() 
                                          : alphabet.at(result_pos));
                key_index++;
            } else {
                output.append(ch);
            }
        }

        return output;
    }
};

#endif // CIPHER_HPP