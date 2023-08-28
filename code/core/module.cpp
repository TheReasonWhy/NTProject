#include "module.h"
#include "initializer.h"

module::module(QObject *parent):QObject(parent),
    m_initializer(nullptr),
    m_current_settings(QSharedPointer<QJsonObject>(new QJsonObject())){
    qDebug()<<"module::module";}

module::module(QSharedPointer<initializer> _initializer, QSharedPointer<workerBase> _worker, QObject *parent):QObject(parent),
    m_initializer(_initializer),
    m_current_settings(QSharedPointer<QJsonObject>(new QJsonObject())),
    m_worker(_worker)
{
    qDebug()<<"module::module 2";
}

module::~module(){}

bool module::parseCommandLine(const QList<QCommandLineOption> &options, std::function<bool (QCommandLineParser &)> *customCommandLineWork)
{

    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.addHelpOption();

    QCommandLineOption uuid("uuid", "Module uuid", "uuid");

    parser.addOptions({ uuid });
    parser.addOptions(options);
    parser.process(*qApp);

    QUuid uuidValue(parser.value(uuid));
    if (!uuidValue.isNull())
        uid = uuidValue;

    return true;
}

bool module::parseIniFile(){
    m_initializer->readSettings();
    return true;
}

QUuid module::moduleUuid() const { return uid; }

bool module::start()
{
    qDebug()<<"module::start()";
    m_worker.get()->start();
    return true;
}
