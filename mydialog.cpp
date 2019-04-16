#include "mydialog.h"

MyDialog::MyDialog():
    yesButton(new QPushButton("yes")),
    noButton(new QPushButton("no"))
{
//    this->setText("Are you sure to order this item ?");
    this->setText("???");
    this->defaultButton();
    buttonBox = new QDialogButtonBox(this);

    buttonBox->setMinimumHeight(100);
    buttonBox->addButton(yesButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(noButton, QDialogButtonBox::NoRole);

    connect(yesButton, SIGNAL(clicked()), this, SLOT(onAccept()));
    connect(noButton, SIGNAL(clicked()), this, SLOT(onAccept()));
}

void MyDialog::onAccept(){
    qInfo() << "accepted";
}

void MyDialog::onReject(){
    qInfo() << "rejected";
}

