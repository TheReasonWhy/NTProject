#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QCommandLineParser>
#include <QUuid>

class initializer;

class module : public QObject
{
    Q_OBJECT
public:
    module(QObject *parent = nullptr);
    module(QSharedPointer<initializer> _initializer, QObject *parent = nullptr);
    virtual ~module();
    bool parseCommandLine(const QList<QCommandLineOption> &options = QList<QCommandLineOption>(), std::function<bool (QCommandLineParser &parser)> *customCommandLineWork = nullptr);

    virtual bool parseIniFile();
    QUuid moduleUuid() const;
    bool start();

protected:
    QUuid uid;
    QSharedPointer<initializer> m_initializer;
    QSharedPointer<QJsonObject> m_current_settings;
};

#endif // MODULE_H
