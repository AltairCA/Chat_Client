#include "mytask1.h"

MyTask1::MyTask1()
{
}
void MyTask1::run(){
        QNetworkAccessManager *am = new QNetworkAccessManager();
        QNetworkRequest request(QUrl("http://127.0.0.1/lal/up.php"));
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
}
