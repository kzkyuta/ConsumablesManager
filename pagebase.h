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

#include "itemcontainer.h"
#include "verticalscrollarea.h"

class PageBase : public QWidget
{
    Q_OBJECT

public:
    explicit PageBase(int, QWidget *parent = nullptr);
    ~PageBase();
    int getPnageId();
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
    void readCsv(QStringList);

    void setTitle(int id);
    QString pageTitle;
};

#endif // PAGEBAGE_H
