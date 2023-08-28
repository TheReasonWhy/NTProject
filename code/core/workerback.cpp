#include "workerback.h"

workerback::workerback():workerBase()
{

}

workerback::workerback(const QUuid uid, const QList<QSharedPointer<workerBase> > children):workerBase(uid, children){
    qDebug()<<"workerback() " << uid;
}

void workerback::start()
{
    qDebug()<<"workerback::start()";
}
