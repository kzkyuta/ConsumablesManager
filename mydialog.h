#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QDebug>


class MyDialog : public QMessageBox
{
    Q_OBJECT

public:
    MyDialog();
private slots:
    void onAccept();
    void onReject();
private:
    QPushButton* yesButton;
    QPushButton* noButton;
    QDialogButtonBox* buttonBox;
};

#endif // MYDIALOG_H
