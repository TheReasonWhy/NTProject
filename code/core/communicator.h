#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QObject>
#include "server.h"
#include "client.h"
#include "message.h"
#include <QSharedPointer>

class communicator
{
    Q_OBJECT
public:
    communicator();
};

#endif // COMMUNICATOR_H
