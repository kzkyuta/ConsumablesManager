#ifndef VERTICALSCROLLAREA_H
#define VERTICALSCROLLAREA_H

#include <QScrollArea>
#include <QGridLayout>
#include <QScrollBar>

#include "itemcontainer.h"

class VerticalScrollArea: public QScrollArea
{
    Q_OBJECT

public:
    explicit VerticalScrollArea(int, int, QWidget*);
    ~VerticalScrollArea();
    void addWidget(QWidget*, int, int);
    int rowCount() const;
private:
    QWidget *contentWidget;
    QVBoxLayout *parentGrid;
    QGridLayout *childGrid;
    int nRows; // for default size of conentWidget. it will used by adaptSize()
    int nColumns;  // for default size of conentWidget. it will used by adaptSize()
    void adaptSize();
protected:
    void resizeEvent(QResizeEvent*);
};

#endif // VERTICALSCROLLAREA_H
