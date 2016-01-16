#include "friendrequest.h"
#include "ui_friendrequest.h"

friendrequest::friendrequest(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::friendrequest)
{
    ui->setupUi(this);
    this->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setFocusPolicy(Qt::StrongFocus);
   ui->listWidget->installEventFilter(this);
}

friendrequest::~friendrequest()
{
    delete ui;
}
void friendrequest::new_requests(QString name){
    bool ch = true;
        for (int i = 0; i < ui->listWidget->count(); i++) {
                if(name ==ui->listWidget->item(i)->text() ){
                            ch=false;
                            break;
                }

            }
        if(ch){
        ui->listWidget->addItem(name);
        }
}

void friendrequest::on_pushButton_clicked()
{
    ui->listWidget->clear();
   emit search_requests();
}

void friendrequest::on_listWidget_doubleClicked(const QModelIndex &index)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Accept Friend Request", "Accept the Friend Request from "+ui->listWidget->currentItem()->text()+" ?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
     emit accept_request(ui->listWidget->currentItem()->text());
        on_pushButton_clicked();
    } else {

    }
}
bool friendrequest::eventFilter(QObject *obj, QEvent *event)
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
