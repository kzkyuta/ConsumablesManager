#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWizard>
#include <QStackedWidget>
#include <QIntegerForSize>
#include <QDebug>

#include "pagebase.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_button_1_clicked();

private:
    Ui::MainWindow *ui;
    PageBase *officePage;
    PageBase *engineeringPage;
    PageBase *hygienePage;
    PageBase *othersPage;
//    SecondPage *secondPage;
};

#endif // MAINWINDOW_H
