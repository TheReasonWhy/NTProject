#ifndef WORKERPROXY_H
#define WORKERPROXY_H

#include <QObject>
#include "workerbase.h"

class workerproxy : public workerBase{
    Q_OBJECT
public:
    workerproxy();
};

#endif // WORKERPROXY_H
