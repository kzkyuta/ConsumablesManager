#ifndef FINISHORDER_H
#define FINISHORDER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPixmap>

class FinishOrder : public QWidget
{
    Q_OBJECT

public:
    explicit FinishOrder(QWidget *parent = nullptr);
    ~FinishOrder();
    QPushButton *finishBtn;

signals:

public slots:

private:
    QLabel *background;
    QLabel *text;
    QLabel *imgLabel;
    QHBoxLayout *baseLayout;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QPixmap pix;
};

#endif // FINISHORDER_H
