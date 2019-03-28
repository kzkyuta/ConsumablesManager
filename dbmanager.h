#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

#include "itemcontainer.h"

class DBManager
{
private:
    DBManager();
    static QSqlDatabase db;
public:
    static void openDB();
    static void closeDB();
    static void createTable(QString);
    static void setupContainer(QVector<ItemContainer*>, QString);
    static int countTableNum();
};

#endif // DBMANAGER_H
