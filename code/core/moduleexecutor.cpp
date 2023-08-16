#include "moduleexecutor.h"

int moduleExecutor::exec(int argc, char *argv[], module *_module)
{
    QCoreApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    if (!_module->commandLineWork()){
        return 1;
    }
    if (!_module->start()){
        return 1;
    }
    return a.exec();
}
