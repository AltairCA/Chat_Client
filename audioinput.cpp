#include "audioinput.h"


AudioInput::AudioInput(QAudioDeviceInfo devinfo, QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleSize(8);

    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);



    audio = new QAudioInput(devinfo, format, this);
    audio->setBufferSize(4096);



    device = audio->start();


    connect(device, SIGNAL(readyRead()), SLOT(readyRead()));
}

void AudioInput::readyRead()
{
    QByteArray data;
    QList<double> btlist;
    //Check the number of samples in input buffer
    qint64 len = audio->bytesReady();

    //Read sound samples from input device to buffer
    if (len > 0)
    {
        data.resize(len);
        device->read(data.data(), len);

        short* resultingData = (short*)data.data();

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
                      emit dataReady(data);
                      ch = false;
                  }


              }
              emit getVolume(ne);

          }

    }



}
void AudioInput::recive_volume(){
    qreal asd = 0;
    emit getVolume(asd);
}
