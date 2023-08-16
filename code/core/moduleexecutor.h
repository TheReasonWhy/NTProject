#ifndef MODULEEXECUTOR_H
#define MODULEEXECUTOR_H

#include <QObject>
#include "module.h"
#include <QDir>

class moduleExecutor : public QObject
{
    Q_OBJECT
public:
    moduleExecutor() = delete;
    static int exec(int argc, char *argv[], module* _module);
};

#endif // MODULEEXECUTOR_H
