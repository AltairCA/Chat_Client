#include "chatopen.h"
#include "mytask.h"
#include "ui_chatopen.h"

ChatOpen::ChatOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatOpen)
{
        QThreadPool::globalInstance()->setMaxThreadCount(10);
        this->setWindowTitle(name);
        ui->setupUi(this);
        size_of_font = "14";
       full_msg_in="";
        full_msg_out="";
        end_mark = "...()End()...";
        sp.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
        sp.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
        sp.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.5);
        sp.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
        sp.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.2);
        sp.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.2);
        sp.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity, 0);
        sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
        sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.5);
        QScroller* scroller = QScroller::scroller(this);
        //scroller->grabGesture(ui->webView, QScroller::LeftMouseButtonGesture);
        scroller->grabGesture(ui->textEdit, QScroller::LeftMouseButtonGesture);
        one_timer = new QTimer(this);
        connect(one_timer,SIGNAL(timeout()),this,SLOT(scroll_one()));
        scroller->setScrollerProperties(sp);
        timer = new QTimer(this);
        one_timer->start(5000);
        z=0;
        replycount = 0;
        fileupload=true;
        connect(timer,SIGNAL(timeout()),this,SLOT(buttonenable()),Qt::QueuedConnection);
        connect(this,SIGNAL(webviewin(QString,int)),this,SLOT(webviewupdate(QString,int)),Qt::QueuedConnection);
        timer->start(2000);
       // QString htmlhead = "<head><style>.container {padding: 40px 20px;margin: 0 auto;}.bubble {box-sizing: border-box;float: left;width: auto;max-width: 80%;position: relative;clear: both;background: #95c2fd;background-image: -webkit-gradient(linear, left bottom, left top, color-stop(0.15, #bee2ff), color-stop(1, #95c2fd));background-image: -webkit-linear-gradient(bottom, #bee2ff 15%, #95c2fd 100%);background-image: -moz-linear-gradient(bottom, #bee2ff 15%, #95c2fd 100%);background-image: -ms-linear-gradient(bottom, #bee2ff 15%, #95c2fd 100%);background-image: -o-linear-gradient(bottom, #bee2ff 15%, #95c2fd 100%);background-image: linear-gradient(bottom, #bee2ff 15%, #95c2fd 100%);filter: progid:DXImageTransform.Microsoft.gradient(GradientType=0,startColorstr='#95c2fd', endColorstr='#bee2ff');border: solid 1px rgba(0,0,0,0.5);-webkit-border-radius: 20px;-moz-border-radius: 20px;border-radius: 20px;-webkit-box-shadow: inset 0 8px 5px rgba(255,255,255,0.65), 0 1px 2px rgba(0,0,0,0.2);-moz-box-shadow: inset 0 8px 5px rgba(255,255,255,0.65), 0 1px 2px rgba(0,0,0,0.2);box-shadow: inset 0 8px 5px rgba(255,255,255,0.65), 0 1px 2px rgba(0,0,0,0.2);margin-bottom: 20px;padding: 6px 20px;color: #000;text-shadow: 0 1px 1px rgba(255,255,255,0.8);word-wrap: break-word;}.bubble:before, .bubble:after {border-radius: 20px / 5px;content: '';display: block;position: absolute;}.bubble:before {border: 10px solid transparent;border-bottom-color: rgba(0,0,0,0.5);bottom: 0px;left: -7px;z-index: -2;}.bubble:after {border: 8px solid transparent;border-bottom-color: #bee2ff;bottom: 1px;left: -5px;}.bubble-alt {float: right;}.bubble-alt:before {left: auto;right: -7px;}.bubble-alt:after {left: auto;right: -5px;}.bubble p {font-size: 1.4em;}.green {background: #7acd47;background-image: -webkit-gradient(linear,left bottom,left top,color-stop(0.15, #ace44b),color-stop(1, #7acd47));background-image: -webkit-linear-gradient(bottom, #ace44b 15%, #7acd47 100%);background-image: -moz-linear-gradient(bottom, #ace44b 15%, #7acd47 100%);background-image: -ms-linear-gradient(bottom, #ace44b 15%, #7acd47 100%);background-image: -o-linear-gradient(bottom, #ace44b 15%, #7acd47 100%);background-image: linear-gradient(bottom, #ace44b 15%, #7acd47 100%);filter: progid:DXImageTransform.Microsoft.gradient(GradientType=0,startColorstr='#7acd47', endColorstr='#ace44b');}.green:after {border-bottom-color: #ace44b;}</style></head><body><div class='container'>";
        //QString htmlhead = "<head><style>body {\n	background-color: #eee;\n	color: #222;\n	font: 0.8125em/1.5 'Helvetica Neue', Helvetica, Arial, sans-serif;\n}\n\nimg {\n	display: block;\n	height: auto;\n	max-width: 100%;\n}\n\n.container {\n	padding: 40px 20px;\n	margin: 0 auto;\n	width: 180px;\n}\n\n/* .bubble */\n\n.bubble {\n	background-image: linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -o-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -moz-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -webkit-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -ms-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -webkit-gradient(\n	linear,\n	left bottom,\n	left top,\n	color-stop(0.25, rgb(210,244,254)),\n	color-stop(1, rgb(149,194,253))\n);\n	border: solid 1px rgba(0, 0, 0, 0.5);\n	/* vendor rules */\n	border-radius: 20px;\n	/* vendor rules */\n	box-shadow: inset 0 5px 5px rgba(255, 255, 255, 0.4), 0 1px 3px rgba(0, 0, 0, 0.2);\n	/* vendor rules */\n	box-sizing: border-box;\n	clear: both;\n	float: left;\n	margin-bottom: 20px;\n	padding: 8px 30px;\n	position: relative;\n	text-shadow: 0 1px 1px rgba(255, 255, 255, 0.7);\n	width: auto;\n	max-width: 100%;\n	word-wrap: break-word;\n}\n\n.bubble:before, .bubble:after {\n	border-radius: 20px / 10px;\n	content: '';\n	display: block;\n	position: absolute;\n}\n\n.bubble:before {\n	border: 10px solid transparent;\n	border-bottom-color: rgba(0, 0, 0, 0.5);\n	bottom: 0;\n	left: -7px;\n	z-index: -2;\n}\n\n.bubble:after {\n	border: 8px solid transparent;\n	border-bottom-color: #d2f4fe;\n	bottom: 1px;\n	left: -5px;\n}\n\n.bubble--alt {\n	background-image: linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -o-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -moz-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -webkit-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -ms-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -webkit-gradient(\n	linear,\n	left bottom,\n	left top,\n	color-stop(0.25, rgb(172,228,75)),\n	color-stop(1, rgb(122,205,71))\n);\n	float: right;\n}\n\n.bubble--alt:before {\n	border-bottom-color: rgba(0, 0, 0, 0.5);\n	border-radius: 20px / 10px;\n	left: auto;\n	right: -7px;\n}\n\n.bubble--alt:after {\n	border-bottom-color: #ace44b;\n	border-radius: 20px / 10px;\n	left: auto;\n	right: -5px;\n}</style></head><body><div class='container'>";
        htmlhead = "<head><style>body {\n	background-color: #eee;\nbackground-image: url("+QUrl("qrc:/new/prefix1/back.png").toString()+");\n	color: #222;\n	font: "+size_of_font+"px 'Helvetica Neue', Helvetica, Arial, sans-serif;\n}\n\nimg {\n	display: block;\n	height: auto;\n	max-width: 100%;\n}\n\n.container {\n	padding: 40px 20px;\n	margin: 0 auto;\n	width: 180px;\n}\n\n/* .bubble */\n\n.bubble {\n \n	background-image: linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -o-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -moz-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -webkit-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -ms-linear-gradient(bottom, rgb(210,244,254) 25%, rgb(149,194,253) 100%);\nbackground-image: -webkit-gradient(\n	linear,\n	left bottom,\n	left top,\n	color-stop(0.25, rgb(210,244,254)),\n	color-stop(1, rgb(149,194,253))\n);\n	border: solid 1px rgba(0, 0, 0, 0.5);\n	/* vendor rules */\n	border-radius: 5px;\n	/* vendor rules */\n	box-shadow: inset 0 5px 5px rgba(255, 255, 255, 0.4), 0 1px 3px rgba(0, 0, 0, 0.2);\n	/* vendor rules */\n	box-sizing: border-box;\n	clear: both;\n	float: left;\n	margin-bottom: 20px;\n	padding: 0px 15px;\n	position: relative;\n	text-shadow: 0 1px 1px rgba(255, 255, 255, 0.7);\n	width: auto;\n	max-width: 100%;\n	word-wrap: break-word;\n}\n\n.bubble:before, .bubble:after {\n\n	border-radius: 20px / 10px;\n	content: '';\n	display: block;\n	position: absolute;\n}\n\n.bubble:before {\n\n	border: 10px solid transparent;\n	border-bottom-color: rgba(0, 0, 0, 0.5);\n	bottom: 0;\n	left: -7px;\n	z-index: -2;\n}\n\n.bubble:after {\n\n	border: 8px solid transparent;\n	border-bottom-color: #d2f4fe;\n	bottom: 1px;\n	left: -5px;\n}\n\n.bubble--alt {\n \n	background-image: linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -o-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -moz-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -webkit-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -ms-linear-gradient(bottom, rgb(172,228,75) 25%, rgb(122,205,71) 100%);\nbackground-image: -webkit-gradient(\n	linear,\n	left bottom,\n	left top,\n	color-stop(0.25, rgb(172,228,75)),\n	color-stop(1, rgb(122,205,71))\n);\n	float: right;\n}\n\n.bubble--alt:before {\n\n	border-bottom-color: rgba(0, 0, 0, 0.5);\n	border-radius: 20px / 10px;\n	left: auto;\n	right: -7px;\n}\n\n.bubble--alt:after {\n\n	border-bottom-color: #ace44b;\n	border-radius: 20px / 10px;\n	left: auto;\n	right: -5px;\n}</style></head><body><div class='container'>";
        //htmlhead = "<body style='background-image: url(back.png);'><div class='container'>";

        // ui->webView->setHtml(htmlhead);

