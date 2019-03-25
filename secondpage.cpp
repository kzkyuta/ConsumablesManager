#include "secondpage.h"

SecondPage::SecondPage(int id, QWidget *parent):
    PageBase (id, parent)
{
    connect(backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
}

SecondPage::~SecondPage(){}

void SecondPage::on_backButton_clicked(){
    qInfo("backButton");
}

void SecondPage::on_cancelButton_clicked(){
    qInfo("cancelButton");
}
