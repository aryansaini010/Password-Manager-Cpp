#ifndef STORAGEMANAGER_H
#define STORAGEMANAGER_H

#include <QString>
#include <QList>
#include <QByteArray>
#include "Entry.h"

class StorageManager {
public:
    StorageManager(const QString& filePath);
    
    bool vaultExists() const;
    bool setupVault(const QString& masterPassword);
    bool unlockVault(const QString& masterPassword);
    
    QList<Entry> getEntries() const;
    void addEntry(const Entry& entry);
    void updateEntry(const Entry& entry);
    void deleteEntry(const QString& id);

private:
    QString m_filePath;
    QByteArray m_key;
    QByteArray m_salt;
    QList<Entry> m_entries;
    
    void save();
};

#endif
