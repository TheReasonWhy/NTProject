#include "moduleexecutor.h"
#include <QSettings>

int moduleExecutor::exec(int argc, char *argv[], module *_module)
{
    QCoreApplication a(argc, argv);
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QDir dir;
    if(!dir.exists(QCoreApplication::applicationDirPath()+"/config")){
        if(dir.mkpath(QCoreApplication::applicationDirPath()+"/config")){

        }
    }
    QFile p(QCoreApplication::applicationDirPath()+"/config/parameters");
    if(p.open(QIODevice::ReadWrite)){
        p.close();
    }
    //QSettings s(QCoreApplication::applicationDirPath()+"/config/parameters", QSettings::IniFormat);
    if (!_module->parseCommandLine()){
        return 1;
    }
    if (!_module->start()){
        return 1;
    }
    return a.exec();
}
