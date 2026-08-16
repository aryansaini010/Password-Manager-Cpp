#ifndef CRYPTOUTILS_H
#define CRYPTOUTILS_H

#include <QString>
#include <QByteArray>

class CryptoUtils {
public:
    static QByteArray deriveKey(const QString& password, const QByteArray& salt, int iterations = 10000, int keyLength = 32);
    static QByteArray encrypt(const QByteArray& data, const QByteArray& key);
    static QByteArray decrypt(const QByteArray& dataWithMac, const QByteArray& key, bool& ok);
    static QByteArray generateRandomBytes(int size);

private:
    static QByteArray hmacSha256(const QByteArray& key, const QByteArray& data);
};

#endif // CRYPTOUTILS_H
