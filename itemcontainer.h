#ifndef ITEMCONTAINER_H
#define ITEMCONTAINER_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QMessageBox>
#include <QDialogButtonBox>

#include "dbmanager.h"
#include "mydialog.h"

class ItemContainer : public QFrame
{
    Q_OBJECT

public:
    explicit ItemContainer(QString, int, int, QString _parentName, QWidget *parent = nullptr);
    ~ItemContainer();

    int getStatus();
    void setStatus(int);

    int getId();
    void setId(int);
    QString getName();
    static int verticalHeght;

protected:
    QPushButton *orderButton;

protected slots:
    void on_orderButton_clicked();

private:
    int status; // 0:stand by, 1:applied, 2:ordered
    int id;
    QString pageName;
    QString name;
    QVBoxLayout *VLayout;
    QLabel *itemTitle;
    QJsonArray tempArray;
    void setContainerColor();
    QMessageBox msgBox;
    MyDialog mMsgBox;
    QDialogButtonBox* msgButtonBox;
};

#endif // ITEMCONTAINER_H
