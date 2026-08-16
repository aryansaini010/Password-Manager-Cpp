#ifndef ENTRY_H
#define ENTRY_H

#include <QString>
#include <QUuid>
#include <QJsonObject>

struct Entry {
    QString id;
    QString title;
    QString username;
    QString password;

    Entry() {
        id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    }

    QJsonObject toJson() const {
        QJsonObject obj;
        obj["id"] = id;
        obj["title"] = title;
        obj["username"] = username;
        obj["password"] = password;
        return obj;
    }

    static Entry fromJson(const QJsonObject& obj) {
        Entry e;
        e.id = obj["id"].toString();
        e.title = obj["title"].toString();
        e.username = obj["username"].toString();
        e.password = obj["password"].toString();
        return e;
    }
};

#endif // ENTRY_H
