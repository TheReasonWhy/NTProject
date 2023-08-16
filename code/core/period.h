#ifndef PERIOD_H
#define PERIOD_H
#include <QUuid>
#include <QDateTime>

class period
{
public:
    period(const QUuid uid, const qint64 _start, const qint64 _length = 0):m_uid(uid),m_start(_start),m_length(_length){};
    period(const QUuid uid, const QDateTime _start, const qint64 _length = 0):m_uid(uid), m_start(_start.toMSecsSinceEpoch()),m_length(_length){};
    period(const QUuid uid, const QString _start, const qint64 _length = 0):m_uid(uid), m_start(geTimestamp(_start)),m_length(_length){};

    qint64 timeleft(){
        return m_length - (QDateTime::currentMSecsSinceEpoch() - m_start);
    }
    void start(){
        m_start = QDateTime::currentMSecsSinceEpoch();
    };
    void start(const qint64 _start){
        m_start = _start;
    };
    void start(const QDateTime _start){
        m_start = _start.toMSecsSinceEpoch();
    };
    static qint64 geTimestamp(const QString str)
    {
        QString time_format = "dd-MM-yyyyTHH:mm:ss";
        QDateTime res = QDateTime::fromString(str, time_format);
        return res.toMSecsSinceEpoch();
    }

    const QUuid m_uid;
    qint64 m_start;
    qint64 m_length;
};

#endif // PERIOD_H
