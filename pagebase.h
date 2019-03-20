#ifndef PAGEBAGE_H
#define PAGEBAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>

#include "verticalscrollarea.h"

class PageBase : public QMainWindow
{
    Q_OBJECT
public:
    explicit PageBase(QWidget *parent = nullptr);

signals:

private slots:
    void virtual on_backButton_clicked() = 0;
    void virtual on_cancelButton_clicked() = 0;

private:
    QHBoxLayout *baseLayout;  // for button container
    QHBoxLayout *HLayout;  // for button container
    QVBoxLayout *VLayout;  // for item container
    QPushButton *backButton;
    QPushButton *cancelButton;
    VerticalScrollArea *scroll;
};

#endif // PAGEBAGE_H
