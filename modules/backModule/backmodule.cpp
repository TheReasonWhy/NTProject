#include "backmodule.h"
#include "initializer.h"
#include "database.h"

backModule::backModule(QObject *parent):module(parent){}

backModule::backModule(QSharedPointer<initializer> _initializer, QObject *parent):module(_initializer, parent)
{
    parseIniFile();
}

backModule::~backModule()
{

}

bool backModule::parseIniFile()
{
    QJsonObject auth = m_initializer.get()->read_property_group("DATABASE");

    m_current_settings.get()->insert("DATABASE", auth);
    const QString host = m_current_settings.get()->value("DATABASE").toObject().value("HOSTNAME").toString();
    const int port = m_current_settings.get()->value("DATABASE").toObject().value("PORT").toInt();
    const QString dbName = m_current_settings.get()->value("DATABASE").toObject().value("DBNAME").toString();
    const QString userName = m_current_settings.get()->value("DATABASE").toObject().value("USERNAME").toString();
    const QString password = m_current_settings.get()->value("DATABASE").toObject().value("PASSWORD").toString();
    dbSettings sett(host,port,dbName,userName,password);
    //m_db = new database(sett,this);
    return true;

}
