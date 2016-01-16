#ifndef MYTASK_H
#define MYTASK_H

#include <QRunnable>
#include <QDebug>
#include <QObject>
#include <QtSql>
#include <QDate>
#include<QTime>

#include<QString>
class mytask : public QObject, public QRunnable
{
    Q_OBJECT
public:
    mytask();
    QString msg;
    QString from;
    QString to;
    QString splitstring;
    QSqlDatabase *db;
     QByteArray bytes;

     QString  filepaths;
    public slots:

signals:
    void Result(int method,QString msg,QString from);
protected:
    void run();

};

#endif // MYTASK_H