image_attach = false;

       // ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
        wordlenth = 0;
         ui->label->setText(name);

          this->setFocusPolicy(Qt::NoFocus);
          ui->textEdit->setFocusPolicy(Qt::StrongFocus);
         ui->textEdit->installEventFilter(this);
         shiftcheck = false;
         lengthof=0;
         length_progress=0;
        currntpath = QApplication::applicationDirPath();



}

void ChatOpen::scroll_one(){
  //  ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());

        one_timer->stop();

}

void ChatOpen::setname(QString nam){
    ui->label->setText(nam);
    this->setWindowTitle(nam);
    //ui->quickWidget->setSource(QUrl("qrc:/webview.qml"));
}

ChatOpen::~ChatOpen()
{
    delete ui;
}

void ChatOpen::buttonenable(){
    if(fileupload){
         buttonenable2();
    }

    //ui->pushButton->setFlat(false);
   // Ui::ChatOpen.pushButton->hide();
   // Ui::ChatOpen.pushButton->repaint();
    if(kl.length()==0){
            ui->pushButton->setEnabled(true);
            ui->progressBar->setValue(0);
            ui->pushButton_3->setEnabled(true);
            ui->pushButton_3->setText("Send\nFiles");


    }

    if(kl.length()>0){
        ui->progressBar->setMaximum(kl.length());
        ui->progressBar->setValue(lengthof+1);
        emit listboxreturn(1,kl[lengthof],name);

        if(kl[lengthof].split(":")[0]!="image_process"){
            chatboxupdate(2,kl[lengthof],name);
            QSqlQuery qry(*db);
            QString query_1;
            query_1 = "INSERT INTO `msg`(`me`, `from`, `msg`, `time`, `date`) VALUES ( '1','"+name+"','"+kl[lengthof]+"','"+QTime::currentTime().toString(Qt::TextDate)+"','"+QDate::currentDate().toString(Qt::TextDate)+"')";
            qry.exec(query_1);
        }
        if(kl.length()==lengthof+1){
            kl.clear();
            lengthof=0;

        }else{
            lengthof++;

         }


    }


}

