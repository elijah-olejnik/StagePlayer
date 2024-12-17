#ifndef DECK_H
#define DECK_H

#include <QObject>
#include <QFile>
#include <QAudio>
#include <QAudioOutput>
#include <QAudioDeviceInfo>
#include <QTimer>
#include <chrono>
#include <QDebug>
#include <QThread>
#include <warningdialog.h>
#include "PlayListItemModel.h"

using namespace std;

typedef QAudio::State deckState;

class Deck : public QObject
{
    Q_OBJECT

public:
    explicit Deck();
    ~Deck();
    int duration() const;
    QTime totalTime() const;
    qint64 currentPosition();
    QTime currentTime();
    QTime reverseTime();
    int currentVolume();

private:
    const PlayListItem *mediaItem;
    QFile media;
    QAudioOutput* output;
    deckState crntState;
    QTime positionTime,revTime,zeroTime;
    bool createAudioOutput(QAudioFormat format, QAudioDeviceInfo device, int startGain);
    void play(QString source, quint64 startPos);
    WarningDialog *wD;
    bool loop;

public slots:
    void loadAndPlay(const PlayListItem *plTrack, QAudioDeviceInfo info, int startVolume,bool repeatTrack);
    void pause();
    void resume();
    void stop();
    void setVolume(int targetVolume);
    void setPosition(unsigned int targetPosition);

private slots:
    void handleStateChanged(deckState);
    void handleNotify();

signals:
    void myStateChanged(qint64,QTime,QTime,int,deckState,QString);
};

class Controller : public QObject
{
    Q_OBJECT

public:
    QThread deckThread;
    QTimer fadeTimer;

    Controller()
    {
        qRegisterMetaType<deckState>("deckState");
        Deck *deck = new Deck();
        deck->moveToThread(&deckThread);
        connect(&deckThread,&QThread::finished,deck,&QObject::deleteLater);
        connect(this,&Controller::operate,deck,&Deck::loadAndPlay);
        connect(this,&Controller::suspend,deck,&Deck::pause);
        connect(this,&Controller::unPause,deck,&Deck::resume);
        connect(this,&Controller::finish,deck,&Deck::stop);
        connect(this,&Controller::changeGain,deck,&Deck::setVolume);
        connect(this,&Controller::setPos,deck,&Deck::setPosition);
        connect(deck,&Deck::myStateChanged,this,&Controller::transmitDeckState);
        deckThread.start();
        fadeTimer.stop();
        connect(&fadeTimer,&QTimer::timeout,this,&Controller::fadeToTrgtVol);
        connect(this,&Controller::stopFade,&fadeTimer,&QTimer::stop);
    }

    ~Controller()
    {
        deckThread.quit();
        deckThread.wait();
    }

private:
    int crntVol,trgtVol;
    qreal tmrInterval;
    deckState dState;

signals:
    void operate(const PlayListItem*,QAudioDeviceInfo,int,bool);
    void suspend();
    void unPause();
    void finish();
    void changeGain(int);
    void setPos(int);
    void currentDeckState(qint64,QTime,QTime,int,deckState,QString);
    void stopFade();

public slots:
    void play(const PlayListItem *plstItem,QAudioDeviceInfo deviceInfo,int vol,bool rpt){emit operate(plstItem,deviceInfo,vol,rpt);}

    void pause()
    {
        switch(dState)
        {
        case QAudio::ActiveState:
            emit suspend();
            break;
        case QAudio::SuspendedState:
            emit unPause();
            break;
        default:
            break;
        }
    }

    void stop()
    {
        fadeTimer.stop();
        emit finish();
        disconnect(this,&Controller::stopFade,this,nullptr);
    }

    void setVolume(int volume)
    {
        if(dState == QAudio::ActiveState || dState == QAudio::SuspendedState)
            emit changeGain(volume);
        else
            return;
    }

    void search(int index){emit setPos(index);}

    void transmitDeckState(qint64 crntPosSecs,QTime crntPosTime,QTime rvrsTime,int crntLevel,deckState crntDeckState,QString pfName)
    {
        crntVol = crntLevel;
        dState = crntDeckState;
        emit currentDeckState(crntPosSecs,crntPosTime,rvrsTime,crntLevel,crntDeckState,pfName);
    }

    void fade(int nVolume,int nTime)
    {
        fadeTimer.stop();
        trgtVol = nVolume;
        if(dState != QAudio::ActiveState)
            return;
        else if(trgtVol < crntVol)
        {
            tmrInterval = qreal(nTime * 1000) / qreal(crntVol - trgtVol);
            fadeTimer.setTimerType(Qt::PreciseTimer);
            fadeTimer.start(chrono::milliseconds(qRound(tmrInterval)));
        }
        else if(trgtVol > crntVol)
        {
            tmrInterval = qreal(nTime * 1000) / qreal(trgtVol - crntVol);
            fadeTimer.setTimerType(Qt::PreciseTimer);
            fadeTimer.start(chrono::milliseconds(qRound(tmrInterval)));
        }
        else
            return;
    }

    void fadeOut(int nFadeTime)
    {
        connect(this,&Controller::stopFade,this,&Controller::stop);
        fade(0,nFadeTime);
    }

private slots:
    void fadeToTrgtVol()
    {
        if(trgtVol < crntVol)
        {
            emit changeGain(crntVol - 1);
        }
        else if(trgtVol > crntVol)
        {
            emit changeGain(crntVol + 1);
        }
        else
        {
            emit stopFade();
            trgtVol = 0;
        }
    }
};

#endif // DECK_H
