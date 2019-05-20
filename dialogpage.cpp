#include "dialogpage.h"

DialogPage::DialogPage(QWidget *parent):
    baseLayout(new QVBoxLayout(this)),
    approveBtn(new QPushButton("Yes")),
    cancelBtn(new QPushButton("cancel"))
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    itemImg = new QLabel("aaa");
    itemImg->setAlignment(Qt::AlignCenter);
    confirmationText = new QLabel("this is test !!");
    QFont font = confirmationText->font();
    font.setPointSize(20);
    confirmationText->setFont(font);
    itemImg->setAlignment(Qt::AlignCenter);
    btnLayout = new QHBoxLayout();
    msgLayout = new QHBoxLayout();

    // set the btn design
    approveBtn->setFixedHeight(200);
    cancelBtn->setFixedHeight(200);
    QFont font_approveBtn = approveBtn->font();
    QFont font_cancelBtn = cancelBtn->font();
    font_approveBtn.setPointSize(50);
    font_cancelBtn.setPointSize(50);
    approveBtn->setFont(font_approveBtn);
    cancelBtn->setFont(font_cancelBtn);

    msgLayout->addWidget(itemImg);
    msgLayout->addWidget(confirmationText);

    btnLayout->addWidget(approveBtn);
    btnLayout->addWidget(cancelBtn);
    baseLayout->addLayout(msgLayout);
    baseLayout->addLayout(btnLayout);

    connect(approveBtn, SIGNAL(clicked()), this, SLOT(on_approveBtn_Clicked()));
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(on_cancelBtn_Clicked()));
}

DialogPage::~DialogPage(){

}

void DialogPage::on_approveBtn_Clicked(){
    this->sendOrderSignal();
    qInfo() << "orderSignal was Sent";
}

void DialogPage::on_cancelBtn_Clicked(){
    qInfo() << "canceled";
}

void DialogPage::sendOrderSignal(){
    QUdpSocket sendPort;
    QByteArray datagram;
    QJsonObject jsonObj;
    jsonObj["val"] = "ordered";
    jsonObj["pageName"] = this->itemPageName;
    jsonObj["id"] = this->itemID;
    jsonObj["name"] = this->itemName;
    jsonObj["URL"] = this->itemURL;
    jsonObj["timeStamp"] = "";
    jsonObj["slackChannel"] = "";
    QJsonDocument jsonDoc(jsonObj);
    QByteArray ba = jsonDoc.toJson();
    sendPort.writeDatagram(ba.data(), QHostAddress::LocalHost, 5824);
}

void DialogPage::setData(QString _name, int _id, QString _pageName, QString _url){
    itemName = _name;
    itemID = _id;
    itemPageName = _pageName;
    itemURL = _url;
    this->changeTaxt();
}

void DialogPage::changeTaxt(){
    QPixmap pix("://DataBase/img/" + this->itemName + ".jpg");
    itemImg->setPixmap(pix.scaled(120,120, Qt::KeepAspectRatio));
    confirmationText->setText("Are you sure to order " + this->itemName + " ?");
}
