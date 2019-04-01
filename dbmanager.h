#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

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
    static QJsonArray setupContainer(QString);
    static int countTableNum();
    static QString getTableName(int);
};

#endif // DBMANAGER_H
