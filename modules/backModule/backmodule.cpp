#include "backmodule.h"
#include "initializer.h"
#include "database.h"
#include "workerback.h"
#include <QSqlResult>

backModule::backModule(QObject *parent):module(parent){}

backModule::backModule(QSharedPointer<initializer_back> _initializer, QObject *parent):
    module(_initializer, QSharedPointer<workerback>( new workerback()), parent)
{
    qDebug()<<"backModule::backModule";
    //parseIniFile();
}

backModule::~backModule()
{

}

bool backModule::parseIniFile()
{
    qDebug()<<"backModule::parseIniFile";
    m_initializer.staticCast<initializer_back>()->readSettings();
    QJsonObject dbsett = m_initializer.get()->read_property_group("DATABASE");
    QJsonObject workersett = m_initializer.get()->read_property_group("MODULEDATA");
    qDebug()<<"dbsett " << dbsett;
    qDebug()<<"workersett " << workersett;

    m_current_settings.get()->insert("DATABASE", dbsett);
    m_current_settings.get()->insert("MODULEDATA", workersett);
    qDebug()<<"m_current_settings.get() " << m_current_settings.get();
    const QString host = m_current_settings.get()->value("DATABASE").toObject().value("HOSTNAME").toString();
    const int port = m_current_settings.get()->value("DATABASE").toObject().value("PORT").toString().toInt();
    const QString dbName = m_current_settings.get()->value("DATABASE").toObject().value("DBNAME").toString();
    const QString userName = m_current_settings.get()->value("DATABASE").toObject().value("USERNAME").toString();
    const QString password = m_current_settings.get()->value("DATABASE").toObject().value("PASSWORD").toString();

    const QString moduleuuid = m_current_settings.get()->value("MODULEDATA").toObject().value("MODULEUUID").toString();
    const QString serverip = m_current_settings.get()->value("MODULEDATA").toObject().value("SERVER_IP").toString();
    database::dbSettings sett(host,port,dbName,userName,password);
    m_db = new database(sett,this);
    if(m_db->start()){
        qDebug()<<"db started ";
        QSqlQuery s = m_db->prepareQuery(m_db->selectAllModulesStr(),false);
        if(m_db->executeQuery(s)){
            qDebug()<<"s executed ";
            while(s.next()){
                QUuid uid = s.value(0).toUuid();
                QUuid parent = s.value(1).toUuid();
                QString name = s.value(2).toString();
                qDebug()<< "uid " << uid << " parent "
                           << parent << " name "
                              << name;
            }
        }
        QSqlQuery s2 = m_db->prepareQuery(m_db->selectAllTasksStr(),false);
        if(m_db->executeQuery(s2)){
            qDebug()<<"s2 executed ";
            while(s2.next()){
                QUuid uid = s2.value(0).toUuid();
                QString name = s2.value(1).toString();
                bool is_done = s2.value(2).toBool();
                QUuid puid = s2.value(3).toUuid();
                QString pname = s2.value(4).toString();
                int svalue = s2.value(5).toInt();
                QUuid spaceuid = s2.value(6).toUuid();
                QString spacename = s2.value(7).toString();
                qDebug()<< "uid " << uid << " name "
                           << name << " is_done "
                              << is_done << " puid "
                                 << puid << " pname "
                                    << pname << " svalue "
                                       << svalue << " spaceuid "
                                          << spaceuid << " spacename "
                                            << spacename;
            }
        }

    }
/*
    достать дерево устройств и модулей
    QSqlQuery database::prepareQuery(const QString &sql, bool isSaveToPreparedMap)
*/

    m_worker = QSharedPointer<workerback>(new workerback(QUuid(moduleuuid), QList <QSharedPointer<workerBase>>()));
    return true;

}

bool backModule::start()
{
    qDebug()<<"backModule::start";
    m_worker.staticCast<workerback>().get()->start();
    return true;
}
