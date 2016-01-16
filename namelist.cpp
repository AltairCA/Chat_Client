#include "namelist.h"
#include "ui_namelist.h"
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

namelist::namelist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::namelist)
{
    ui->setupUi(this);
    newlogin = true;
    this->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setFocusPolicy(Qt::StrongFocus);
   ui->listWidget->installEventFilter(this);
   timer = new QTimer(this);
   connect(timer,SIGNAL(timeout()),this,SLOT(timers()),Qt::QueuedConnection);
   timer->start(180000);
   timer2 = new QTimer(this);
   timer3= new QTimer(this);
   connect(timer2,SIGNAL(timeout()),this,SLOT(timers2()),Qt::QueuedConnection);
   connect(timer3,SIGNAL(timeout()),this,SLOT(reconnection()));
   timer2->start(8000);

    check = false;

    reconnect_check = false;
   socket_re();

    call_end = false;

}
void namelist::socket_re(){
    socket = new QTcpSocket(this);
    //socket->setProtocol(QSsl::TlsV1SslV3);
    socket->setSocketOption(QAbstractSocket::KeepAliveOption,true);

    connect(socket,SIGNAL(connected()),this,SLOT(connected()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));
   // connect(socket,SIGNAL(BytesWritten(qint64)),this,SLOT(bytesWritten(qint64)));
    //connect(socket, SIGNAL(sslErrors(const QList<QSslError> &)),
      //      this, SLOT(errorOccured(const QList<QSslError> &)));
}

void namelist::closeEvent(QCloseEvent *event){
    //emit show_perant();
    this->hide();
   event->ignore();
}
void namelist::dos(){
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    for(int x=0;namelister.length()>x;x++){
        QString asd = namelister[x];
        ui->listWidget->addItem(asd);
        ui->listWidget_2->addItem("Offline");
        /*
        QListWidgetItem *itm = new QListWidgetItem(tr("one"));
        itm->setIcon(QIcon(":/new/prefix1/Offline.png"));
        ui->listWidget_2->addItem(itm);
        */
        ui->listWidget_2->item(ui->listWidget_2->count()-1)->setIcon(QIcon(":/new/prefix1/Offline.png"));

    }
}

namelist::~namelist()
{
    delete ui;
}
void namelist::listboxreturn(int method, QString msg,QString clientname){
    emit returntomain(method,msg,clientname);
}

void namelist::takefrommain(int method,QString msg,QString clientname){
emit listboxgive(method,msg,clientname);
    if(!openchat.contains(clientname)){
        bool ch = false;
        for(int x=0;x<ui->listWidget->count();x++){
                if(ui->listWidget->item(x)->data(Qt::DisplayRole).toString()==clientname){
                    ch = true;
                    break;
                }
        }
        if(ch==false){
            return;
        }
        openchat.append(clientname);
        mDialog = new ChatOpen(this);
        mDialog->name = clientname;
        mDialog->setname(clientname);
        mDialog->db = db;
        mDialog->splitstring = splitstring;
        connect(mDialog,SIGNAL(listboxreturn(int,QString,QString)),this,SLOT(listboxreturn(int,QString,QString)),Qt::QueuedConnection);

        connect(this,SIGNAL(listboxgive(int,QString,QString)),mDialog,SLOT(chatboxupdate(int,QString,QString)),Qt::QueuedConnection);
        connect(mDialog,SIGNAL(chatboxname(QString)),this,SLOT(openchatboxlist(QString)));
        connect(this,SIGNAL(show_chat(QString)),mDialog,SLOT(show_me(QString)));
        connect(mDialog,SIGNAL(user_name(QString)),this,SLOT(call_out(QString)));
        mDialog->loader(clientname);
        mDialog->showNormal();
    }else{
        try{
            emit show_chat(clientname);
        }catch(...){

        }
    }

    //here database insert

}

void namelist::on_listWidget_doubleClicked(const QModelIndex &index)
{

    if(!openchat.contains(ui->listWidget->currentItem()->text())){
        openchat.append(ui->listWidget->currentItem()->text());
        mDialog = new ChatOpen(this);
        mDialog->name = ui->listWidget->currentItem()->text();
        mDialog->setname(ui->listWidget->currentItem()->text());
        mDialog->db = db;
        mDialog->splitstring = splitstring;
        connect(mDialog,SIGNAL(listboxreturn(int,QString,QString)),this,SLOT(listboxreturn(int,QString,QString)),Qt::QueuedConnection);

        connect(this,SIGNAL(listboxgive(int,QString,QString)),mDialog,SLOT(chatboxupdate(int,QString,QString)),Qt::QueuedConnection);
        connect(mDialog,SIGNAL(chatboxname(QString)),this,SLOT(openchatboxlist(QString)));
        connect(this,SIGNAL(show_chat(QString)),mDialog,SLOT(show_me(QString)));
        connect(mDialog,SIGNAL(user_name(QString)),this,SLOT(call_out(QString)));
        mDialog->loader(ui->listWidget->currentItem()->text());
        mDialog->showNormal();
    }else{
        emit show_chat(ui->listWidget->currentItem()->text());
    }





}

