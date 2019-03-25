#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    officePage(new PageBase(1)),
    engineeringPage(new PageBase(2)),
    hygienePage(new PageBase(3)),
    othersPage(new PageBase(4))
{
    ui->setupUi(this);
    ui->stackedWidget->addWidget(officePage);
    ui->stackedWidget->addWidget(engineeringPage);
    ui->stackedWidget->addWidget(hygienePage);
    ui->stackedWidget->addWidget(othersPage);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::on_button_1_clicked(){
    qDebug() << officePage->getPnageId();
    ui->stackedWidget->setCurrentIndex(officePage->getPnageId());
}
