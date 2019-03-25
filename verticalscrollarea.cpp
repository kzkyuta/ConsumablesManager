#include "verticalscrollarea.h"

VerticalScrollArea::VerticalScrollArea(int rows, int cols, QWidget *parent = Q_NULLPTR):
    QScrollArea (parent),
    nRows(rows),
    nColumns(cols)
{
    this->setWidgetResizable(true);
    contentWidget = new QWidget(this);
    setWidget(contentWidget);
    parentGrid = new QVBoxLayout(contentWidget);
    childGrid = new QGridLayout();
    parentGrid->addLayout(childGrid);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

VerticalScrollArea::~VerticalScrollArea(){}

void VerticalScrollArea::addWidget(QWidget *w, int row, int col){
    childGrid->addWidget(w, row, col);
    adaptSize();
}

int VerticalScrollArea::rowCount() const{
    if(childGrid->count() == 0){
        return 0;
    }
    return childGrid->rowCount();
}

void VerticalScrollArea::adaptSize(){
    if(rowCount() > nRows){
        int vCorrected = ItemContainer::verticalHeght * rowCount() - 10*childGrid->verticalSpacing() *(rowCount() + 1);
        contentWidget->setFixedHeight(vCorrected);
    }
    contentWidget->setFixedWidth(viewport()->width());
}

void VerticalScrollArea::resizeEvent(QResizeEvent *event){
    QScrollArea::resizeEvent(event);
    adaptSize();
}
