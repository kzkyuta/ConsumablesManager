#include "pagebase.h"

PageBase::PageBase(int pageId, QWidget *parent):
    baseLayout(new QVBoxLayout(this)),
    backButton(new QPushButton("back")),
    cancelButton(new QPushButton("cansel"))
{
    this->setPageId(pageId);
    this->setTitle(pageId);
    titleLabel = new QLabel();
    titleLabel->setText(pageTitle);
    titleLabel->setFont(QFont("Helvetica", 20));

    backButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    cancelButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

    topContainer = new QFrame();
    topContainer->setFixedHeight(60);

    buttonLayout = new QHBoxLayout(topContainer);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(titleLabel);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->setMargin(0);

    VLayout = new QVBoxLayout();
    scroll = new VerticalScrollArea(1,3, this);

    baseLayout->addWidget(topContainer);
    baseLayout->addLayout(VLayout);
    VLayout->addWidget(scroll);

    // database open
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("./consumableManage.sqlite3");
//    db.open();


    // table open
//    if(db.tables().count() == 0){ // if there is no table, create.
//        createDB(db);
//    }

    if(DBManager::countTableNum() == 0){
        DBManager::createTable(pageName);
    }

    for(int i = 0; i < DBManager::countTableNum(); i++){
        if(DBManager::getTableName(i) == pageName){
            break;
        }
        DBManager::createTable(pageName);
    }

    containarItem = DBManager::setupContainer(pageName);
    for(int i = 0; i < containarItem.count(); i++){
        scroll->addWidget(containarItem[i], i/3 , i%3);
    }

//    setupItemContainer(db);


//    db.close();
}

PageBase::~PageBase(){}

void PageBase::setTitle(int id){
    switch (id) {
    case 1: // office consumables
        pageTitle = "Office Consumables";
        pageName = "office";
        break;
    case 2:
        pageTitle = "Engineering Supplementation";
        pageName = "engineering";
        break;
    case 3:
        pageTitle = "Hygiene consumables";
        pageName = "hygiene";
        break;
    case 4:
        pageTitle = "Others";
        pageName = "others";
        break;
    }
}

int PageBase::getPageId(){
    return pageId;
}

void PageBase::setPageId(int id){
    if(id == 0){
        qWarning("you need to set id from 1");
    }else{
        pageId = id;
    }
}

QString PageBase::getPageName(){
    return pageName;
}

void PageBase::on_backButton_clicked(){
    qInfo("back");
}

void PageBase::on_cancelButton_clicked(){
    qInfo("cancel");
}

void PageBase::createDB(QSqlDatabase db){
    QFile openFile(":/DataBase/ConsubamleItems.json");
    openFile.open(QIODevice::ReadOnly);

    // input all data to QByteArray data
    QByteArray data = openFile.readAll();
    // read as json
    QJsonDocument jsonDoc(QJsonDocument::fromJson(data));
    QJsonObject jsonObj(jsonDoc.object());
    // get Array data. there will be if state
    QJsonArray jsonArr = jsonObj.value(pageName).toArray();

    QSqlQuery query(db);
    query.exec("create table " + pageName + "(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, name TEXT NOT NULL UNIQUE, "
                                   "status INTEGER NOT NULL, URL TEXT, archive INTEGER)");
    query.prepare("insert into office (id, name, status, URL, archive) values (?, ?, ?, ?, ?)");
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
    query.exec("select * from " + pageName);
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int status = query.value(2).toInt();
        QString URL = query.value(3).toString();
        int archive = query.value(4).toInt();
        qDebug() << QString("id(%1),name(%2),status(%3),URL(%4),archive(%5)").arg(id).arg(name).arg(status).arg(URL).arg(archive);
    }
}

void PageBase::setupItemContainer(QSqlDatabase db){
    QSqlQuery query(db);
    query.exec("select * from " +pageName);
    int i = 0;
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int status = query.value(2).toInt();
        QString URL = query.value(3).toString();
        int archive = query.value(4).toInt();
        containarItem.append(new ItemContainer(name,id, status));
        scroll->addWidget(containarItem.back(), i/3 , i%3);
        i ++;
        qDebug() << QString("id(%1),name(%2),status(%3),URL(%4),archive(%5)").arg(id).arg(name).arg(status).arg(URL).arg(archive);
    }
}
