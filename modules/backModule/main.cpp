#include <QCoreApplication>
#include "backmodule.h"
#include "moduleexecutor.h"
#include "initializer.h"

int main(int argc, char *argv[])
{
    QSharedPointer<initializer_back> i = QSharedPointer<initializer_back>(new initializer_back());
    backModule module(i);
    return moduleExecutor::exec(argc, argv, &module);
}
