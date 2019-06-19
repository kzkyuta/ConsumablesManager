#ifndef UNDERPROCESSINGPAGE_H
#define UNDERPROCESSINGPAGE_H

#include <QWidget>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QTimer>
#include <QDebug>

class UnderProcessingPage : public QWidget
{
    Q_OBJECT

public:
    explicit UnderProcessingPage(QWidget *parent = nullptr);
    ~UnderProcessingPage();

    void playGif();
    void stopGif();
    void stopTimeout();

signals:

public slots:
    void on_timeout_error();

private:
    QLabel *processingLabel;
    QMovie *processingGif;
    QVBoxLayout *baseLayout;
    QTimer *timer;
};

#endif // UNDERPROCESSINGPAGE_H
