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
public:
    static QSqlDatabase db;
    static void setDB(QSqlDatabase);
    static void openDB();
    static void closeDB();
    static void createTable(QString);
    static void setupContainer(QVector<ItemContainer*>, QString);
    static QVector<ItemContainer*> setupContainer(QString);
    static int countTableNum();
};

#endif // DBMANAGER_H
