#include "database.h"


void database::swap(database &src) noexcept
{
    this->m_settings = src.m_settings;
}

database::database(const dbSettings settings, QObject *parent):QObject(parent),m_settings(settings){}

database::database(const database &src){
    this->m_settings = src.m_settings;
}

database::database(database &&src) noexcept{
    database tmp(std::move(src));
    swap(tmp);
}

database &database::operator=(const database &src)
{
    this->m_settings = src.m_settings;
    return *this;
}

bool database::start(){
    return true;
}

void database::stop(){
}

dbSettings::dbSettings(){}

void dbSettings::swap(dbSettings &src) noexcept{
    this->hostName = src.hostName;
    this->port = src.port;
    this->dbName = src.dbName;
    this->userName = src.userName;
    this->password = src.password;
}

dbSettings::dbSettings(const QString _hostName, const int _port, const QString _dbName, const QString _userName, const QString _password):hostName(_hostName),
    port(_port),
    dbName(_dbName),
    userName(_userName),
    password(_password)
{}

dbSettings::dbSettings(const dbSettings &src){
    this->hostName = src.hostName;
    this->port = src.port;
    this->dbName = src.dbName;
    this->userName = src.userName;
    this->password = src.password;
}

dbSettings::dbSettings(dbSettings &&src) noexcept{
    dbSettings tmp(std::move(src));
    swap(tmp);
}

dbSettings &dbSettings::operator=(dbSettings &&src) noexcept{
    swap(src);
    return *this;
}

dbSettings &dbSettings::operator=(const dbSettings &src)
{
    this->hostName = src.hostName;
    this->port = src.port;
    this->dbName = src.dbName;
    this->userName = src.userName;
    this->password = src.password;
    return *this;
}
