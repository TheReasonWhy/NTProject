#include "module.h"

module::module()
{

}

bool module::commandLineWork(const QList<QCommandLineOption> &options, std::function<bool (QCommandLineParser &)> *customCommandLineWork)
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

bool module::start()
{
    return true;
}
