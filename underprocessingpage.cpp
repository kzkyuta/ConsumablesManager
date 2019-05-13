#include "underprocessingpage.h"

UnderProcessingPage::UnderProcessingPage(QWidget *parent) : QWidget(parent)
{
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(Pal);

    timer = new QTimer(this);
    timer->setInterval(5000);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_timeout_error()));
    processingLabel = new QLabel("processing");
    processingGif = new QMovie("://DataBase/img/processing.gif");
    baseLayout = new QVBoxLayout(this);
    processingLabel->setMovie(processingGif);
    processingLabel->setAlignment(Qt::AlignCenter);
    baseLayout->addWidget(processingLabel);
}

UnderProcessingPage::~UnderProcessingPage(){}

void UnderProcessingPage::playGif(){
    processingGif->start();
    timer->start();
}

void UnderProcessingPage::stopGif(){
    processingGif->stop();
    timer->stop();
}

void UnderProcessingPage::stopTimeout(){
    timer->stop();
}

void UnderProcessingPage::on_timeout_error(){
    processingGif = new QMovie("://DataBase/img/error.png");
    processingLabel->setMovie(processingGif);
    processingLabel->setFixedSize(700, 300);
    processingGif->start();
    qWarning() << "python Server did not respond.";
}
