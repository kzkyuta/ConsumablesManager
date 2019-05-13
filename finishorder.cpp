#include "finishorder.h"

FinishOrder::FinishOrder(QWidget *parent) :
   QWidget(parent), baseLayout(new QHBoxLayout(this)), finishBtn(new QPushButton("Finish"))
{
    vLayout = new QVBoxLayout();

    finishBtn->setFixedHeight(200);
    text = new QLabel("Thank you for ordering !");
    QFont tempFont = text->font();
    tempFont.setPointSize(30);
    text->setFont(tempFont);
    tempFont = finishBtn->font();
    tempFont.setPointSize(30);
    finishBtn->setFont(tempFont);

    imgLabel = new QLabel("aaa");
    QPixmap pix("://DataBase/img/ojigi_boy.png");
    imgLabel->setPixmap(pix.scaled(350, 350, Qt::KeepAspectRatio));
    imgLabel->setAlignment(Qt::AlignCenter);
    vLayout->addWidget(text);
    vLayout->addWidget(finishBtn);
    baseLayout->addLayout(vLayout);
    baseLayout->addWidget(imgLabel);
}

FinishOrder::~FinishOrder(){

}
