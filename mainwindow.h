#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtQuickWidgets/QQuickWidget>
#include <QQuickView>
#include "QCloseEvent"
#include <QMainWindow>
#include <QDebug>
#include <QTcpSocket>
#include <QSslSocket>
#include <QAbstractSocket>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include "chatopen.h"
#include "namelist.h"
#include "friendsearch.h"
#include "friendrequest.h"
#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>
#include <QFile>
#include <QSslConfiguration>
#include <QHostAddress>
#include <QMessageBox>
#include <QtSql>
#include <QDate>
#include<QTime>
 #include <QDesktopServices>
 #include <QSystemTrayIcon>
#include <QMenu>
#include <QMenuBar>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QQueue>

class QAction;
class QMenu;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void login(QString username,QString password);

    void doDownload(QString link);

    //QTcpSocket *socket;
    QTcpSocket *socket;
    QLabel *up;
    QLabel *down;
public slots:
    void connected();
    void disconnected();

    void readyRead();
    void listboxreturn(int method, QString msg,QString clientname);

    void friend_serch_in(QString asd);
    void friend_request(QString name);
    void friend_requests_command();
    void accept_request(QString name);
    void name_list_reload();
    void delete_friend(QString lol);
    void send(QString, QString, QString);
    void onliner_send();
    void replyFinished (QNetworkReply *reply);

    void task_new_1(QString y,QString z);
    void task_new_1_replyFinished();

    void task_new_2();
    void task_new_2_replyFinished();

    void task_new_3();
    void task_new_3_replyFinished();

    void task_new_4(QString wor);
    void task_new_4_replyFinished();

    void task_new_5(QString to);
    void task_new_5_replyFinished();

    void task_new_6();
    void task_new_6_replyFinished();

    void task_new_7(QString fname);
    void task_new_7_replyFinished();


    void task_new_8(QString fname);
    void task_new_8_replyFinished();

    void task_new_9();
    void task_new_9_replyFinished();

    void task_new_10(QString times,QString msg);
    void task_new_10_replyFinished();

    void task_10_timer();
signals:
    void listboxgive(int method,QString msg,QString clientname);
    void friend_search_give(QString lol);
    void friend_requsts_reciving(QString lol);
    void name_list_sender(QString lol);
    void onliner(QString name);
    void sendsignal(QString,QString,QString);
private slots:
    void on_pushButton_clicked();
    void sendingss();
    void buttonenable();
    void onli();

    void on_pushButton_5_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();


    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void TrayIconActivate(QSystemTrayIcon::ActivationReason reson);
      void iconchanger();
      void show_friend_list();



private:
      QString requesting_url;
    bool check_exit;
    QString usrname;
    QString pass;
    Ui::MainWindow *ui;
    QListWidget *mylist;
    QLabel *mylabel;
    QTextEdit *mytextedit;
   // ChatOpen *mDialog;
    namelist *mDialog;
    friendsearch *fDialog;
    friendrequest *frDialog;
    QString splitstring;
    QStringList namelister;
    QSqlDatabase *db;
    bool logincheck;
    QTimer *timer;
    QTimer *timer2;
    QTimer *icon_timer;
    int lengthof;
     QStringList kl;

     int lengthof_f;
     int lengthof_fa;
      QStringList kl_f;
       QStringList kl_fa;

     void creatAction();
     void creatTrayIcon();
     QAction *restoreAction;
     QAction *quitAction;
     QAction *hideAction;
     QAction *restorefriendAction;
     QSystemTrayIcon *trayIcon;
     QMenu *trayMenu;
     int icon_no;
 QNetworkAccessManager *manager;
 bool check_download;

QQueue<QString> queue;
QQueue<QString> queue_msg;
QQueue<QString> queue_msg_2;

QNetworkReply *reply_1;
QNetworkReply *reply_2;
QNetworkReply *reply_3;
QNetworkReply *reply_4;
QNetworkReply *reply_5;
QNetworkReply *reply_6;
QNetworkReply *reply_7;
QNetworkReply *reply_8;
QNetworkReply *reply_9;
QNetworkReply *reply_10;
protected:
     void closeEvent(QCloseEvent *event);
     bool eventFilter(QObject *obj, QEvent *event);


};

#endif // MAINWINDOW_H