void namelist::openchatboxlist(QString name){
    qDebug()<<"removing ";
    if(openchat.contains(name)){
        openchat.removeOne(name);
    }else{

    }

}

void namelist::onlin(QString name){
    if(!(onliner.contains(name))){
        onliner.append(name);
    }



}

void namelist::on_listWidget_clicked(const QModelIndex &index)
{

}

void namelist::takenamelist(QString name){
    if(!(checkeeerr.contains(name))){
        ui->listWidget->addItem(name);
        ui->listWidget_2->addItem("Offline");
        ui->listWidget_2->item(ui->listWidget_2->count()-1)->setIcon(QIcon(":/new/prefix1/Offline.png"));
        checkeeerr.append(name);
    }

}

void namelist::on_pushButton_clicked()
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    checkeeerr.clear();
    emit namelistreload();
}
bool namelist::eventFilter(QObject *obj, QEvent *event)
 {
     if (event->type() == QEvent::KeyPress) {
         //and here put your own logic!!

         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);


        if(16777223==keyEvent->key()){
                if(ui->listWidget->isItemSelected(ui->listWidget->currentItem())){
                     //qDebug("Ate key press %d", keyEvent->key());
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::question(this, "Remove Friend", "Remove Friend "+ui->listWidget->currentItem()->text()+" ?",
                                                  QMessageBox::Yes|QMessageBox::No);
                    if (reply == QMessageBox::Yes) {
                        QString names = ui->listWidget->currentItem()->text();
                        emit friend_delete(names);
                        ui->pushButton->click();
                        ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));


                    }


                }

        }
     }
     return QObject::eventFilter(obj, event);

 }
void namelist::timers(){
    if(check==false){
        onliner.clear();
        emit get_online_list();
    }

}
void namelist::timers2(){

check = true;
        for(int x = 0;ui->listWidget->count()>x;x++){
                 //qDebug()<<ui->listWidget->item(x)->text();
                if(onliner.contains(ui->listWidget->item(x)->text())){
                    if(ui->listWidget_2->item(x)->text()=="Offline"){
                        ui->listWidget_2->takeItem(x);
                        ui->listWidget_2->insertItem(x,"Online");
                        ui->listWidget_2->item(x)->setIcon(QIcon(":/new/prefix1/messenger_013.png"));
                    }

                }else{
                    ui->listWidget_2->takeItem(x);
                    ui->listWidget_2->insertItem(x,"Offline");
                    ui->listWidget_2->item(x)->setIcon(QIcon(":/new/prefix1/Offline.png"));
                }

        }

check = false;
}

void namelist::on_listWidget_itemEntered(QListWidgetItem *item)
{

    try{
        //ui->listWidget_2->item(ui->listWidget->row(item))->setSelected(true);
    }catch(...){

    }


}

void namelist::on_listWidget_currentRowChanged(int currentRow)
{
    try{
         //ui->listWidget_2->item(currentRow)->setSelected(true);

    }catch(...){

    }


}

void namelist::on_toolButton_clicked()
{
    emit show_perant();
}


void namelist::connected(){







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

        QString temp = "1"+splitstring+cname+splitstring+"waaat"+splitstring;
        socket->write(temp.toUtf8());
        if(!socket->waitForBytesWritten(30000)){

        }
    check_connectings=0;
    this->socket->setSocketOption(QAbstractSocket::LowDelayOption,1);
}

void namelist::disconnected(){
    timer3->stop();
    call_end = false;
    emit exitcall();
    if(!newlogin){
        reconnect_check = false;
    timer3->start(2000);
    }
    socket->close();
}

void namelist::bytesWritten(qint64 bytes){

}

