#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <QObject>
#include <QUuid>
#include "period.h"
#include <QSharedPointer>
#include <QMap>
#include <task.h>

class workerBase : public QObject{
    Q_OBJECT
public:
    workerBase(){

    };
    workerBase(const QUuid uid):m_uid(uid){

    };
    workerBase(const QUuid uid, const QList <QSharedPointer<workerBase>> children):m_uid(uid), m_children(children){

    };
    virtual ~workerBase(){}
    void updateCapacity(const int capacity){
        m_capacity = capacity;
    };
    virtual void start();
    virtual void send(QList<task> tasks);

    const QUuid m_uid;
    int m_capacity = 0;
    QList <QSharedPointer<workerBase>>  m_children;
};

#endif // WORKERBASE_H
