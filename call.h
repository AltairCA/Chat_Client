#ifndef CALL_H
#define CALL_H

#include <QDialog>
#include "QCloseEvent"
#include "audiooutput.h"
#include "audioinput.h"
#include <QMessageBox>
namespace Ui {
class Call;
}

class Call : public QDialog
{
    Q_OBJECT

public:
    explicit Call(QWidget *parent = 0);
    ~Call();
     bool answered;

private:
    Ui::Call *ui;
    AudioOutput *output;
     AudioInput *input;


public slots:
    void from_namelist( QByteArray data);
    void from_mic( QByteArray data);
    void exitcall();
    void getfrommic(qreal volume);
     void getfromout(qreal volume);
     void to_call(QString name);
     void from_call(QString name);
signals:
    void to_namelist(QByteArray data);
    void disconnect_socket();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_Call_finished(int result);

protected:

     void closeEvent(QCloseEvent *event);

};

#endif // CALL_H
