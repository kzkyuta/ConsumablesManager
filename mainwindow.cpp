#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qInfo() << "open database";
    DBManager::openDB();
    ui->setupUi(this);

    officePage = new PageBase(1);
    engineeringPage = new PageBase(2);
    hygienePage = new PageBase(3);
    othersPage = new PageBase(4);
    ui->stackedWidget->addWidget(officePage);
    ui->stackedWidget->addWidget(engineeringPage);
    ui->stackedWidget->addWidget(hygienePage);
    ui->stackedWidget->addWidget(othersPage);

    receiveSocket = new QUdpSocket(this);
    bool result =  receiveSocket->bind(QHostAddress::AnyIPv4, 5825);
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
}

MainWindow::~MainWindow(){
    qInfo() << "close database";
    DBManager::closeDB();
    delete ui;
}

void MainWindow::backToInitPage(){
    qInfo() << "backToInitPage";
    ui->stackedWidget->setCurrentIndex(0);
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
    qInfo() << "received";
}
