#include "initializer.h"
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include "coreenums.h"

initializer::initializer()
{
    qDebug()<<"initializer::initializer()";

    //readSettings();
}

initializer::~initializer()
{
    if(m_settings){delete m_settings;}
}

void initializer::set_default_MODULE(const settings_module sett_){
    QString param;
    QString val;
    switch (sett_) {
    case moduleuuid:
        param = "MODULEUUID";
        val = corespace::defaults::moduleuuid;
        break;
    case client_ip:
        param = "CLIENT_IP";
        val = corespace::defaults::client_ip;
        break;
    case server_ip:
        param = "SERVER_IP";
        val = corespace::defaults::server_ip;
        break;
    }
    m_settings->beginGroup("MODULEDATA");
    m_settings->setValue(param, val);
    m_settings->endGroup();
}

bool initializer::readSettings()
{
    qDebug()<<"initializer::readSettings()";
    m_path = (QCoreApplication::applicationDirPath()+"/config/parameters");
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
    QJsonObject dbauth = read_property_group("MODULEDATA");

<<<<<<< HEAD
initializer::~initializer()
{

}

void initializer::set_default_MODULE(const settings_module sett_){
    QString param;
    QString val;
    switch (sett_) {
    case moduleuuid:
        param = "MODULEUUID";
        val = corespace::defaults::moduleuuid;
        break;
    case client_ip:
        param = "CLIENT_IP";
        val = corespace::defaults::client_ip;
        break;
    case server_ip:
        param = "SERVER_IP";
        val = corespace::defaults::server_ip;
        break;
    }
    m_settings->beginGroup("MODULEUUID");
    m_settings->setValue(param, val);
    m_settings->endGroup();
}

bool initializer::readSettings()
{
    QJsonObject dbauth = read_property_group("MODULE");

    if(!dbauth.contains("MODULEUUID") || (dbauth.value("MODULEUUID").toString() == "")){
        set_default_MODULE(settings_module::moduleuuid);
    }

=======
    if(!dbauth.contains("MODULEUUID") || (dbauth.value("MODULEUUID").toString() == "")){
        set_default_MODULE(settings_module::moduleuuid);
    }

>>>>>>> core
    if(!dbauth.contains("CLIENT_IP") || (dbauth.value("CLIENT_IP").toString() == "")){
        set_default_MODULE(settings_module::client_ip);
    }

    if(!dbauth.contains("SERVER_IP") || (dbauth.value("SERVER_IP").toString() == "")){
        set_default_MODULE(settings_module::server_ip);
    }

    sync();
    return true;
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

<<<<<<< HEAD
initializer_back::initializer_back()
{

=======
initializer_back::initializer_back():initializer()
{
    qDebug()<<"initializer_back::initializer_back()";
>>>>>>> core
}

initializer_back::~initializer_back()
{

}

void initializer_back::set_default_DATABASE(const settings_db sett_)
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

bool initializer_back::readSettings()
{
<<<<<<< HEAD
=======
    qDebug()<<"initializer_back::readSettings()";
    initializer::readSettings();
    m_path = (QCoreApplication::applicationDirPath()+"/config/parameters");
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
>>>>>>> core
    bool m_static_ip_mode(true);
    QJsonObject dbauth = read_property_group("DATABASE");

    if(!dbauth.contains("HOSTNAME") || (dbauth.value("HOSTNAME").toString() == "")){
        set_default_DATABASE(settings_db::hostName);
    }
    if(!dbauth.contains("PORT") || (dbauth.value("PORT").toString() == "")){
        set_default_DATABASE(settings_db::port);
    }
    if(!dbauth.contains("DBNAME") || (dbauth.value("DBNAME").toString() == "")){
        set_default_DATABASE(settings_db::dbName);
    }
    if(!dbauth.contains("USERNAME") || (dbauth.value("USERNAME").toString() == "")){
        set_default_DATABASE(settings_db::userName);
    }
    if(!dbauth.contains("PASSWORD") || (dbauth.value("PASSWORD").toString() == "")){
        set_default_DATABASE(settings_db::password);
    }

    sync();
    return m_static_ip_mode;
}