void namelist::readyRead(){
//emit to call
    if(call_end){
    QByteArray data;

    while (socket->bytesAvailable() > 0)
        data.append(qUncompress(socket->readAll()));
    //qDebug()<<"voice awa";
   emit to_call(data);
    }else{
        QStringList list;

        QByteArray temp=this->socket->readAll();
        qDebug()<<temp;

        try{
            list = QString(temp).split(this->splitstring);

        }catch(...){

        }



            for(int x=0;(list.length()/3)>x;x++){
                if(list[0+x*3]=="3"){
                    call_end=true;
                     callgui = new Call(this);
                    connect(this,SIGNAL(to_call(QByteArray)),callgui,SLOT(from_namelist(QByteArray)));
                    connect(callgui,SIGNAL(to_namelist(QByteArray)),this,SLOT(from_call(QByteArray)));
                    connect(callgui,SIGNAL(disconnect_socket()),this,SLOT(disconnect_socket()));
                    connect(this,SIGNAL(exitcall()),callgui,SLOT(exitcall()));
                    callgui->showNormal();
                    callgui->to_call(list[1+x*3]);

                }else if(list[0+x*3]=="4"){
                    call_end=false;
                    qDebug()<<"offline msg recieved";
                      emit exitcall();
                    //send notifcation to call gui inform about user is not online
                    if(this->isHidden()){
                        this->showNormal();
                        QMessageBox::information(this, "User Is not Available", "User Is not Available",QMessageBox::Ok);
                        this->hide();
                    }else{
                        QMessageBox::information(this, "User Is not Available", "User Is not Available",QMessageBox::Ok);
                    }

                }else if(list[0+x*3]=="5"){
                    if(call_end==false){
                       callgui = new Call(this);

                       connect(this,SIGNAL(to_call(QByteArray)),callgui,SLOT(from_namelist(QByteArray)));
                       connect(callgui,SIGNAL(to_namelist(QByteArray)),this,SLOT(from_call(QByteArray)));
                       connect(callgui,SIGNAL(disconnect_socket()),this,SLOT(disconnect_socket()));
                       connect(this,SIGNAL(exitcall()),callgui,SLOT(exitcall()));
                       callgui->showNormal();
                       callgui->from_call(list[1+x*3]);
                       call_end=true;
                    }
                    //send notifcation to call gui inform about user is not online
                }
            }
    }
}
void namelist::from_call(QByteArray data){
//from call
    if(call_end){

    socket->write(qCompress(data,5));
    socket->flush();
    }
 }
 void namelist::call_out(QString name){

     if(call_end==false){





       // socket->connectToHostEncrypted("127.0.0.1",1235);





         emit exitcall();

        socket->write(QString("2"+splitstring+name+splitstring+"waat"+splitstring).toUtf8());


        //call_end=true;
     }
 }

 void namelist::errorOccured(const QList<QSslError> &error)
 {
 // simply ignore the errors
 // it should be very careful when ignoring errors
     //this->socket->ignoreSslErrors();
 }
void namelist::disconnect_socket(){
    //delete  callgui;
    callgui->disconnect(this,SIGNAL(to_call(QByteArray)),callgui,SLOT(from_namelist(QByteArray)));
    callgui->disconnect(callgui,SIGNAL(to_namelist(QByteArray)),this,SLOT(from_call(QByteArray)));
    callgui->disconnect(callgui,SIGNAL(disconnect_socket()),this,SLOT(disconnect_socket()));
    callgui->disconnect(this,SIGNAL(exitcall()),callgui,SLOT(exitcall()));
    socket->disconnectFromHost();
   // socket->close();
   // while(socket->isOpen()){
   //      socket->close();
   // }
    //socket->connectToHostEncrypted("chat.altairsl.us",1235);
     //socket->connectToHostEncrypted("chat.altairsl.us",1235);
    //call_end = false;
    //timer3->start(5000);

}
 void namelist::connect_to_host(QString name){
     newlogin = true;
     //check_connectings=0;
     socket->disconnectFromHost();
      socket->close();
   // timer4->start(15000);
     cname = name;
    //socket->connectToHostEncrypted("chat.altairsl.us",1235);
    call_end = false;
    socket->connectToHost("chat.altairsl.us",1235);
    newlogin = false;
    if(!socket->waitForConnected(30000)){

    }



 }

 void namelist::reconnection(){
    if(reconnect_check==true){
        timer3->stop();
        socket->connectToHost("chat.altairsl.us",1235);
        if(!socket->waitForConnected(30000)){

        }


        QString temp = "1"+splitstring+cname+splitstring+"waaat"+splitstring;
        socket->write(temp.toUtf8());
        if(!socket->waitForBytesWritten(30000)){

        }
       // socket->connectToHostEncrypted("127.0.0.1",1235);

    }else{
        reconnect_check = true;
    }

 }

