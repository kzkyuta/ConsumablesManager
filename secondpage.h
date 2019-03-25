#ifndef SECONDPAGE_H
#define SECONDPAGE_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

#include "pagebase.h"

class SecondPage : public PageBase
{
public:
    explicit SecondPage(int, QWidget *parent = nullptr);
    ~SecondPage();

private slots:
    void on_backButton_clicked();
    void on_cancelButton_clicked();

};

#endif // SECONDPAGE_H
