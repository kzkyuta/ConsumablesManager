#include "dbmanager.h"

DBManager::DBManager()
{

}

QSqlDatabase DBManager::db = QSqlDatabase::addDatabase("QSQLITE");

void DBManager::setDB(QSqlDatabase _db){
    db = _db;
}

void DBManager::openDB(){
    db.setDatabaseName("./consumableManage.sqlite3");
    db.open();
}

void DBManager::closeDB(){
    db.close();
}

int DBManager::countTableNum(){
    return db.tables().count();
}

void DBManager::createTable(QString _pageName){
    QFile openFile(":/DataBase/ConsubamleItems.json");
    openFile.open(QIODevice::ReadOnly);

    // input all data to QByteArray data
    QByteArray data = openFile.readAll();
    // read as json
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
    QJsonObject jsonObj(jsonDoc.object());
    // get Array data. there will be if state
    QJsonArray jsonArr = jsonObj.value(_pageName).toArray();

    QSqlQuery query(db);
    query.exec("create table " + _pageName + "(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, name TEXT NOT NULL UNIQUE, "
                                   "status INTEGER NOT NULL, URL TEXT, archive INTEGER)");
    query.prepare("insert into " + _pageName +"(id, name, status, URL, archive) values (?, ?, ?, ?, ?)");
    for(int i = 0; i < jsonArr.size(); i++){
        QJsonObject temp = jsonArr[i].toObject();
        query.bindValue(0, i); // id
        query.bindValue(1, temp["name"].toString()); // name
        query.bindValue(2, 0); // status
        query.bindValue(3, temp["URL"].toString());  // URL
        query.bindValue(4, 0); // archive
        query.exec();
    }

    // output the datas
    qDebug() << "records";
    query.exec("select * from " + _pageName);
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int status = query.value(2).toInt();
        QString URL = query.value(3).toString();
        int archive = query.value(4).toInt();
        qDebug() << QString("id(%1),name(%2),status(%3),URL(%4),archive(%5)").arg(id).arg(name).arg(status).arg(URL).arg(archive);
    }
}

QJsonArray DBManager::setupContainer(QString _pageName){
    QSqlQuery query(db);
    QJsonObject jsonChild;
    QJsonArray jsonArr;
    query.exec("select * from " +_pageName);
    while (query.next()) {
        jsonChild["id"] = query.value(0).toInt();
        jsonChild["name"] = query.value(1).toString();
        jsonChild["status"] = query.value(2).toInt();
        jsonChild["URL"] = query.value(3).toString();
        jsonArr.append(jsonChild);
//        qDebug() << QString("id(%1),name(%2),status(%3),URL(%4),archive(%5)").arg(id).arg(name).arg(status).arg(URL).arg(archive);
    }
    return jsonArr;
}


QString DBManager::getTableName(int _i){
    return db.tables().at(_i);
}

void DBManager::changeState(QString _tableName, int _itemId,int _status){
    QSqlQuery query(db);
    QString a;
    query.exec("UPDATE " + _tableName + " SET status = " + QString::number(_status) + " WHERE id = " + QString::number(_itemId));
    a = "UPDATE " + _tableName + " SET status = " + QString::number(_status) + " WHERE id = " + QString::number(_itemId);
    qDebug() << a;
}

int DBManager::getState(QString _pageName, QString _itemName){
    int status = 3;
    QSqlQuery query(db);
    query.exec("select * from " +_pageName);
    while (query.next()){
        if(query.value(1).toString() == _itemName){
            status = query.value(2).toInt();
        }
    }
    return status;  // if this returns 3, it is error
}
