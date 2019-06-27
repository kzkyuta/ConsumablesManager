#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qInfo() << "open database";
    DBManager::openDB();
    ui->setupUi(this);

    officePage = new PageBase(1, this);
    engineeringPage = new PageBase(2, this);
    hygienePage = new PageBase(3, this);
    othersPage = new PageBase(4, this);
    dialog = new DialogPage(this);
    finishPage = new FinishOrder(this);
    processingPage = new UnderProcessingPage(this);
    ui->stackedWidget->addWidget(officePage);
    ui->stackedWidget->addWidget(engineeringPage);
    ui->stackedWidget->addWidget(hygienePage);
    ui->stackedWidget->addWidget(othersPage);
    ui->stackedWidget->addWidget(dialog);
    ui->stackedWidget->addWidget(finishPage);  // 6
    ui->stackedWidget->addWidget(processingPage);  // 7

    receiveSocket = new QUdpSocket(this);
    bool result =  receiveSocket->bind(QHostAddress::AnyIPv4, 5826);
    qInfo() << "connection Result is" << result;

    connect(officePage->backButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    connect(officePage->cancelButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    for(int i = 0; i < officePage->containarItem.size(); i++){
        connect(officePage->containarItem[i], SIGNAL(btnClicked(QString, int, QString, QString)), this, SLOT(on_orderBtn_clicked(QString, int, QString, QString)));
    }
    connect(engineeringPage->backButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    connect(engineeringPage->cancelButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    for(int i = 0; i < engineeringPage->containarItem.size(); i++){
        connect(engineeringPage->containarItem[i], SIGNAL(btnClicked(QString, int, QString, QString)), this, SLOT(on_orderBtn_clicked(QString, int, QString, QString)));
    }
    connect(hygienePage->backButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    connect(hygienePage->cancelButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    for(int i = 0; i < hygienePage->containarItem.size(); i++){
        connect(hygienePage->containarItem[i], SIGNAL(btnClicked(QString, int, QString, QString)), this, SLOT(on_orderBtn_clicked(QString, int, QString, QString)));
    }
    connect(othersPage->backButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    connect(othersPage->cancelButton, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    for(int i = 0; i < othersPage->containarItem.size(); i++){
        connect(othersPage->containarItem[i], SIGNAL(btnClicked(QString, int, QString, QString)), this, SLOT(on_orderBtn_clicked(QString, int, QString, QString)));
    }
    connect(receiveSocket, SIGNAL(readyRead()), this, SLOT(receiveUDP()));
    connect(dialog->cancelBtn, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));
    connect(dialog->approveBtn, SIGNAL(clicked()), this, SLOT(on_change_to_processing()));
    connect(finishPage->finishBtn, SIGNAL(clicked()), this, SLOT(on_backToInitPage()));

    process.start("curl https://utailconsumablemanager_test.serveo.net/python/thread_start");  // it is needed to change based on the slack setting.
}

MainWindow::~MainWindow(){
    qInfo() << "close database";
    DBManager::closeDB();
    delete ui;
}

void MainWindow::on_backToInitPage(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_change_to_finished(){
    ui->stackedWidget->setCurrentIndex(6);
}

void MainWindow::on_change_to_processing(){
    processingPage->playGif();
    ui->stackedWidget->setCurrentIndex(7);
}

void MainWindow::on_orderBtn_clicked(QString _name, int _id, QString _pageName, QString _url){
    qInfo() << "clicked orderBtn on" << _name;
    dialog->setData(_name, _id, _pageName, _url);
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_button_1_clicked(){
    qDebug() << officePage->getPageId();
    ui->stackedWidget->setCurrentIndex(officePage->getPageId());
}

void MainWindow::on_button_2_clicked()
{
    qDebug() << engineeringPage->getPageId();
    ui->stackedWidget->setCurrentIndex(engineeringPage->getPageId());
}

void MainWindow::on_button_3_clicked()
{
    qDebug() << hygienePage->getPageId();
    ui->stackedWidget->setCurrentIndex(hygienePage->getPageId());
}

void MainWindow::on_button_4_clicked()
{
    qDebug() << othersPage->getPageId();
    ui->stackedWidget->setCurrentIndex(othersPage->getPageId());
}

void MainWindow::receiveUDP(){
    QByteArray datagram;
    datagram.resize(receiveSocket->pendingDatagramSize());
    receiveSocket->readDatagram(datagram.data(), datagram.size());
    QJsonDocument jsondoc = QJsonDocument::fromJson(datagram);
    QJsonObject jsonobj = jsondoc.object();
    QString val = jsonobj.value(QString("val")).toString();
    QString name = jsonobj.value(QString("name")).toString(); // id is needed
    QString pageName = jsonobj.value(QString("pageName")).toString();
    QString timeStamp = jsonobj.value(QString("timeStamp")).toString();
    QString slackChannel = jsonobj.value(QString("slackChannel")).toString();
    int id = jsonobj.value(QString("callback_id")).toString().toInt();

    qInfo() <<"received UDP val" << val;
    qInfo() <<"received UDP id" << id;
    qInfo() <<"received UDP name" << name;
    qInfo() <<"received UDP pageName" << pageName;
    qInfo() << "timeStamp" << timeStamp;

    if(val == "done"){
        DBManager::changeState(pageName, id, 2);
        sendConfirmSig(val, id, name, pageName, timeStamp, slackChannel);
    }else if(val == "received"){
        DBManager::changeState(pageName, id, 0);
        sendConfirmSig(val, id, name, pageName, timeStamp, slackChannel);
    }else if(val == "ordered"){
        DBManager::changeState(pageName, id, 1);
        ui->stackedWidget->setCurrentIndex(6);
        processingPage->stopGif();
    }
    this->updateContainars();
}

void MainWindow::updateContainars(){
    officePage->updateStatus();
    engineeringPage->updateStatus();
    othersPage->updateStatus();
    hygienePage->updateStatus();
}

void MainWindow::sendConfirmSig(QString _val, int _id, QString _name, QString _pageName, QString _timeStamp, QString _slackChannel){
    QUdpSocket sendPort;
    QByteArray datagram;
    QJsonObject jsonObj;
    jsonObj["val"] = _val;
    jsonObj["pageName"] = _pageName;
    jsonObj["id"] = _id;
    jsonObj["name"] = _name;
    jsonObj["timeStamp"] = _timeStamp;
    jsonObj["slackChannel"] = _slackChannel;
    QJsonDocument jsonDoc(jsonObj);
    QByteArray ba = jsonDoc.toJson();
    sendPort.writeDatagram(ba.data(), QHostAddress::LocalHost, 5824);
}
