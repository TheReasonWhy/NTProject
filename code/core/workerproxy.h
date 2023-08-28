#ifndef WORKERPROXY_H
#define WORKERPROXY_H

#include <QObject>
#include "workerbase.h"

class workerproxy : public workerBase{
    Q_OBJECT
public:
    workerproxy();
    workerproxy(const QUuid uid, const QList <QSharedPointer<workerBase>> children):workerBase(uid, children){

    };
    virtual void start()override;
};

#endif // WORKERPROXY_H
