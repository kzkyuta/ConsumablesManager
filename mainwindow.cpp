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
    ui->stackedWidget->addWidget(officePage);
    ui->stackedWidget->addWidget(engineeringPage);
    ui->stackedWidget->addWidget(hygienePage);
    ui->stackedWidget->addWidget(othersPage);
    ui->stackedWidget->addWidget(dialog);

    receiveSocket = new QUdpSocket(this);
    bool result =  receiveSocket->bind(QHostAddress::AnyIPv4, 5826);
    qInfo() << "connection Result is" << result;

    connect(officePage->backButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(engineeringPage->backButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(hygienePage->backButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(othersPage->backButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(officePage->cancelButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(engineeringPage->cancelButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(hygienePage->cancelButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(othersPage->cancelButton, SIGNAL(clicked()), this, SLOT(backToInitPage()));
    connect(receiveSocket, SIGNAL(readyRead()), this, SLOT(receiveUDP()));
    for(int i = 0; i < officePage->containarItem.size(); i++){
        connect(officePage->containarItem[i], SIGNAL(btnClicked(QString, int, QString, QString)), this, SLOT(on_orderBtn_clicked(QString, int, QString, QString)));
    }
}

MainWindow::~MainWindow(){
    qInfo() << "close database";
    DBManager::closeDB();
    delete ui;
}

void MainWindow::backToInitPage(){
    ui->stackedWidget->setCurrentIndex(0);

}

void MainWindow::on_orderBtn_clicked(QString _pageName, int _id, QString _name, QString _url){
    qInfo() << "clicked orderBtn on" << _name;
    dialog->setData(_pageName, _id, _name, _url);
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
    int id = jsonobj.value(QString("callback_id")).toString().toInt();

    if(val == "done"){
        DBManager::changeState(pageName, id, 2);
    }else if(val == "received"){
        DBManager::changeState(pageName, id, 0);
    }else if(val == "ordered"){
        DBManager::changeState(pageName, id, 1);
    }
    this->updateContainars();
}

void MainWindow::updateContainars(){
    officePage->updateStatus();
    engineeringPage->updateStatus();
    othersPage->updateStatus();
    hygienePage->updateStatus();
}
