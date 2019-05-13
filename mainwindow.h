#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWizard>
#include <QStackedWidget>
#include <QIntegerForSize>
#include <QDebug>
#include <QUdpSocket>

#include "finishorder.h"
#include "dbmanager.h"
#include "pagebase.h"
#include "dialogpage.h"
#include "underprocessingpage.h"

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
    void on_backToInitPage();
    void on_orderBtn_clicked(QString, int, QString, QString);
    void on_change_to_finished();
    void on_change_to_processing();
    void receiveUDP();

private:
    Ui::MainWindow *ui;
    PageBase *officePage;
    PageBase *engineeringPage;
    PageBase *hygienePage;
    PageBase *othersPage;
    DialogPage *dialog;
    FinishOrder *finishPage;
    UnderProcessingPage *processingPage;

    QSqlDatabase db;
    void updateContainars();

    QUdpSocket *receiveSocket;
};

#endif // MAINWINDOW_H
