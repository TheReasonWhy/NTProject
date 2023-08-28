#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QCommandLineParser>
#include <QUuid>
#include "workerbase.h"

class initializer;

class module : public QObject
{
    Q_OBJECT
public:
    module(QObject *parent = nullptr);
    module(QSharedPointer<initializer> _initializer, QSharedPointer<workerBase> _worker, QObject *parent = nullptr);
    virtual ~module();
    bool parseCommandLine(const QList<QCommandLineOption> &options = QList<QCommandLineOption>(), std::function<bool (QCommandLineParser &parser)> *customCommandLineWork = nullptr);

    virtual bool parseIniFile();
    QUuid moduleUuid() const;
    virtual bool start();

protected:
    QUuid uid;
    QSharedPointer<initializer> m_initializer;
    QSharedPointer<QJsonObject> m_current_settings;
    QSharedPointer<workerBase> m_worker;
};

#endif // MODULE_H
