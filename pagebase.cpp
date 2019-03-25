#include "pagebase.h"

PageBase::PageBase(int pageId, QWidget *parent):
    baseLayout(new QVBoxLayout(this)),
    backButton(new QPushButton("back")),
    cancelButton(new QPushButton("cansel"))
{
    this->setPageId(pageId);
    this->setTitle(pageId);
    qInfo() << pageTitle;
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
    db.setDatabaseName("./testdatabase.sqlite3");
    db.open();
    qDebug() << "tables 1";
    for (int i = 0; i < db.tables().count(); i ++) {
        qDebug() << db.tables().at(i);
    }

    QSqlQuery query(db);
    query.exec("create table test1(id, name, memo)");
    qDebug() << "tables 2";
    for (int i = 0; i < db.tables().count(); i ++) {
        qDebug() << db.tables().at(i);
    }

    query.prepare("insert into test1 (id, name, memo) "
                      "values (?, ?, ?)");
    for (int i = 0; i < 20; i ++) {
        query.bindValue(0, i);
        query.bindValue(1, QString("name%1").arg(i));
        query.bindValue(2, QString("memo%1").arg(i));
        query.exec();
    }

    qDebug() << "records";
    query.exec("select * from test1");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        QString memo = query.value(2).toString();
        qDebug() << QString("id(%1),name(%2),memo(%3)").arg(id).arg(name).arg(memo);
    }

     db.close();

//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//    db.setDatabaseName("./DataBase/ConsumablesData.db");
//    db.open();

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
