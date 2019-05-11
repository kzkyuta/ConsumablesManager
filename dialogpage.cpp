#include "dialogpage.h"

DialogPage::DialogPage(QWidget *parent):
    baseLayout(new QVBoxLayout(this)),
    approveBtn(new QPushButton("Yes")),
    cancelBtn(new QPushButton("cancel"))
{
    itemImg = new QLabel("aaa");
    confirmationText = new QLabel("this is test !!");
    baseLayout->addWidget(confirmationText);
    baseLayout->addWidget(approveBtn);
    baseLayout->addWidget(cancelBtn);

    connect(approveBtn, SIGNAL(clicked()), this, SLOT(on_approveBtn_Clicked()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(on_cancelBtn_Clicked()));
}

DialogPage::~DialogPage(){

}

void DialogPage::on_approveBtn_Clicked(){
    sendOrderSignal();
    qInfo() << "orderSignal was Sent";
}

void DialogPage::on_cancelBtn_Clicked(){
    qInfo() << "canceled";
}

void DialogPage::sendOrderSignal(){
    QUdpSocket sendPort;
    QByteArray datagram;
    QJsonObject jsonObj;
    jsonObj["pageName"] = this->itemPageName;
    jsonObj["id"] = this->itemID;
    jsonObj["name"] = this->itemName;
    jsonObj["URL"] = this->itemURL;
    QJsonDocument jsonDoc(jsonObj);
    QByteArray ba = jsonDoc.toJson();
    sendPort.writeDatagram(ba.data(), QHostAddress::LocalHost, 5824);
}

void DialogPage::setData(QString _name, int _id, QString _pageName, QString _url){
    itemName = _name;
    itemID = _id;
    itemPageName = _pageName;
    itemURL = _url;
    changeTaxt();
}

void DialogPage::changeTaxt(){
    confirmationText->setText(this->itemName + " " + QString::number(this->itemID) + " " + this->itemPageName + " " + this->itemURL);
}
