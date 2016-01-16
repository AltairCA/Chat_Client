#ifndef NAMELIST_H
#define NAMELIST_H

#include "QCloseEvent"
#include <QDialog>
#include <QDebug>
#include <QTcpSocket>
#include <QSslSocket>
#include <QAbstractSocket>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include "chatopen.h"
#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>
#include <QFile>
#include <QSslConfiguration>
#include <QHostAddress>
#include <QMessageBox>
#include <QtSql>
#include<QTime>
#include<QKeyEvent>
#include<QtNetwork>
#include "call.h"


#include <stdio.h>
#include <stdlib.h>
namespace Ui {
class namelist;
}

class namelist : public QDialog
{
    Q_OBJECT

public:
    explicit namelist(QWidget *parent = 0);
    QStringList namelister;
    QSqlDatabase *db;
    QStringList onliner;
    QString splitstring;
    QString cname;
    void dos();
    void connect_to_host(QString name);
    QTimer *timer4;
    QTcpSocket *socket;
    bool newlogin;
    void socket_re();

    ~namelist();

private:
    Ui::namelist *ui;
     ChatOpen *mDialog;
     Call *callgui;
     QTimer *timer;
     QTimer *timer3;

     QTimer *timer2;
     bool check;
     QStringList checkeeerr;
      QStringList openchat;

        int check_connectings;
       bool call_end;
       bool reconnect_check;



public slots:
    void listboxreturn(int method, QString msg, QString clientname);
    void takefrommain(int method,QString msg,QString clientname);
    void takenamelist(QString name);
    void onlin(QString name);
    void openchatboxlist(QString name);



    void connected();

    void disconnected();

    void bytesWritten(qint64 bytes);

    void readyRead();

    void from_call(QByteArray data);

    void call_out(QString name);
    void errorOccured(const QList<QSslError> &error);
    void disconnect_socket();
signals:
    void returntomain(int method,QString msg,QString clientname);
    void listboxgive(int method,QString msg,QString clientname);
    void namelistreload();
    void friend_delete(QString name);
    void get_online_list();
    void show_perant();
    void show_chat(QString name);
     void to_call(QByteArray data);
      void call_in(QString name);
      void exitcall();
private slots:
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_listWidget_clicked(const QModelIndex &index);
    void timers();
    void timers2();


    void on_pushButton_clicked();
    void on_listWidget_itemEntered(QListWidgetItem *item);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_toolButton_clicked();
     void reconnection();
protected:
    bool eventFilter(QObject *, QEvent *);
     void closeEvent(QCloseEvent *event);
};

#endif // NAMELIST_H
