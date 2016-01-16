#include "audiooutput.h"

AudioOutput::AudioOutput(QObject *parent) : QObject(parent)
{
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleSize(8);

    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);


    audio = new QAudioOutput(format, this);
    audio->setBufferSize(4096);

    device = audio->start();

    connect(audio,SIGNAL(notify()),this,SLOT(recive_volume()));
}

void AudioOutput::writeData(QByteArray data)
{
    device->write(data.data(), data.size());

}
void AudioOutput::recive_volume(){
    emit getVolume(audio->volume());
}
