#ifndef ITEMCONTAINER_H
#define ITEMCONTAINER_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QUdpSocket>
#include <QPixmap>

#include "dbmanager.h"
#include "mydialog.h"

class ItemContainer : public QFrame
{
    Q_OBJECT

public:
    explicit ItemContainer(QString, int, int, QString, QString, QWidget *parent = nullptr);
    ~ItemContainer();

    int getStatus();
    void setStatus(int);

    int getId();
    int getId(QString);
    void setId(int);
    QString getName();
    static int verticalHeght;
    void updateStatus(); // when udp signal received, this func will be called and update the status and display.

    QPushButton *orderButton;

protected:
signals:
    void btnClicked(QString _pageName, int _id, QString _name, QString _url);

public slots:
    void on_orderButton_clicked();
    void emitSignal();

private:
    int status; // 0:stand by, 1:applied, 2:ordered
    int id;
    QString pageName;
    QString name;
    QString url;
    QVBoxLayout *VLayout;
    QLabel *itemTitle;
    QLabel *itemImg;
    QJsonArray tempArray;
    void setContainerColor();
    QMessageBox msgBox;
    MyDialog mMsgBox;
    QDialogButtonBox* msgButtonBox;
    void sendOrderedSignal();
    void changeButtonState();
    void changeButtonText();
};

#endif // ITEMCONTAINER_H
