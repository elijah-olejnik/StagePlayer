#ifndef PLAYLISTITEMMODEL_H
#define PLAYLISTITEMMODEL_H
#include <QAudioFormat>
#include <QTime>
struct PlayListItem
{
    QString filePath;
    QString fileName;
    QTime durationTime;
    QAudioFormat audioFormat;
    quint32 dataLengthSecs;
    quint64 dataPosition;
    quint32 byteRate;
    quint32 dataSize;
};
#endif // PLAYLISTITEMMODEL_H
