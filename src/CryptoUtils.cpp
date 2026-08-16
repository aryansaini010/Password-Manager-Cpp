#include "CryptoUtils.h"
#include <QMessageAuthenticationCode>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDataStream>
#include <QIODevice>

QByteArray CryptoUtils::hmacSha256(const QByteArray& key, const QByteArray& data) {
    QMessageAuthenticationCode mac(QCryptographicHash::Sha256);
    mac.setKey(key);
    mac.addData(data);
    return mac.result();
}

QByteArray CryptoUtils::deriveKey(const QString& password, const QByteArray& salt, int iterations, int keyLength) {
    QByteArray passwordBytes = password.toUtf8();
    QByteArray derivedKey;
    QByteArray t, u;

    int blockCount = (keyLength + 31) / 32;
    for (int i = 1; i <= blockCount; ++i) {
        QByteArray saltBlock = salt;
        QDataStream stream(&saltBlock, QIODevice::Append);
        stream << (quint32)i; // Append block number

        u = hmacSha256(passwordBytes, saltBlock);
        t = u;
        for (int j = 1; j < iterations; ++j) {
            u = hmacSha256(passwordBytes, u);
            for (int k = 0; k < t.size(); ++k) {
                t[k] = t[k] ^ u[k];
            }
        }
        derivedKey.append(t);
    }
    return derivedKey.left(keyLength);
}

QByteArray CryptoUtils::generateRandomBytes(int size) {
    QByteArray result;
    result.resize(size);
    for (int i = 0; i < size; ++i) {
        result[i] = static_cast<char>(QRandomGenerator::system()->generate() % 256);
    }
    return result;
}

QByteArray CryptoUtils::encrypt(const QByteArray& data, const QByteArray& key) {
    QByteArray iv = generateRandomBytes(16);
    
    QByteArray encrypted = data;
    for (int i = 0; i < encrypted.size(); ++i) {
        if (i % 32 == 0) {
            QByteArray blockData = iv + QByteArray::number(i / 32);
            QByteArray keystream = hmacSha256(key, blockData);
            for (int j = 0; j < 32 && i + j < encrypted.size(); ++j) {
                encrypted[i + j] = encrypted[i + j] ^ keystream[j];
            }
        }
    }
    
    QByteArray result = iv + encrypted;
    QByteArray mac = hmacSha256(key, result);
    return result + mac; // Append 32 bytes MAC
}

QByteArray CryptoUtils::decrypt(const QByteArray& dataWithMac, const QByteArray& key, bool& ok) {
    ok = false;
    if (dataWithMac.size() < 16 + 32) return QByteArray(); // Need at least IV + MAC
    
    int dataSize = dataWithMac.size() - 32;
    QByteArray payload = dataWithMac.left(dataSize);
    QByteArray receivedMac = dataWithMac.right(32);
    
    QByteArray expectedMac = hmacSha256(key, payload);
    if (expectedMac != receivedMac) {
        return QByteArray(); // Authentication failed
    }
    
    QByteArray iv = payload.left(16);
    QByteArray encrypted = payload.mid(16);
    QByteArray decrypted = encrypted;
    
    for (int i = 0; i < decrypted.size(); ++i) {
        if (i % 32 == 0) {
            QByteArray blockData = iv + QByteArray::number(i / 32);
            QByteArray keystream = hmacSha256(key, blockData);
            for (int j = 0; j < 32 && i + j < decrypted.size(); ++j) {
                decrypted[i + j] = decrypted[i + j] ^ keystream[j];
            }
        }
    }
    
    ok = true;
    return decrypted;
}
