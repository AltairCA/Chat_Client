#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#ifdef Q_OS_WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include "mstcpip.h"
#endif

#ifdef Q_OS_LINUX
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    requesting_url = "http://chat.altairsl.us/server/fun.php";
    //requesting_url = "http://127.0.0.1/chat/public_html/server/fun.php";
    installEventFilter(this);
    mytextedit = new QTextEdit;
    //ui->scrollArea->setWidget(mytextedit);
    socket = new QTcpSocket(this);

    socket->setSocketOption(QAbstractSocket::KeepAliveOption,true);

    icon_no=1;
    creatAction();
    creatTrayIcon();
    trayIcon->show();
    check_exit = true;


    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
   // connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
   // connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)),
            //this, SLOT(errorOccured(const QList<QSslError> &)));
    connect(this,SIGNAL(sendsignal(QString,QString,QString)),this,SLOT(send(QString,QString,QString)),Qt::QueuedConnection);

    splitstring = ",,,,";
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(QApplication::applicationDirPath()+"//msgs.db");
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(buttonenable()),Qt::QueuedConnection);
icon_timer = new QTimer(this);
icon_timer->start(1500);
connect(icon_timer,SIGNAL(timeout()),this,SLOT(iconchanger()));
    timer->start(2000);
     lengthof=0;
      lengthof_f = 0;
      lengthof_fa = 0;
     timer2 = new QTimer(this);
     connect(timer2,SIGNAL(timeout()),this,SLOT(task_10_timer()),Qt::QueuedConnection);

     timer2->start(5000);
    if(db->open()){
        /*
        QMessageBox msgBox;
         msgBox.setText("The document has been modified.");
         msgBox.exec();
*/
    }else{


    }
    logincheck = false;


    mDialog = new namelist(this);

    mDialog->db = db;
    mDialog->splitstring = splitstring;
    connect(mDialog,SIGNAL(returntomain(int,QString,QString)),this,SLOT(listboxreturn(int,QString,QString)),Qt::QueuedConnection);

    connect(this,SIGNAL(listboxgive(int,QString,QString)),mDialog,SLOT(takefrommain(int,QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(name_list_sender(QString)),mDialog,SLOT(takenamelist(QString)),Qt::QueuedConnection);
    connect(mDialog,SIGNAL(namelistreload()),this,SLOT(name_list_reload()),Qt::QueuedConnection);
    connect(mDialog,SIGNAL(friend_delete(QString)),this,SLOT(delete_friend(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(onliner(QString)),mDialog,SLOT(onlin(QString)),Qt::QueuedConnection);
    connect(mDialog,SIGNAL(get_online_list()),this,SLOT(onli()),Qt::QueuedConnection);
    connect(mDialog,SIGNAL(show_perant()),this,SLOT(show()));



    fDialog = new friendsearch(this);

    connect(fDialog,SIGNAL(search_keyout(QString)),this,SLOT(friend_serch_in(QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(friend_search_give(QString)),fDialog,SLOT(name_in(QString)),Qt::QueuedConnection);
    connect(fDialog,SIGNAL(friend_request(QString)),this,SLOT(friend_request(QString)),Qt::QueuedConnection);


    frDialog = new friendrequest(this);
    connect(frDialog,SIGNAL(search_requests()),this,SLOT(friend_requests_command()),Qt::QueuedConnection);
    connect(this,SIGNAL(friend_requsts_reciving(QString)),frDialog,SLOT(new_requests(QString)),Qt::QueuedConnection);
    connect(frDialog,SIGNAL(accept_request(QString)),this,SLOT(accept_request(QString)),Qt::QueuedConnection);
    connect(frDialog,SIGNAL(friend_delete(QString)),this,SLOT(delete_friend(QString)),Qt::QueuedConnection);
    ui->pushButton_2->hide();

    ui->pushButton_3->hide();
    ui->pushButton_5->hide();
    this->setWindowFlags ( Qt::CustomizeWindowHint | Qt::WindowTitleHint);



}
void MainWindow::iconchanger(){
    sendingss();
    this->setWindowTitle("Negombo General Hospital "+QTime::currentTime().toString());
    icon_no+=2;
    if(icon_no>=8){
        icon_no=1;
    }
    QIcon icon(":/new/prefix1/"+QString::number(icon_no)+".png");
    trayIcon->setIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::creatAction(){
    restoreAction = new QAction(tr("&Menu"),this);
    restoreAction->setIcon(QIcon(":/new/prefix1/restore.png"));
    restorefriendAction = new QAction(tr("&Friend List"),this);
    restorefriendAction->setIcon(QIcon(":/new/prefix1/Users.png"));
    connect(restorefriendAction,SIGNAL(triggered()),this,SLOT(show_friend_list()));
    connect(restoreAction,SIGNAL(triggered()),this,SLOT(showNormal()));
    quitAction = new QAction(tr("&Quit"),this);
    quitAction->setIcon(QIcon(":/new/prefix1/exit-icon-2013105.PNG"));
    hideAction = new QAction(tr("&Hide"),this);
    hideAction->setIcon(QIcon(":/new/prefix1/minimize-icon.PNG"));
    connect(hideAction,SIGNAL(triggered()),this,SLOT(hide()));
    connect(quitAction,SIGNAL(triggered()),this,SLOT(close()));

}
void MainWindow::TrayIconActivate(QSystemTrayIcon::ActivationReason reson){
    if(reson == QSystemTrayIcon::DoubleClick){
        this->showNormal();
    }
}

void MainWindow::creatTrayIcon(){
    trayMenu = new QMenu(this);
    trayMenu->setStyleSheet("QMenu{color:white;background-color:rgb(96,96,96);}");
    trayMenu->addAction(restorefriendAction);
    trayMenu->addAction(restoreAction);
    trayMenu->addSeparator();
    trayMenu->addAction(hideAction);
    trayMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/new/prefix1/1.png");
    trayIcon->setIcon(icon);
    trayIcon->setContextMenu(trayMenu);
     connect(trayIcon,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(TrayIconActivate(QSystemTrayIcon::ActivationReason)));
     trayIcon->setToolTip("Altair Chat - Offline");

}



void MainWindow::login(QString username, QString password){


     usrname = username;
     pass = password;
    QHostAddress HostAddress;
    HostAddress.setAddress("127.0.0.1");

    //socket->connectToHost(HostAddress,1234);
   // socket->setProtocol(QSsl::TlsV1SslV3);
    this->socket->setSocketOption(QAbstractSocket::KeepAliveOption,true);

    socket->connectToHost("chat.altairsl.us",1234);
   //socket->connectToHost("127.0.0.1",1234);



    if(!socket->waitForConnected(30000)){
        ui->pushButton->setEnabled(true);
        ui->label->setText("Failed to Connect");
    }

    /*if(!socket->waitForEncrypted(30000)){
        ui->pushButton->setEnabled(true);
        ui->label->setText("Failed to Connect");
    }
    */

    QString temp="0"+splitstring+username+splitstring+password+splitstring;

    socket->write(temp.toUtf8());


    if(!socket->waitForBytesWritten(30000)){
        ui->pushButton->setEnabled(true);
        ui->label->setText("Time out");
    }

}




void MainWindow::connected(){
    ui->label->setText("Connected to the server");
    logincheck = true;
    ui->pushButton->setText("Logout");
    namelister.clear();
    ui->pushButton_2->setEnabled(true);

    ui->pushButton_3->setEnabled(true);

    ui->pushButton_5->setEnabled(true);

    ui->pushButton_2->show();

    ui->pushButton_3->show();

    ui->pushButton_5->show();

    mDialog->namelister.clear();
    mDialog->namelister.append(namelister);

    mDialog->showNormal();
    this->hide();
    trayIcon->setToolTip("Altair Chat - Online");
     trayIcon->showMessage(tr("Altair Chat"), tr("Altair Chat is Connected to the Server"), QSystemTrayIcon::Information, 51051000);
    mDialog->connect_to_host(usrname);

#ifdef Q_OS_WIN32

    //windows
        DWORD         dwBytesRet=0;
        WSADATA       wsd;
        struct tcp_keepalive   alive;
        alive.onoff = TRUE;
        alive.keepalivetime = 10000;
        alive.keepaliveinterval = 5000;
        if (WSAIoctl(socket->socketDescriptor(), SIO_KEEPALIVE_VALS, &alive, sizeof(alive),
                     NULL, 0, &dwBytesRet, NULL, NULL) == SOCKET_ERROR)
        {
            //printf("WSAIotcl(SIO_KEEPALIVE_VALS) failed; %d\n");
            qDebug()<<"SIO KEEPALIVE VALS FAILS";


        }
    //end windows
#endif

#ifdef Q_OS_LINUX

        int enableKeepAlive = 1;
        int fd = socket->socketDescriptor();
        setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &enableKeepAlive, sizeof(enableKeepAlive));

        int maxIdle = 10; /* seconds */
        setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &maxIdle, sizeof(maxIdle));

        int count = 3;  // send up to 3 keepalive packets out, then disconnect if no response
        setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &count, sizeof(count));

        int interval = 2;   // send a keepalive packet out every 2 seconds (after the 5 second idle period)
        setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &interval, sizeof(interval));
#endif


}
void MainWindow::closeEvent(QCloseEvent *event){

        event->accept();

}
bool MainWindow::eventFilter(QObject *obj, QEvent *event){

    if (event->type() == QEvent::WindowStateChange) {
       if (!isHidden()) {

         this->showNormal();
         hide();



         event->ignore();

       } else {
         event->accept();

       }
     }else{
        return QObject::eventFilter(obj, event);

    }


}



void MainWindow::disconnected(){
    ui->pushButton->setEnabled(true);
    ui->pushButton->show();
    ui->pushButton_2->setEnabled(false);

    ui->pushButton_3->setEnabled(false);

    ui->pushButton_5->setEnabled(false);

    ui->label->setText("Disconnected");
    logincheck = false;
     ui->pushButton->setText("Login");
     mDialog->hide();

     ui->pushButton_2->hide();

     ui->pushButton_3->hide();
     ui->pushButton_5->hide();
     this->show();
     //QMessageBox::warning(this,tr("Disconnected"),tr("You have been disconnected from the server"));
     trayIcon->setToolTip("Altair Chat - Offline");
     trayIcon->showMessage(tr("Altair Chat"), tr("Altair Chat is Disconnected from the Server"), QSystemTrayIcon::Warning, 51051000);
    mDialog->newlogin = true;
    // mDialog->socket->reset();
      mDialog->socket->disconnectFromHost();
     this->socket->close();


}


void MainWindow::readyRead(){
    QString asd = socket->readAll();
    QStringList temp;
    try{
        temp = asd.split(splitstring);

    }catch(...){

    }
    for(int x=0;(temp.length()/3)>x;x++){
        //done
        if(temp[0+x*3] =="1"){
           // kl.append(temp[1+x*3]);
         //  kl.append(temp[2+x*3]);
           // task_new_2();
        }else if(temp[0+x*3]=="0"){


            task_new_3();



            mDialog->cname = usrname;
            onliner_send();
        }else if(temp[0+x*3]=="3"){
//done
                if(temp[1+x*3]==usrname){
                    // ui->label->setText(temp[2+x*3]);
                     if(!(namelister.contains(temp[2+x*3]))){
                         namelister.append(temp[2+x*3]);
                     }

                }else{
                     //ui->label->setText(temp[1+x*3]);
                     if(!(namelister.contains(temp[1+x*3]))){
                         namelister.append(temp[1+x*3]);
                     }


                }
                for(int x=0;namelister.length()>x;x++){
                    emit name_list_sender(namelister[x]);
                }

        }else if(temp[0+x*3]=="4"){
            //done
            emit friend_search_give(temp[1+x*3]);
        }else if(temp[0+x*3]=="6"){
            trayIcon->showMessage("Altair Chat", "You have New Friend Request", QSystemTrayIcon::Information, 5551000);
            ui->label->setText("You have New Friend Request");
            emit friend_requsts_reciving(temp[1+x*3]);
        }else if(temp[0+x*3]=="9"){
            emit onliner(temp[1+x*3]);
        }else if(temp[0+x*3]=="2"){
            //task_new_2();
        }
    }

}
void MainWindow::onli(){
    //request online users

    if(logincheck==true){
         onliner_send();
         task_new_6();
    }
}
void MainWindow::doDownload(QString link)
{
    manager = new QNetworkAccessManager(this);

    connect(manager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(replyFinished(QNetworkReply*)),Qt::QueuedConnection);
    manager->get(QNetworkRequest(QUrl("http://chat.altairsl.us/server/upload/"+link)));
   // manager->get(QNetworkRequest(QUrl("http://127.0.0.1/chat/public_html/server/upload/"+link)));

    // using the QNetworkAccsessManager object - manager,
    // connect the signals.
    // and this is the place that you give the url link for download
    // You can download whatever you want, zip, pdf, avi, mp3, basicly everything

}
void MainWindow::replyFinished (QNetworkReply *reply)
{
    if(reply->error())
    {
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else
    {

        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        QString currntpath = QApplication::applicationDirPath()+"//upload//";
        if(!QDir(currntpath).exists()){
            QDir().mkpath(currntpath);
        }
        for(int x=1;;x++){

            if(!QFile::exists(currntpath+QString::number(x)+"."+kl_f[lengthof_f])){

                QFile *file = new QFile(currntpath+QString::number(x)+"."+kl_f[lengthof_f]);
                if(file->open(QFile::Append))
                {
                    file->write(reply->readAll());
                    file->flush();
                    file->close();
                     emit listboxgive(1,"image_process:"+QString::number(x)+"."+kl_f[lengthof_f],kl_f[lengthof_f+1]);
                    QSqlQuery qry(*db);
                    QString query_1;
                   query_1 = "INSERT INTO `msg`(`me`, `from`, `msg`, `time`, `date`) VALUES ( '0','"+kl_f[lengthof_f+1]+"','"+"image_process:"+QString::number(x)+"."+kl_f[lengthof_f]+"','"+QTime::currentTime().toString(Qt::TextDate)+"','"+QDate::currentDate().toString(Qt::TextDate)+"')";
                    qry.exec(query_1);

                }
                delete file;
                break;

            }
        }
    }

    if(kl_f.length()==lengthof_f+2){
        kl_f.clear();
        lengthof_f=0;
    }else{
        lengthof_f+=2;
    }

    QString temp;
    temp = "10"+splitstring+kl_fa[lengthof_fa]+splitstring+"waat"+splitstring;
    socket->write(temp.toUtf8());
    if(!socket->waitForBytesWritten(30000)){
        ui->label->setText("Time out");
        trayIcon->showMessage("Altair Chat", "Disconnected form the Server", QSystemTrayIcon::Warning, 5651000);
    }

    if(kl_fa.length()==lengthof_fa+1){
        kl_fa.clear();
        lengthof_fa=0;
    }else{
        lengthof_fa++;
    }
    reply->deleteLater();

    // in function replyFinished first we check does everything work fine, if wanst it will display us Error as you can see
    // if everything is good, after download, QNetworkReply provide us nice functionality about downloaded file
    // reply->header(QNetworkRequest::ContentTypeHeader).toString(); give us the  name of file
    // reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();; // what date we downloaded it
    //reply->header(QNetworkRequest::ContentLengthHeader).toULongLong(); // gives us the number of BYTES

    // Now the program work fine but we need to know what type of file we download if we download a mp3 we must change
    //QFile *file = new QFile("your_file_name + extention"); the extention of our downloaded one.
    timer->start(2000);
}
void MainWindow::buttonenable(){


    if(kl.length()>0){


        QStringList asd = kl[lengthof].split(":");
        QStringList ta = asd[asd.length()-1].split(".");
         QString ext = ta[ta.length()-1];
        if(asd[0]=="image_process"){
            kl_f.append(ext);
           kl_fa.append(asd[1]);
            kl_f.append(kl[lengthof+1]);
                doDownload(asd[1]);
                timer->stop();

        }else{


        emit listboxgive(1,kl[lengthof],kl[lengthof+1]);

        QSqlQuery qry(*db);
        QString query_1;
        query_1 = "INSERT INTO `msg`(`me`, `from`, `msg`, `time`, `date`) VALUES ( '0','"+kl[lengthof+1]+"','"+kl[lengthof]+"','"+QTime::currentTime().toString(Qt::TextDate)+"','"+QDate::currentDate().toString(Qt::TextDate)+"')";
        qry.exec(query_1);
        }
        if(kl.length()==lengthof+2){
            kl.clear();
            lengthof=0;
        }else{
            lengthof+=2;

         }


    }

}

void MainWindow::on_pushButton_clicked()
{
    if(logincheck==false){
        login(ui->lineEdit_2->text(),ui->lineEdit->text());
    }else{
        //socket->reset();
        socket->disconnectFromHost();
        //socket->close();

    }
}




///******************************************************************************************************************************/


void MainWindow::listboxreturn(int method,QString msg,QString clientname){
    if(socket->isOpen()){
        QString temp;
        if(method==1){
           // temp="1"+splitstring+msg+splitstring+clientname+splitstring;
                task_new_1(msg,clientname);
           // emit sendsignal("1",msg,clientname);


        }
        if(method==2){
            temp="2"+splitstring+msg+splitstring+clientname+splitstring;
            emit sendsignal("2",msg,clientname);



        }if(method==10){
            temp="10"+splitstring+msg+splitstring+clientname+splitstring;
            emit sendsignal("10",msg,clientname);
        }

    }else{
        socket->close();
    }



}
void MainWindow::show_friend_list(){
    mDialog->namelister.clear();
    mDialog->namelister.append(namelister);

    mDialog->showNormal();
}

void MainWindow::on_pushButton_5_clicked()
{

    mDialog->namelister.clear();
    mDialog->namelister.append(namelister);

    mDialog->showNormal();
    this->hide();
    //mDialog->dos();
}




/*******************************************************************************************************************************/



void MainWindow::on_pushButton_2_clicked()
{
    fDialog->namelister.clear();
      fDialog->namelister.append(namelister);
    fDialog->showNormal();
}
void MainWindow::friend_serch_in(QString asd){
    //writebytes

    //emit sendsignal("4",asd,"waat");
    task_new_4(asd);
}
 void MainWindow::friend_request(QString name){

     //emit sendsignal("5",name,"waat");
    task_new_5(name);
 }

void MainWindow::on_pushButton_3_clicked()
{
    ui->label->clear();

    frDialog->showNormal();
}
void MainWindow::friend_requests_command(){

      //emit sendsignal("6","waat","waat");
    task_new_6();
}
 void MainWindow::accept_request(QString name){

     // emit sendsignal("7",name,"waat");
     task_new_7(name);
 }

 void MainWindow::name_list_reload(){

      //emit sendsignal("3","asd","asd");
     task_new_3();
    namelister.clear();
    onli();
 }

void MainWindow::on_pushButton_4_clicked()
{
    QString link = "http://chat.altairsl.us";
    //link = "http://127.0.0.1/chat/public_html/";
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::delete_friend(QString name){

     //emit sendsignal("8",name,"waat");
    task_new_8(name);
}
 void MainWindow::sendingss(){

     if(queue.isEmpty()){
         return;
     }

     if(socket->state() == QAbstractSocket::ConnectedState && socket->isOpen()){

         QString temp;
         temp  = queue.dequeue();
         socket->write(temp.toUtf8());
         socket->flush();

         if(!socket->waitForBytesWritten(30000)){

        }



     }else{


         QMessageBox::information(this,tr("Disconneted"),tr("you have been disconected from the server"));



         socket->close();
         ui->label->setText("Time out");

     }
 }

void MainWindow::send(QString x,QString a,QString b){



    if(socket->state() == QAbstractSocket::ConnectedState && socket->isOpen()){
        QString temp;
        temp = x+splitstring+a+splitstring+b+splitstring;
         queue.enqueue(temp);
        /*
        socket->write(temp.toUtf8());
        socket->flush();

        if(!socket->waitForBytesWritten(30000)){

       }

*/

    }else{


        QMessageBox::information(this,tr("Disconneted"),tr("you have been disconected from the server"));



        socket->close();
        ui->label->setText("Time out");

    }


}

void MainWindow::onliner_send(){
   //  emit sendsignal("9","waat","waat");
    task_new_9();
}

void MainWindow::on_pushButton_6_clicked()
{
    check_exit = false;
    this->close();

}

void MainWindow::on_pushButton_7_clicked()
{
    this->hide();
    trayIcon->showMessage("Altair Chat", "Altair Chat is runing background", QSystemTrayIcon::Information, 5551000);

}

 void MainWindow::task_new_1(QString y,QString z){
     if(logincheck){
        QNetworkRequest request;
        request.setUrl(requesting_url);

        request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
         QUrl params;
         QUrlQuery urlQuery;
            urlQuery.addQueryItem("username",usrname );
            urlQuery.addQueryItem("pass", pass);
            urlQuery.addQueryItem("option","1" );
            urlQuery.addQueryItem("to", z);
            urlQuery.addQueryItem("msg",y );

            params.setQuery(urlQuery);

            QByteArray data;
            data.append(params.toString());
            data.remove(0,1);

            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            reply_1 = manager->post(request, data);
            connect(reply_1, SIGNAL(finished()), this, SLOT(task_new_1_replyFinished()));

     }

 }
 void MainWindow::task_new_1_replyFinished(){
     QString data = reply_1->readAll().trimmed();
      qDebug() << data;
 }

 void MainWindow::task_new_2(){
     if(logincheck){
         QNetworkRequest request;
         request.setUrl(requesting_url);

         request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
         QUrl params;
         QUrlQuery urlQuery;
            urlQuery.addQueryItem("username",usrname );
            urlQuery.addQueryItem("pass", pass);
            urlQuery.addQueryItem("option","2" );

            params.setQuery(urlQuery);

            QByteArray data;
            data.append(params.toString());
            data.remove(0,1);

            QNetworkAccessManager *manager = new QNetworkAccessManager(this);
            reply_2 = manager->post(request, data);
            connect(reply_2, SIGNAL(finished()), this, SLOT(task_new_2_replyFinished()));

     }
 }
  void MainWindow::task_new_2_replyFinished(){
      QString data = reply_2->readAll().trimmed();
       qDebug() << data;

       QStringList temp;
       try{
           temp = data.split(splitstring);

       }catch(...){

       }
       for(int x=0;temp.length()/3>x;x++){
           kl.append(temp[x*3]);
           kl.append(temp[1+x*3]);
            //task_new_10(temp[2+x*3],temp[x*3]);
            queue_msg.enqueue(temp[2+x*3]);

            queue_msg_2.enqueue(temp[x*3]);
       }


  }
  void MainWindow::task_new_3(){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","3" );

             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_3 = manager->post(request, data);
             connect(reply_3, SIGNAL(finished()), this, SLOT(task_new_3_replyFinished()));

      }
  }

  void MainWindow::task_new_3_replyFinished(){
      QString data = reply_3->readAll().trimmed();
       qDebug() << data;
       QStringList temp;
       try{
           temp = data.split(splitstring);

       }catch(...){

       }
       for(int x=0;temp.length()-1>x;x++){
           if(temp[x]!=usrname){
               // ui->label->setText(temp[2+x*3]);
                if(!(namelister.contains(temp[x]))){
                    namelister.append(temp[x]);

                }
           }
       }

       for(int x=0;namelister.length()>x;x++){
           emit name_list_sender(namelister[x]);
       }
       reply_3->deleteLater();
       task_new_6();
  }


  void MainWindow::task_new_4(QString wor){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","4" );
             urlQuery.addQueryItem("word",wor );

             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_4 = manager->post(request, data);
             connect(reply_4, SIGNAL(finished()), this, SLOT(task_new_4_replyFinished()));

      }
  }

  void MainWindow::task_new_4_replyFinished(){
      QString data = reply_4->readAll().trimmed();
       qDebug() << data;
       QStringList temp;
       try{
           temp = data.split(splitstring);

       }catch(...){

       }
       for(int x=0;temp.length()-1>x;x++){
            emit friend_search_give(temp[x]);
       }

  }

  void MainWindow::task_new_5(QString to){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","5" );
             urlQuery.addQueryItem("to",to );

             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_5 = manager->post(request, data);
             connect(reply_5, SIGNAL(finished()), this, SLOT(task_new_5_replyFinished()));

      }
  }

  void MainWindow::task_new_5_replyFinished(){
      QString data = reply_5->readAll().trimmed();
       qDebug() << data;
  }

  void MainWindow::task_new_6(){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","6" );

             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_6 = manager->post(request, data);
             connect(reply_6, SIGNAL(finished()), this, SLOT(task_new_6_replyFinished()));

      }
  }

  void MainWindow::task_new_6_replyFinished(){
      QString data = reply_6->readAll().trimmed();
       qDebug() << data;
       QStringList temp;
       try{
           temp = data.split(splitstring);

       }catch(...){

       }
       for(int x=0;temp.length()-1>x;x++){
            emit friend_requsts_reciving(temp[x]);
           trayIcon->showMessage("Altair Chat", "You have New Friend Request", QSystemTrayIcon::Information, 5551000);
           ui->label->setText("You have New Friend Request");
       }
    reply_6->deleteLater();

  }

  void MainWindow::task_new_7(QString fname){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","7" );
             urlQuery.addQueryItem("to",fname );

             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_7 = manager->post(request, data);
             connect(reply_7, SIGNAL(finished()), this, SLOT(task_new_7_replyFinished()));

      }
  }

  void MainWindow::task_new_7_replyFinished(){
      QString data = reply_7->readAll().trimmed();
       qDebug() << data;
  }

  void MainWindow::task_new_8(QString fname){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","8" );
             urlQuery.addQueryItem("to",fname );

             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_8 = manager->post(request, data);
             connect(reply_8, SIGNAL(finished()), this, SLOT(task_new_8_replyFinished()));

      }
  }

  void MainWindow::task_new_8_replyFinished(){
      QString data = reply_8->readAll().trimmed();
       qDebug() << data;
  }

  void MainWindow::task_new_9(){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","9" );


             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_9 = manager->post(request, data);
             connect(reply_9, SIGNAL(finished()), this, SLOT(task_new_9_replyFinished()));

      }
  }

  void MainWindow::task_new_9_replyFinished(){
      QString data = reply_9->readAll().trimmed();
       qDebug() << data;



       QStringList temp;
       try{
           temp = data.split(splitstring);

       }catch(...){

       }
       for(int x=0;temp.length()>x;x++){
            emit onliner(temp[x]);
       }
  }


  void MainWindow::task_new_10(QString times,QString msg){
      if(logincheck){
          QNetworkRequest request;
          request.setUrl(requesting_url);

          request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
          QUrl params;
          QUrlQuery urlQuery;
             urlQuery.addQueryItem("username",usrname );
             urlQuery.addQueryItem("pass", pass);
             urlQuery.addQueryItem("option","10" );
             urlQuery.addQueryItem("time", times);
             urlQuery.addQueryItem("msg",msg );


             params.setQuery(urlQuery);

             QByteArray data;
             data.append(params.toString());
             data.remove(0,1);

             QNetworkAccessManager *manager = new QNetworkAccessManager(this);
             reply_10 = manager->post(request, data);
             connect(reply_10, SIGNAL(finished()), this, SLOT(task_new_10_replyFinished()));

      }
  }

  void MainWindow::task_new_10_replyFinished(){
      QString data = reply_10->readAll().trimmed();
       qDebug() << data;

       if(!queue_msg.isEmpty()){
           task_new_10(queue_msg.dequeue(),queue_msg_2.dequeue());
       }else{
            timer2->start(5000);
       }

  }

  void MainWindow::task_10_timer(){






        if(!queue_msg.isEmpty()){
            task_new_10(queue_msg.dequeue(),queue_msg_2.dequeue());
            timer2->stop();
        }else{
            task_new_2();
        }


  }
