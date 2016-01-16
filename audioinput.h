#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QtCore>
#include <QtMultimedia>
#include <QList>

class AudioInput : public QObject
{
    Q_OBJECT
public:
    explicit AudioInput(QAudioDeviceInfo devinfo, QObject *parent = 0);

signals:
    void dataReady(QByteArray data);
    void getVolume(qreal amount);
public slots:
    void recive_volume();
private slots:
    void readyRead();

private:
    QAudioInput *audio;
     QByteArray m_buffer;
    QIODevice *device;
};

#endif // AUDIOINPUT_H
