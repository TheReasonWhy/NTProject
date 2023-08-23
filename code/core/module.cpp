#include "module.h"
#include "initializer.h"

module::module(QObject *parent):QObject(parent),
    m_initializer(nullptr){}

module::module(QSharedPointer<initializer> _initializer, QObject *parent):QObject(parent),
    m_initializer(_initializer)
{
    m_initializer.get()->readSettings();
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

bool module::parseIniFile(){return true;}

QUuid module::moduleUuid() const { return uid; }

bool module::start()
{
    return true;
}
