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

    if(DBManager::countTableNum() == 0){
        qInfo() << officePage->getPageName();
        DBManager::createTable(officePage->getPageName());
        DBManager::createTable(engineeringPage->getPageName());
        DBManager::createTable(hygienePage->getPageName());
        DBManager::createTable(othersPage->getPageName());
    }
}

MainWindow::~MainWindow(){
    qInfo() << "close database";
    DBManager::closeDB();
    delete ui;
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
