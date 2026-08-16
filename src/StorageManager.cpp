#include "StorageManager.h"
#include "CryptoUtils.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

StorageManager::StorageManager(const QString& filePath) : m_filePath(filePath) {}

bool StorageManager::vaultExists() const {
    return QFile::exists(m_filePath);
}

bool StorageManager::setupVault(const QString& masterPassword) {
    m_salt = CryptoUtils::generateRandomBytes(16);
    m_key = CryptoUtils::deriveKey(masterPassword, m_salt);
    m_entries.clear();
    save();
    return true;
}

bool StorageManager::unlockVault(const QString& masterPassword) {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) return false;
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (doc.isNull()) return false;
    
    QJsonObject vaultObj = doc.object();
    m_salt = QByteArray::fromBase64(vaultObj["salt"].toString().toLatin1());
    QByteArray encryptedData = QByteArray::fromBase64(vaultObj["data"].toString().toLatin1());
    
    QByteArray derivedKey = CryptoUtils::deriveKey(masterPassword, m_salt);
    
    bool ok;
    QByteArray decryptedData = CryptoUtils::decrypt(encryptedData, derivedKey, ok);
    if (!ok) return false; // Authentication failed
    
    m_key = derivedKey;
    
    QJsonDocument dataDoc = QJsonDocument::fromJson(decryptedData);
    QJsonArray entriesArr = dataDoc.object()["entries"].toArray();
    
    m_entries.clear();
    for (int i = 0; i < entriesArr.size(); ++i) {
        m_entries.append(Entry::fromJson(entriesArr[i].toObject()));
    }
    return true;
}

QList<Entry> StorageManager::getEntries() const {
    return m_entries;
}

void StorageManager::addEntry(const Entry& entry) {
    m_entries.append(entry);
    save();
}

void StorageManager::updateEntry(const Entry& entry) {
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].id == entry.id) {
            m_entries[i] = entry;
            save();
            return;
        }
    }
}

void StorageManager::deleteEntry(const QString& id) {
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].id == id) {
            m_entries.removeAt(i);
            save();
            return;
        }
    }
}

void StorageManager::save() {
    QJsonObject vaultObj;
    vaultObj["salt"] = QString::fromLatin1(m_salt.toBase64());
    
    QJsonArray entriesArr;
    for (const Entry& e : m_entries) {
        entriesArr.append(e.toJson());
    }
    
    QJsonObject dataObj;
    dataObj["entries"] = entriesArr;
    
    QByteArray encryptedData = CryptoUtils::encrypt(QJsonDocument(dataObj).toJson(QJsonDocument::Compact), m_key);
    vaultObj["data"] = QString::fromLatin1(encryptedData.toBase64());
    
    QFile file(m_filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(vaultObj).toJson(QJsonDocument::Indented));
    }
}
