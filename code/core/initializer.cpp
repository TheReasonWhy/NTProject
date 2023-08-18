#include "initializer.h"
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include "coreenums.h"

initializer::initializer():
    m_path(QCoreApplication::applicationDirPath()+"/config/parameters")
{
    QDir dir;
    if(!dir.exists(QCoreApplication::applicationDirPath()+"/config")){
        if(dir.mkpath(QCoreApplication::applicationDirPath()+"/config")){

        }
    }
    QFile p(m_path);
    if(p.open(QIODevice::ReadWrite)){
        p.close();
    }
    m_settings = new QSettings(m_path, QSettings::IniFormat);
    readSettings();
}

initializer::~initializer()
{

}

void initializer::set_default_DATABASE(const settings sett_)
{
    QString param;
    QString val;
    switch (sett_) {
        case hostName:
        param = "HOSTNAME";
        val = corespace::defaults::hostName;
        break;
        case port:
        param = "PORT";
        val = corespace::defaults::port;
        break;
        case dbName:
        param = "DBNAME";
        val = corespace::defaults::dbName;
        break;
        case userName:
        param = "USERNAME";
        val = corespace::defaults::userName;
        break;
        case password:
        param = "PASSWORD";
        val = corespace::defaults::password;
        break;
    }
    m_settings->beginGroup("DATABASE");
    m_settings->setValue(param, val);
    m_settings->endGroup();
}

void initializer::sync()
{
    m_settings->sync();
}

bool initializer::readSettings()
{
    bool m_static_ip_mode(true);
    QJsonObject dbauth = read_property_group("DATABASE");

    if(!dbauth.contains("HOSTNAME") || (dbauth.value("HOSTNAME").toString() == "")){
        set_default_DATABASE(settings::hostName);
    }
    if(!dbauth.contains("PORT") || (dbauth.value("PORT").toString() == "")){
        set_default_DATABASE(initializer::settings::port);
    }
    if(!dbauth.contains("DBNAME") || (dbauth.value("DBNAME").toString() == "")){
        set_default_DATABASE(initializer::settings::dbName);
    }
    if(!dbauth.contains("USERNAME") || (dbauth.value("USERNAME").toString() == "")){
        set_default_DATABASE(initializer::settings::userName);
    }
    if(!dbauth.contains("PASSWORD") || (dbauth.value("PASSWORD").toString() == "")){
        set_default_DATABASE(initializer::settings::password);
    }

    sync();
    return m_static_ip_mode;
}

QJsonObject initializer::read_property_group(const QString code)
{
    QJsonObject res;
    m_settings->beginGroup(code);
    foreach(auto i, m_settings->childKeys()){
        QString key  = i;
        QString Val  = m_settings->value(key).toString();
        res.insert(key,Val);
    }
    m_settings->endGroup();
    return res;
}



