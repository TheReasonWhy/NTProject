#ifndef WORKERFINAL_H
#define WORKERFINAL_H

#include <QObject>
#include "workerbase.h"

class workerFinal  : public workerBase{
    Q_OBJECT
public:
    workerFinal(const QUuid uid, const QSharedPointer<workerBase>  children) = delete;
    workerFinal(const QUuid uid, const int capacity):workerBase(uid){
        updateCapacity(capacity);
    };

    virtual void start()override;
    QList <workerBase> m_children;
};

#endif // WORKERFINAL_H
