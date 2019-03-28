#ifndef PAGEBAGE_H
#define PAGEBAGE_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QLabel>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QStringList>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "itemcontainer.h"
#include "verticalscrollarea.h"

class PageBase : public QWidget
{
    Q_OBJECT

public:
    explicit PageBase(int, QWidget *parent = nullptr);
    ~PageBase();
    int getPageId();
    QString getPageName();
    void setPageId(int);

protected slots:
    void on_backButton_clicked();
    void on_cancelButton_clicked();

protected:
    QPushButton *backButton;
    QPushButton *cancelButton;
    QLabel *titleLabel;

private:
    QVBoxLayout *baseLayout;
    QHBoxLayout *buttonLayout;  // for button container
    QVBoxLayout *VLayout;  // for VerticalScrollArea
    QFrame* topContainer;
    int pageId;
    VerticalScrollArea *scroll;
    QVector<ItemContainer*> containarItem;
    ItemContainer *test;
    QStringList itemList;
    QStringList tableList;
    void createDB(QSqlDatabase);
    void setupItemContainer(QSqlDatabase);

    void setTitle(int id);
};

#endif // PAGEBAGE_H
