#ifndef DIALOGPAGE_H
#define DIALOGPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QDebug>
#include <QByteArray>
#include <QUdpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonDocument>

class DialogPage : public QWidget
{
    Q_OBJECT

public:
    explicit DialogPage(QWidget *parent = nullptr);
    ~DialogPage();

    QPushButton *approveBtn;
    QPushButton *cancelBtn;

    void setData(QString, int, QString, QString);

private slots:
    void on_approveBtn_Clicked();
    void on_cancelBtn_Clicked();

private:
    QLabel *itemImg;
    QLabel *confirmationText;
    QVBoxLayout *baseLayout;
    QString itemName;
    int itemID;
    QString itemPageName;
    QString itemURL;
    void sendOrderSignal();
    void changeTaxt();
};

#endif // DIALOGPAGE_H
