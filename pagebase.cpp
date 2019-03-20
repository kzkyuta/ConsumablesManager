#include "pagebase.h"

PageBase::PageBase(QWidget *parent):
    baseLayout(new QHBoxLayout(this)),
    HLayout(new QHBoxLayout),
    VLayout(new QVBoxLayout),
    backButton(new QPushButton("back")),
    cancelButton(new QPushButton("cansel"))
{
    backButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    cancelButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    HLayout->addWidget(backButton);
    HLayout->addStretch();
    HLayout->addWidget(cancelButton);

    baseLayout->addLayout(HLayout);
    baseLayout->addLayout(VLayout);

    connect(backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(on_cancelButton_clicked()));
}
