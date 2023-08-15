#ifndef WORKERBASE_H
#define WORKERBASE_H

#include <QObject>
#include <QUuid>


class workerBase : public QObject{
    Q_OBJECT
public:
    workerBase(const QUuid uid):m_uid(uid){

    };
    workerBase(const QUuid uid, const QList <workerBase> children):m_uid(uid), m_children(children){

    };
    void updateCapacity(const int capacity){
        m_capacity = capacity;
    };

    const QUuid m_uid;
    int m_capacity = 0;
    QList <workerBase> m_children;
};

#endif // WORKERBASE_H
