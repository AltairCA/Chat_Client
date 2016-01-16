#ifndef MYTASK1_H
#define MYTASK1_H

#include <QRunnable>
#include <QDebug>
#include <QObject>
#include<QNetworkReply>
#include<QNetworkAccessManager>
class MyTask1 : public QObject, public QRunnable
{
    Q_OBJECT
public:
    MyTask1();

    QNetworkReply *reply;
    QNetworkAccessManager *am;
    QByteArray bytes;
     QString ext;
      QByteArray data;
      QNetworkRequest request;
protected:
    void run();
};

#endif // MYTASK1_H
