#include "call.h"
#include "ui_call.h"

Call::Call(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Call)
{
    ui->setupUi(this);
     answered = false;
    output = new AudioOutput();
    connect(output,SIGNAL(getVolume(qreal)),this,SLOT(getfromout(qreal)));
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if (devices.isEmpty()) QMessageBox::warning(this, "Error", "No input device found!");
    for(int i = 0; i < devices.size(); ++i){
        ui->comboBox->addItem(devices.at(i).deviceName(), qVariantFromValue(devices.at(i)));
    }


}

Call::~Call()
{
    delete ui;


}



void Call::on_pushButton_clicked()
{
    if(ui->comboBox->currentIndex()>-1){
        QAudioDeviceInfo devinfo = ui->comboBox->itemData(ui->comboBox->currentIndex()).value<QAudioDeviceInfo>();
        input = new AudioInput(devinfo, this);

        connect(input, SIGNAL(dataReady(QByteArray)), this,SLOT(from_mic(QByteArray)));
        connect(input,SIGNAL(getVolume(qreal)),this,SLOT(getfrommic(qreal)));

    }
    ui->comboBox->setEnabled(false);
    answered =true;
    ui->pushButton->setEnabled(false);
    ui->pushButton->setVisible(false);

}

 void Call::from_namelist( QByteArray data1){
     if(answered){



     QList<double> btlist;
     //Check the number of samples in input buffer
     qint64 len = data1.length();

     //Read sound samples from input device to buffer
     if (len > 0)
     {



         short* resultingData = (short*)data1.data();

         if(len > 4096){
             len = 4096;
         }
         for (int i=0; i < len/2; i++ )
              {
                  btlist.append( resultingData[ i ]);
              }
        bool ch = true;
         for(int i = 1; i < btlist.size(); i++){
               int x1 =btlist.at(i);
              double x2 = sqrt(btlist.at(i)*btlist.at(i));
               x1 = (x2/32768.0)*100.0;
               int ne =100 - x1;
               ne = ne*2;
               if(ne>100){
                   ne = 100;
               }else if(ne<4){
                   ne = 0;
               }
               if(ne>5){
                   if(ch){
                       output->writeData(data1);
                       ch=false;
                   }

               }
               ui->progressBar_2->setValue(ne);


           }

     }


     }

 }
void Call::from_mic(QByteArray data){
    emit to_namelist(data);
}

void Call::on_pushButton_2_clicked()
{
    this->deleteLater();
     this->close();

}

void Call::on_Call_finished(int result)
{
    //emit disconnect_socket();

}
void Call::exitcall(){
this->close();


}
void Call::closeEvent(QCloseEvent *event){
    //emit show_perant();
    this->hide();
    emit disconnect_socket();

   event->ignore();
   this->deleteLater();
}
void Call::getfrommic(qreal volume){
    //qDebug()<<volume;

    ui->progressBar->setValue(volume);
}
 void Call::getfromout(qreal volume){
    // qDebug()<<QString::number(volume*10);

    // ui->progressBar_2->setValue(val);
 }
 void Call::to_call(QString name){

     ui->label->setText(name);
     this->on_pushButton_clicked();

 }

 void Call::from_call(QString name){
    ui->label->setText(name);
    ui->pushButton->setText("Answer");

 }
