#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWizard>
#include <QStackedWidget>
#include <QIntegerForSize>
#include <QDebug>
#include <QUdpSocket>

#include "dbmanager.h"
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
    void on_button_2_clicked();
    void on_button_3_clicked();
    void on_button_4_clicked();
    void backToInitPage();
    void receiveUDP();

private:
    Ui::MainWindow *ui;
    PageBase *officePage;
    PageBase *engineeringPage;
    PageBase *hygienePage;
    PageBase *othersPage;
    QSqlDatabase db;
    void updateContainars();

    QUdpSocket *receiveSocket;
};

#endif // MAINWINDOW_H
