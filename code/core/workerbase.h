#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <QObject>
#include <QUuid>
#include "period.h"
#include <QSharedPointer>
/*
    period - время жизни модуля или задания
    periodable - что-то имеющее время жизни
        ресурс
        ресурсоемкость

        resource - time
        resource_type
        task : resource_consumer
        path : resource_consumer
    capacity
*/

class workerBase : public QObject{
    Q_OBJECT
public:
    workerBase(const QUuid uid):m_uid(uid){

    };
    workerBase(const QUuid uid, const QList <QSharedPointer<workerBase>> children):m_uid(uid), m_children(children){

    };
    void updateCapacity(const int capacity){
        m_capacity = capacity;
    };

    const QUuid m_uid;
    int m_capacity = 0;
    QList <QSharedPointer<workerBase>>  m_children;
};

#endif // WORKERBASE_H