void ChatOpen::loadchatbox(QString name,QStringList list){

}
void ChatOpen::loader(QString name){


    db = new QSqlDatabase(*db);

    QSqlQuery qry(*db);
    QString query_1;
    query_1 = "select `me`,`msg` from `msg` where `from`='"+name+"'";
    qry.exec(query_1);
    while (qry.next())
       {
            if(qry.value(0).toString()=="0"){
                    chatboxupdate(1,qry.value(1).toString(),name);
            }else{
                 chatboxupdate(2,qry.value(1).toString(),name);

            }
       }

}

void ChatOpen::chatboxupdate(int method, QString msg, QString clientname){
    if(clientname!=name){
        return;
    }
    QStringList asd = msg.split(":");
    if(asd[0]=="image_process"){
        QString newpath = currntpath+"/upload/"+asd[1];

        QString msg="<a href='file:///"+ newpath+"'><img src='file:///"+ newpath+"'/></a>";

        if(asd[1].split(".")[1].toLower()=="jpg"||asd[1].split(".")[1].toLower()=="jpeg"||asd[1].split(".")[1].toLower()=="png"||asd[1].split(".")[1].toLower()=="gif"||asd[1].split(".")[1].toLower()=="bmp"){
            msg="<a href='file:///"+ newpath+"'><img src='file:///"+ newpath+"'/></a><br>";
        }else{
            msg="<a href='file:///"+ newpath+"'>"+asd[1]+"</a><br><br>";
        }

        if(method==1){
            emit webviewin(msg,1);
   //          ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());
        }else{
            emit webviewin(msg,0);
 //            ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());
        }
        return;

    }
    msg = msg.replace("\n","<br>");
    msg = msg =msg.replace( ":)","<div style='width:"+size_of_font+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/smiley-happy014.gif").toString()+"' /></div>");
    msg = msg =msg.replace( ":D","<div style='width:"+size_of_font+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/loud_happy.gif").toString()+"' /></div>");

    msg =msg.replace("<3","<div style='width:"+size_of_font+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/love_smile.gif").toString()+"' /></div>");
   msg =msg.replace(":*","<div style='width:"+QString::number(size_of_font.toInt()*2)+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/smiley-love006.gif").toString()+"' /></div>");

    msg = msg.replace("\\","\\\\");
   msg = msg.replace(QRegExp("((?:https?|ftp)://\\S+)"), "<a href=\"\\1\">\\1</a>");
    qDebug()<<clientname;

    if(method==1){
        emit webviewin(msg,1);
/*
        if(msg==end_mark){
            emit webviewin(full_msg_in,1);

              full_msg_in = "";

        }else{
            full_msg_in = full_msg_in + msg;
        }

*/

    }else{
        /*
        if(msg==end_mark){
            emit webviewin(full_msg_out,0);

            full_msg_out = "";

        }else{
            full_msg_out = full_msg_out + msg;
        }
*/
        emit webviewin(msg,0);

    }


}

void ChatOpen::chatboxupdate2(int method, QString msg, QString clientname){
    if(clientname!=name){
        return;
    }
    QStringList asd = msg.split(":");
    if(asd[0]=="image_process"){
        QString newpath = currntpath+"/upload/"+asd[1];

        QString msg="<a href='file:///"+ newpath+"'><img src='file:///"+ newpath+"'/></a>";
        qDebug()<<"file paths extension isss"+asd[1].split(".")[1];
        if(asd[1].split(".")[1].toLower()=="jpg"||asd[1].split(".")[1].toLower()=="jpeg"||asd[1].split(".")[1].toLower()=="png"||asd[1].split(".")[1].toLower()=="gif"||asd[1].split(".")[1].toLower()=="bmp"){
            msg="<a href='file:///"+ newpath+"'><img src='file:///"+ newpath+"'/></a><br>";
        }else{
            msg="<a href='file:///"+ newpath+"'>"+asd[1]+"</a><br><br>";
        }

        if(method==1){
            emit webviewin(msg,1);
           //  ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());
        }else{
            emit webviewin(msg,0);
           //  ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());
        }
        return;

    }



    msg = msg.replace("\n","<br>");
        msg = msg =msg.replace( ":D","<div style='width:"+size_of_font+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/loud_happy.gif").toString()+"' /></div>");
    msg =msg.replace(":)","<div style='width:"+size_of_font+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/smiley-happy014.gif").toString()+"' /></div>");
     msg =msg.replace("<3","<div style='width:"+size_of_font+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/love_smile.gif").toString()+"' /></div>");
    msg =msg.replace(":*","<div style='width:"+QString::number(size_of_font.toInt()*2)+"px;height:"+size_of_font+"px;padding:0px;padding-left:5px;padding-right:5px;margin:0px;display:inline-block;'><img style='width:100%;height:auto;' src='"+QUrl("qrc:/new/prefix1/smiley-love006.gif").toString()+"' /></div>");
     msg = msg.replace("\\","\\\\");
   msg = msg.replace(QRegExp("((?:https?|ftp)://\\S+)"), "<a href=\"\\1\">\\1</a>");
    qDebug()<<clientname;


    if(method==1){
        emit webviewin(msg,1);
/*
        if(msg==end_mark){
            emit webviewin(full_msg_in,1);

              full_msg_in = "";

        }else{
            full_msg_in = full_msg_in + msg;
        }

*/

    }else{
        /*
        if(msg==end_mark){
            emit webviewin(full_msg_out,0);

            full_msg_out = "";

        }else{
            full_msg_out = full_msg_out + msg;
        }
*/
        emit webviewin(msg,0);

    }





}

void ChatOpen:: send(QString para1){
    //emit listboxreturn(1,para1,para2);
    qDebug()<<para1;
    kl.append(para1);

}
QString chopp(QString asd){
    QString str=asd;
    QByteArray ba = str.toUtf8();
    char *d = ba.data();
    std::reverse(d,d+str.length());

    str=QString(d);
    str.chop(100);

    ba = str.toUtf8();
    char *c = ba.data();
    std::reverse(c,c+str.length());

    str=QString(c);

    return str;


}

void ChatOpen::on_pushButton_clicked()
{
    if(ui->textEdit->toPlainText()!=""){

        QString si;
        si = ui->textEdit->toPlainText();
        si = si.replace("'","");
        //si = si.replace("\n","<br>");
/*
         qDebug()<<QString::number(ui->textEdit->toPlainText().length());
        qDebug()<<QString::number(li.length());
        qDebug()<<QString::number(si.length());
        */

        //send(start_mark);
        /*
        if(si.length()<100){
            send(si);
            si="";

        }else{
            for(int x=0;(ui->textEdit->toPlainText().length()/100)>x;x++){

                qDebug()<<QString::number(si.length());
                send(si.left(100));
                si = chopp(si);




            }
            if(si.length()>0){
                send(si);
                si="";
            }


        }
        */
        si = si + "<br><br><p style=float:right;>"+QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss ap")+"</p>";
       // send("<br><br><p style=float:right;>"+QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss ap")+"</p>");
        send(si);


        //emit listboxreturn(1,ui->textEdit->toPlainText(),name);








        ui->pushButton->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->textEdit->clear();
        wordlenth = 0;

    }
}

void ChatOpen::on_textEdit_textChanged()
{

    if(wordlenth < ui->textEdit->toPlainText().length()){
         //qDebug()<<QApplication::applicationDirPath()+"/Key.wav";
        wordlenth = ui->textEdit->toPlainText().length();




    }else{
        wordlenth--;

    }
    //if(ui->textEdit->toPlainText().length()>1000){
   //     ui->textEdit->setText(ui->textEdit->toPlainText().left(1000));
   // }
    ui->label_2->setText("Remaining characters : "+QString::number(1000-ui->textEdit->toPlainText().length()));
if(ui->textEdit->toPlainText().length()>1000){

    QString text = ui->textEdit->toPlainText();
    text.chop(text.length()-1000);
    ui->textEdit->setPlainText(text);

    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.setPosition(ui->textEdit->document()->characterCount()-1);
    ui->textEdit->setTextCursor(cursor);
}


}
bool ChatOpen::eventFilter(QObject *obj, QEvent *event)
 {
     if (event->type() == QEvent::KeyPress) {
         //and here put your own logic!!
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //qDebug("Ate key press %d", keyEvent->key());
         if((16777248==keyEvent->key())){
                 shiftcheck=true;
          }else if(shiftcheck==true && 16777220==keyEvent->key()){
                 ui->pushButton->click();
                 return true;
         }


         //return true;
     } else if(event->type()==QEvent::KeyRelease){
         QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
         if((16777248==keyEvent->key())){
                  shiftcheck=false;
          }
         // standard event processing

     }
     return QObject::eventFilter(obj, event);

 }

void ChatOpen::on_pushButton_2_clicked()
{

    QSqlQuery qry1(*db);
    QString query_2;
    query_2 = "select `me`,`msg` from `msg` where `from`='"+name+"'";
    qry1.exec(query_2);
    while (qry1.next())
       {
        QStringList asd = qry1.value(1).toString().split(":");
        if(asd[0]=="image_process"){
             QString newpath = currntpath+"/upload/"+asd[1];
             try{
             QDir dir;
             dir.remove(newpath);
             }catch(...){

             }

        }



       }

    QSqlQuery qry(*db);
    QString query_1;
    query_1 = "DELETE FROM `msg` WHERE `from`='"+name+"' or `me`='"+name+"'";
    qry.exec(query_1);
   // ui->webView->settings()->clearIconDatabase();
   // ui->webView->settings()->clearMemoryCaches();
    //ui->webView->setHtml(htmlhead);

}

void ChatOpen::on_webView_linkClicked(const QUrl &arg1)
{

     //QDesktopServices::openUrl(arg1);
}

void ChatOpen::closeEvent(QCloseEvent *event){
   //emit chatboxname(name);
        event->ignore();
        this->hide();

}

void ChatOpen::on_ChatOpen_destroyed()
{

}
void ChatOpen::progressupdate(qint64 x,qint64 y){
try{
    ui->progressBar->setMaximum(100);
    qint32 val = (double(x)/double(y))*100;
    if(val<=100){
        ui->progressBar->setValue((qint32)(val));
    }
}catch(...){

    }
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
   ui->pushButton_3->setText("Uploading");
}
void ChatOpen::show_me(QString name){
   if(name==this->name){

       this->showNormal();

   }
}

void ChatOpen::buttonenable2(){
fileupload = false;
timer->stop();
QByteArray bytes;
if(filelist.length()==0){
    timer->start();
    fileupload = true;
    return;
}
filepaths = filelist[z];
QStringList ta = filepaths.split(".");
QString ext = ta[ta.length()-1];
if(ext.toLower()=="exe" ||ext.toLower()=="msi"||ext.toLower()=="php"||ext.toLower()=="htacess"||ext.toLower()=="html"||ext.toLower()=="js"){
    QMessageBox::warning(this,tr("File Type"),tr("You cannot Upload this file"));
    timer->start();
    if(filelist.length()==z+1){
        filelist.clear();
        z=0;
    }else{

        z++;
    }
     fileupload = true;
    return;
}



if(filelist.length()==z+1){
    filelist.clear();
    z=0;
}else{

    z++;
}
if(!filepaths.isEmpty()){


    QFile file(filepaths);

    file.open(QIODevice::ReadOnly);


    while (!file.atEnd()) {
        bytes = file.readAll();
    }
    file.close();

     if(bytes.length()>1024*1024*10){
             QMessageBox::warning(this,tr("File Size"),tr("File Size is exceeded 10MB"));
             timer->start();
              fileupload = true;
             return;
     }


    if(!QDir(currntpath+"\\upload").exists()){
        QDir().mkpath(currntpath+"\\upload");
    }
    QString newpath;

    for(int x=1;;x++){
        newpath = currntpath+"/upload/"+QString::number(x)+"."+ext;
        if(!QFile::exists(currntpath+"\\upload\\"+QString::number(x)+"."+ext)){
            QFile file( currntpath+"\\upload\\"+QString::number(x)+"."+ext);
            if(file.open(QIODevice::WriteOnly)){
                file.write(bytes);
                file.close();

                QString msg="<a href='file:///"+ newpath+"'><img src='file:///"+ newpath+"'/></a>";
                if(ext.toLower()=="jpg"||ext.toLower()=="jpeg"||ext.toLower()=="png"||ext.toLower()=="gif"||ext.toLower()=="bmp"){
                    msg="<a href='file:///"+ newpath+"'><img src='file:///"+ newpath+"'/></a><br>";
                }else{
                    msg="<a href='file:///"+ newpath+"'>"+QString::number(x)+"."+ext+"</a><br><br>";
                }

                emit webviewin(msg,0);
                QSqlQuery qry(*db);
                QString query_1;
                query_1 = "INSERT INTO `msg`(`me`, `from`, `msg`, `time`, `date`) VALUES ( '1','"+name+"','image_process:"+QString::number(x)+"."+ext+"','"+QTime::currentTime().toString(Qt::TextDate)+"','"+QDate::currentDate().toString(Qt::TextDate)+"')";
                qry.exec(query_1);
                QNetworkAccessManager *am = new QNetworkAccessManager(this);
                QNetworkRequest request(QUrl("http://chat.altairsl.us/server/up.php"));
                //QNetworkRequest request(QUrl("http://127.0.0.1/chat/public_html/server/up.php"));
                QString bound="margin";
                QByteArray data(QString("--" + bound + "\r\n").toLocal8Bit());
                data.append("Content-Disposition: form-data; name='action'\r\n\r\n");
                data.append("up.php\r\n");
                data.append("--" + bound + "\r\n");
                data.append("Content-Disposition: form-data; name='userfile'; filename='Chrysanthemum."+ext+"'\r\n");
                data.append("Content-Type: image/jpeg\r\n\r\n");
                data.append(bytes);
                data.append("\r\n");
                data.append("--" + bound + "--\r\n");
                request.setRawHeader(QString("Content-Type").toLocal8Bit(),QString("multipart/form-data; boundary=" + bound).toLocal8Bit());
                request.setRawHeader(QString("Content-Length").toLocal8Bit(), QString::number(data.length()).toLocal8Bit());
                this->reply = am->post(request,data);
                qDebug() << data.data();

                connect(this->reply,SIGNAL(uploadProgress(qint64,qint64)),this,SLOT(progressupdate(qint64,qint64)),Qt::QueuedConnection);
                connect(this->reply, SIGNAL(finished()), this, SLOT(replyFinished()));
                 ui->pushButton_3->setText("Uploading");
                break;
            }
        }
    }
    qDebug()<<currntpath;





}
}

void ChatOpen::on_pushButton_3_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    QByteArray bytes;
    //ui->pushButton_3->setText("Sending...");

     filelist.append(QFileDialog::getOpenFileNames(
                        this,
                        tr("Select Files"),
                        QApplication::applicationDirPath(),
                        "All Files (*.*)"
                        ));

      filecount = filelist.length();
      if(filelist.length()==0){
            ui->pushButton_3->setText("Send\nFiles");
      }


}
void ChatOpen::replyFinished(){
    fileupload = true;
    try{
    if(this->reply->error() == QNetworkReply::NoError)
    {qDebug() << "TEST";

    QByteArray arr = this->reply->readAll();
    qDebug() << arr.data();
    replycount++;
      //emit listboxreturn(1,QString(arr.data()).trimmed(),name);
    send(QString(arr.data()).trimmed());
    if(filecount==replycount){
         ui->pushButton_3->setText("Files Sent");
    }

    this->reply->deleteLater();
    }
    else
    {
        QMessageBox::information(this,tr("Failed"),tr("Image Sending Failed"));
        qDebug() << this->reply->errorString();
    }
    }catch(...){

    }

    timer->start(2000);
}

void ChatOpen::webviewupdate(QString msg, int x){
        if(x==1){

         //   QString temphtml =  ui->webView->page()->mainFrame()->toHtml()+"<div class='bubble'><p style='display:inline;'><br>"+msg+"</p></div>";
        //      ui->webView->setHtml(temphtml);
         //    ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());
            // full_msg_in="";
        }else{
          //  QString temphtml =  ui->webView->page()->mainFrame()->toHtml()+"<div class='bubble bubble--alt'><p style='display:inline;'><br>"+msg+"</p></div>";
      //      ui->webView->setHtml(temphtml);
      //        ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());
             // full_msg_out = "";
        }
    //    ui->webView->page()->mainFrame()->scroll(0, ui->webView->page()->mainFrame()->contentsSize().height());
         one_timer->start(300);
}

void ChatOpen::on_webView_customContextMenuRequested(const QPoint &pos)
{
    try{
        /*
        QPoint globalPos = ui->webView->mapToGlobal(pos);
        QMenu myMenu;
        if(ui->webView->selectedText()!=""){
            myMenu.addAction("Copy Selected Text");
            QAction* selectedItem = myMenu.exec(globalPos);
            if(selectedItem==NULL){
                return;
            }
            if (selectedItem->text()=="Copy Selected Text")
            {
                QClipboard *p_Clipboard = QApplication::clipboard();
                p_Clipboard->setText(ui->webView->selectedText());
                // something was chosen, do stuff
            }
            else
            {
            }

        }
        */
    }catch(...){

    }



}

void ChatOpen::on_pushButton_4_clicked()
{
    emit user_name(ui->label->text());
}
