#include "deck.h"
#include "wavheader.h"

Deck::Deck()
{
    zeroTime.setHMS(0,0,0,0);
    qRegisterMetaType<deckState>("deckState");
    crntState = QAudio::StoppedState;
}

void Deck::loadAndPlay(const PlayListItem *plTrack, QAudioDeviceInfo info, int startVolume, bool repeatTrack)
{
    if(crntState == QAudio::ActiveState || crntState == QAudio::SuspendedState)
        stop();
    mediaItem = plTrack;
    loop = repeatTrack;
    if(createAudioOutput(plTrack->audioFormat,info,startVolume) == true)
        play(plTrack->filePath,plTrack->dataPosition);
    else
        return;
}

bool Deck::createAudioOutput(QAudioFormat format,QAudioDeviceInfo device, int startGain)
{
    if(!device.isFormatSupported(format))
    {
        wD = new WarningDialog();
        wD->display("FORMAT NOT SUPPORTED!");
        wD->exec();
        return false;
    }
    else
    {
        output = new QAudioOutput(device,format,this);
        connect(output,SIGNAL(stateChanged(deckState)),this,SLOT(handleStateChanged(deckState)));
        connect(output,SIGNAL(notify()),this,SLOT(handleNotify()));
        output->setNotifyInterval(5);
        setVolume(startGain);
        return true;
    }
}

void Deck::play(QString source,quint64 startPos)
{
    media.setFileName(source);
    if(!media.open(QIODevice::ReadOnly))
    {
        wD = new WarningDialog();
        wD->display("CAN'T OPEN THIS FILE!");
        wD->exec();
        return;
    }
    else
    {
        media.seek(static_cast<qint64>(startPos));
        output->start(&media);
    }
}

void Deck::handleStateChanged(deckState newState)
{
    crntState = newState;
    switch(newState)
    {
    case QAudio::IdleState:
        emit myStateChanged(0,zeroTime,zeroTime,10,newState,"");
        stop();
        break;
    case QAudio::StoppedState:
        if(output->error() != QAudio::NoError)
        {
            wD = new WarningDialog();
            wD->display("OUTPUT ERROR OCCURED...");
            wD->exec();
            emit myStateChanged(0,zeroTime,zeroTime,10,newState,"");
            stop();
        }
        else
        {
            emit myStateChanged(0,zeroTime,zeroTime,10,newState,"");
            stop();
        }
        break;
    case QAudio::SuspendedState:
        emit myStateChanged(currentPosition(),currentTime(),reverseTime(),currentVolume(),newState,mediaItem->fileName);
        break;
    case QAudio::ActiveState:
        emit myStateChanged(currentPosition(),currentTime(),reverseTime(),currentVolume(),newState,mediaItem->fileName);
        break;
    default:
        break;
    }
}

void Deck::handleNotify()
{
    emit myStateChanged(currentPosition(),currentTime(),reverseTime(),currentVolume(),crntState,mediaItem->fileName);
    if((mediaItem->dataSize - media.pos()) <= (mediaItem->byteRate / 50) && loop == true)
        media.seek(static_cast<qint64>(mediaItem->dataPosition + (mediaItem->byteRate / 50)));
    else if((mediaItem->dataSize - media.pos()) <= (mediaItem->byteRate / 100))
        output->setVolume(0.0);
}

void Deck::pause()
{
    if(crntState == QAudio::ActiveState)
        output->suspend();
    else
        return;
}

void Deck::resume()
{
    if(crntState == QAudio::SuspendedState)
        output->resume();
    else
        return;
}

void Deck::stop()
{
    if(crntState == QAudio::ActiveState || crntState == QAudio::IdleState || crntState == QAudio::SuspendedState)
    {
        output->setVolume(0.0);
        output->stop();
        media.close();
        output->setVolume(1.0);
        emit myStateChanged(0,zeroTime,zeroTime,100,crntState,"");
        delete output;
    }
    else
    {
        return;
    }
}

void Deck::setVolume(int targetVolume)
{
    qreal linearVolume = QAudio::convertVolume(targetVolume / 100,
                                               QAudio::LogarithmicVolumeScale,
                                               QAudio::LinearVolumeScale);
    output->setVolume(linearVolume);
}

void Deck::setPosition(unsigned int targetPosition)
{
    if(crntState == QAudio::ActiveState || crntState == QAudio::SuspendedState)
        media.seek(static_cast<qint64>(((targetPosition / 1000) * mediaItem->byteRate) + mediaItem->dataPosition));
    else
        return;
}

qint64 Deck::currentPosition()
{
    qint64 currentPos = ((media.pos() - static_cast<qint64>(mediaItem->dataPosition)) * 1000) / mediaItem->byteRate;
    return currentPos;
}

QTime Deck::currentTime()
{
    qint32 positionSecs = static_cast<qint32>(media.pos() / mediaItem->byteRate);
    int hours = (positionSecs / 60) / 60;
    int minutes = (positionSecs / 60) % 60;
    int seconds = (positionSecs) %60;
    positionTime.setHMS(hours,minutes,seconds,0);
    return positionTime;
}

QTime Deck::reverseTime()
{
    qint32 remainingSecs = static_cast<qint32>(mediaItem->dataLengthSecs - (media.pos() / mediaItem->byteRate));
    int revhours = (remainingSecs / 60) / 60;
    int revminutes = (remainingSecs / 60) % 60;
    int revseconds = (remainingSecs) %60;
    revTime.setHMS(revhours,revminutes,revseconds,0);
    return revTime;
}

int Deck::currentVolume()
{
    int logVolume = static_cast<int>(QAudio::convertVolume(output->volume(),QAudio::LinearVolumeScale,
                                            QAudio::LogarithmicVolumeScale) * 100);
    return logVolume;
}

Deck::~Deck(){}
