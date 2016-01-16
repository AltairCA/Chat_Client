#include "friendsearch.h"
#include "ui_friendsearch.h"

friendsearch::friendsearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::friendsearch)
{
    ui->setupUi(this);


}

friendsearch::~friendsearch()
{
    delete ui;
}

void friendsearch::on_lineEdit_returnPressed()
{

    if(ui->lineEdit->text()!="" || ui->lineEdit->text()==" "){
        ui->listWidget->clear();
        // qDebug("hard return");
        emit search_keyout(ui->lineEdit->text());
    }
}

void friendsearch::name_in(QString name){
    if(!namelister.contains(name)){
         ui->listWidget->addItem(name);
    }

}


void friendsearch::on_listWidget_clicked(const QModelIndex &index)
{

}

void friendsearch::on_listWidget_doubleClicked(const QModelIndex &index)
{
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Add Friend", "Send a Friend Request to "+ui->listWidget->currentItem()->text()+" ?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
       emit friend_request(ui->listWidget->currentItem()->text());
        ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));

      } else {

      }
}
