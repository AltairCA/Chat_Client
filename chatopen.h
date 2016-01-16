#ifndef CHATOPEN_H
#define CHATOPEN_H

#include "QCloseEvent"
#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QTextEdit>
#include <QScroller>
#include <QTimer>
#include <QtSql>
#include <QFileDialog>
#include <QThreadPool>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QMessageBox>
#include "mytask1.h"
#include <QMenu>
#include <QTcpSocket>
#include <QSslSocket>
#include <QAbstractSocket>
#include <QtQuickWidgets/QQuickWidget>
#include <QQuickWidget>
namespace Ui {
class ChatOpen;
}

class ChatOpen : public QDialog
{
    Q_OBJECT

public:

    explicit ChatOpen(QWidget *parent = 0);
    Ui::ChatOpen *ui;
    QString name;
    ~ChatOpen();



public slots:
    void loader(QString name);
    void loadchatbox(QString name,QStringList list);
    void chatboxupdate(int method,QString msg,QString clientname);
     void chatboxupdate2(int method,QString msg,QString clientname);
    void setname(QString nam);
    void send(QString para1);
    void buttonenable2();
    void progressupdate(qint64 x,qint64 y);
signals:
    void listboxreturn(int method,QString msg,QString clientname);
    void chatboxname(QString name);
    void webviewin(QString msg,int x);
    void user_name(QString name);

private slots:
    void on_pushButton_clicked();
     void replyFinished();
    void on_textEdit_textChanged();
    void buttonenable();

    void on_pushButton_2_clicked();

    void on_webView_linkClicked(const QUrl &arg1);
void scroll_one();


void on_ChatOpen_destroyed();

void on_pushButton_3_clicked();
void webviewupdate(QString msg,int x);

void show_me(QString name);
void on_webView_customContextMenuRequested(const QPoint &pos);

void on_pushButton_4_clicked();

private:
QStringList filelist;
int z;
int filecount;
int replycount;
bool fileupload;
    QLabel *up;
    QLabel *down;
    QScrollerProperties sp;
    QTimer *timer;
     QTimer *one_timer;
    int wordlenth;

     bool shiftcheck;
     QStringList kl;
     QStringList imagelist;
     int lengthof;
     QString full_msg_in;
     QString full_msg_out;
     QString end_mark;
    int length_progress;
    QString  filepaths;
    QString currntpath;
    QNetworkReply *reply;

    QString size_of_font;
    //image attaching
    bool image_attach;
public:
    QSqlDatabase *db;
     QString htmlhead;
    QString headname;
    QString splitstring;
protected:
    bool eventFilter(QObject *, QEvent *);
    void closeEvent(QCloseEvent *event);
   // bool event(QEvent *);


};

#endif // CHATOPEN_H
