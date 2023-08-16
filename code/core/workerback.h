#ifndef WORKERBACK_H
#define WORKERBACK_H

#include <QObject>
#include "workerbase.h"

class workerback : public workerBase
{
    Q_OBJECT
public:
    workerback();
    workerback(const QUuid uid, const QList <QSharedPointer<workerBase>> children):workerBase(uid, children){

    };
};

#endif // WORKERBACK_H
