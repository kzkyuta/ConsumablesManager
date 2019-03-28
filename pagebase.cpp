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

    // for test
    int k = 0;
    for(int i = 0; i < 7; i ++){
        containarItem.append(new ItemContainer("item_" + QString::number(i), i, 0));
        if(i % 3 == 0 && i != 0) k ++;
        scroll->addWidget(containarItem.back(), k , i%3);  //
        qInfo() << k << ", " << i%3;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./consumableManage.sqlite3");
    db.open();

    QSqlQuery query(db);
    query.exec("create table test1(id, name, memo)");
    qDebug() << "tables 2";
    for (int i = 0; i < db.tables().count(); i ++) {
        qDebug() << db.tables().at(i);
    }

    // table open
    if(db.tables().count() == 0){ // if there is no table, create.
        createDB(db);
    }

    db.close();

    connect(backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
}

PageBase::~PageBase(){}

void PageBase::setTitle(int id){
    switch (id) {
    case 1: // office consumables
        pageTitle = "Office Consumables";
        break;
    case 2:
        pageTitle = "Engineering Supplementation";
        break;
    case 3:
        pageTitle = "Hygiene consumables";
        break;
    case 4:
        pageTitle = "Others";
        break;
    }
}

int PageBase::getPnageId(){
    return pageId;
}

void PageBase::setPageId(int id){
    if(id == 0){
        qWarning("you need to set id from 1");
    }else{
        pageId = id;
    }
}

void PageBase::on_backButton_clicked(){
    qInfo("back");
}

void PageBase::on_cancelButton_clicked(){
    qInfo("cancel");
}

void PageBase::readCsv(QStringList wordList){
    QFile file(":/DataBase/consumables.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        wordList.append(line.split(',')[1]);
    }
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
    QJsonArray jsonArr = jsonObj.value("office").toArray();

    QSqlQuery query(db);
    query.exec("create table office(id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, name TEXT NOT NULL UNIQUE, "
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
    query.exec("select * from office");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int status = query.value(2).toInt();
        QString URL = query.value(3).toString();
        int archive = query.value(4).toInt();
        qDebug() << QString("id(%1),name(%2),status(%3),URL(%4),archive(%5)").arg(id).arg(name).arg(status).arg(URL).arg(archive);
    }
}
