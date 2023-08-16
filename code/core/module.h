#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QCommandLineParser>
#include <QUuid>

class module : public QObject
{
    Q_OBJECT
public:
    module();
    bool commandLineWork(const QList<QCommandLineOption> &options = QList<QCommandLineOption>(), std::function<bool (QCommandLineParser &parser)> *customCommandLineWork = nullptr);


    QUuid moduleUuid() const { return uid; }
    bool start();
private:

    QUuid uid;
};

#endif // MODULE_H
