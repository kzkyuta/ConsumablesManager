#include "itemcontainer.h"

ItemContainer::ItemContainer(QString _name, int _id, int _status, QString _pageName, QWidget *parent):
    name(_name),
    id(_id),  // TODO:why ??
    status(_status)
{
    pageName = _pageName;
    this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    this->setMinimumSize(0,verticalHeght);
    this->setFrameShape(Panel);
    VLayout = new QVBoxLayout(this);

    itemTitle = new QLabel(name);
    itemTitle->setAlignment(Qt::AlignCenter);
    orderButton = new QPushButton("order");
    VLayout->addWidget(itemTitle);
    VLayout->addWidget(orderButton);

    // This may be related to some bug. it seems change object name.
    // This is for reference to change stype.
    this->setObjectName("SendContainerFrame");

    connect(orderButton, SIGNAL(clicked()), this, SLOT(on_orderButton_clicked()));
//    connect(orderButton, SIGNAL(clicked()), this, SLOT(on_orderBtn_clicked()));
    this->setContainerColor();
}

int ItemContainer::verticalHeght = 240;

ItemContainer::~ItemContainer(){}

void ItemContainer::on_orderButton_clicked(){
    int newStatus = this->getStatus()+1;
    qInfo() << this->getStatus();
    if(newStatus >= 3){
        setStatus(0);
        DBManager::changeState(this->pageName, this->getId(), 0);
//        DBManager::changeState(this->pageName, this->getName(), 0);
    }else{
        this->setStatus(newStatus);
        DBManager::changeState(this->pageName, this->getId(), newStatus);
//        DBManager::changeState(this->pageName, this->getName(), newStatus);
    }
    // after confirming sending to slack, change the color.
    this->setContainerColor();
}

int ItemContainer::getStatus(){
    return status;
}

void ItemContainer::setStatus(int _status){
    if(_status >= 0 && _status < 3){
        status = _status;
    }else{
        qWarning() << "this status is not suitable";
    }
}

int ItemContainer::getId(){
    return id;
}

void ItemContainer::setId(int _id){
    if(_id < 0){
        qWarning() << "this id is invalid";
    }else{
        id = _id;
    }
}

QString ItemContainer::getName(){
    return name;
}

void ItemContainer::setContainerColor(){
    int state = this->getStatus();
    switch (state) {
    case 0:
        this->setStyleSheet("#SendContainerFrame {background-color: darkgray;}");
        break;
    case 1:
        this->setStyleSheet("#SendContainerFrame {background-color: red;}");
        break;
    case 2:
        this->setStyleSheet("#SendContainerFrame {background-color: blue;}");
    }
}
