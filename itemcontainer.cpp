#include "itemcontainer.h"

ItemContainer::ItemContainer(QString _name, int _id, int _status, QString _pageName, QString _url, QWidget *parent):
    name(_name),
    id(_id),  // TODO:why ??
    status(_status)
{
    url = _url;
    pageName = _pageName;
    this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
    this->setMinimumSize(0,verticalHeght);
    this->setFrameShape(Panel);
    VLayout = new QVBoxLayout(this);

    itemTitle = new QLabel(name);
    itemImg = new QLabel(name);
    itemTitle->setAlignment(Qt::AlignCenter);
    itemTitle->setFixedHeight(15);
    itemImg->setAlignment(Qt::AlignCenter);
    QPixmap pix("://DataBase/img/" + name + ".jpg");
    itemImg->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio));
    orderButton = new QPushButton("order");
    orderButton->setFixedHeight(40);
    VLayout->addWidget(itemTitle);
    VLayout->addWidget(itemImg);
    VLayout->addWidget(orderButton);

    msgBox.setText(tr("Are you sure to order this item?"));
    msgBox.setWindowTitle(tr("Confirmation"));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);

    // This may be related to some bug. it seems change object name.
    // This is for reference to change stype.
    this->setObjectName("SendContainerFrame");

    connect(orderButton, SIGNAL(clicked()), this, SLOT(on_orderButton_clicked()));
//    connect(orderButton, SIGNAL(clicked()), this, SLOT(on_orderBtn_clicked()));
    this->setContainerColor();
    this->changeButtonState();
    this->changeButtonText();
}

int ItemContainer::verticalHeght = 240;

ItemContainer::~ItemContainer(){}

void ItemContainer::on_orderButton_clicked(){
    this->emitSignal();
//    int res = msgBox.exec();
//    if(res == QMessageBox::Yes){
//        this->sendOrderedSignal();
//    }else{
//        return;
//    }
//    this->changeButtonState();  // set button disabled.
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
        this->setStyleSheet("#SendContainerFrame {background-color: #FFF6F7;}");
        break;
    case 1:
        this->setStyleSheet("#SendContainerFrame {background-color: #FB94A0;}");
        break;
    case 2:
        this->setStyleSheet("#SendContainerFrame {background-color: #B0DEFB;}");
    }
}

void ItemContainer::changeButtonState(){
    if(status == 0) orderButton->setEnabled(true);
    else orderButton->setEnabled(false);
}

void ItemContainer::changeButtonText(){
    if(status == 0){
        orderButton->setText("Order !!");
    }else if(status == 1){
        orderButton->setText("Sent order request");
    }else if(status == 2){
        orderButton->setText("Under Delivery");
    }
}

void ItemContainer::sendOrderedSignal(){
    QUdpSocket sendPort;
    QByteArray datagram;
    QJsonObject jsonObj;
    jsonObj["pageName"] = this->pageName;
    jsonObj["id"] = this->id;
    jsonObj["name"] = this->name;
    jsonObj["URL"] = this->url;
    QJsonDocument jsonDoc(jsonObj);
    QByteArray ba = jsonDoc.toJson();
    sendPort.writeDatagram(ba.data(), QHostAddress::LocalHost, 5824);
}

void ItemContainer::updateStatus(){
    int tempStatus = DBManager::getState(this->pageName, this->name);
    if(tempStatus == 3){
        qWarning() << "Error on updating status ! check ItemContainer::updateSutatus";
    }else{
        this->status = tempStatus;
    }
    this->setContainerColor();  // change the color of this container
    this->changeButtonState();  // change the button state.
    this->changeButtonText();
}

void ItemContainer::emitSignal(){
    emit btnClicked(this->name, this->id, this->pageName, this->url);
    qInfo() << "order btn was clicked";
}
