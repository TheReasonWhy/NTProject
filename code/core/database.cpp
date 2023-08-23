#include "database.h"



database::database():m_timer(new QTimer(this)){}

bool database::start(){
    is_connected = true;
    _db = QSqlDatabase::addDatabase("QPSQL");
    if (openDb(_db)) {
        qDebug()<<"database opened";
    } else {
        qDebug()<<"database ! opened";
        return false;
    }
    bool isOk = true;
    while (isOk) {
        QSqlQuery query = executeCommand("SELECT version();");
        if (query.next() && query.record().count()) {
            restartKeepAliveTimer();
            is_connecting = false;
            return true;
        }
        break;
    }

    stop();
    return false;
}

void database::stop(){
    is_connecting = false;
    m_timer->stop();

    if (_db.isOpen()){
        qDebug()<<"database : closed";
    }
    QList<QSqlDatabase*> bases;
    bases << &_db;
    for (QSqlDatabase* db : bases) {
        QString name = db->connectionName();
        *db = QSqlDatabase();
        QSqlDatabase::removeDatabase(name);
    }
}

void database::restartKeepAliveTimer()
{
    m_timer->start();
}

QSqlQuery database::executeCommand(const QString &command) {

    QSqlQuery query(_db);
    bool result = query.exec(command);
    QSqlError error = query.lastError();
    setLastError(error);
    updateDatabaseState(!result);

    return query;
}

bool database::executeSimpleCommand(const QString &command) {
    QSqlQuery query(_db);
    bool result = query.exec(command);
    setLastError(query.lastError());
    if (query.lastError().isValid()){
        qDebug()<<"Can't execute query: " + query.lastError().text();
    }
    updateDatabaseState(!result);

    return result;
}

bool database::executeQuery(QSqlQuery &query) {
    QString debugQuery = query.executedQuery();

    QVariantList lst = query.boundValues();

    if (!lst.isEmpty())
        debugQuery += ";";
    int iter = 0;
    for (auto it = lst.constBegin(); it != lst.constEnd(); ++it) {
        QVariant value = it->data();
        debugQuery += " ";
        debugQuery += QString::number(iter++) + "=" + paramToString(value);
    }

    qDebug()<<debugQuery;
    bool result = query.exec();
    qDebug()<< "numRowsAffected " << query.numRowsAffected();

    setLastError(query.lastError());
    if (query.lastError().isValid()) {
        qDebug()<<"Can't execute query: "<<debugQuery;
    }
    updateDatabaseState(!result);
    return result;
}

QString database::paramToString(const QVariant &v) {
    return v.toString();
}

QSqlQuery database::prepareQuery(const QString &sql, bool isSaveToPreparedMap) {

    if (isSaveToPreparedMap) {
        is_error = false;
        auto it = m_preparedQueries.find(sql);
        if (it != m_preparedQueries.end())
            return it.value();
    }

    QSqlQuery query(_db);
    bool success = query.prepare(sql);
    setLastError(query.lastError());
    if (!success) {
        qDebug()<<"Can't prepare query: " << query.lastError().text();
    }
    updateDatabaseState(!success);

    if (isSaveToPreparedMap) {
        if (success)
            m_preparedQueries.insert(sql, query);
    }

    return success ? query : QSqlQuery();
}

bool database::prepareQuery(QSqlQuery &query, const QString &queryStr) {
    bool success = query.prepare(queryStr);
    setLastError(query.lastError());
    if (!success) {
        qDebug()<<queryStr;
    }
    return success;
}

void database::setLastError(const QSqlError &error)
{
    m_lastError = error.text().trimmed();
}

void database::updateDatabaseState(bool isLastCommandFailed)
{
    is_error = isLastCommandFailed;
    keepAlive(isLastCommandFailed);
}

void database::slotKeepAlive()
{
    QVariant h = _db.driver()->handle();
    keepAlive(true);
}

void database::keepAlive(bool shouldCheckDb)
{
    if (is_connecting)
        return;

    if (!shouldCheckDb || _db.isOpen()) {
        restartKeepAliveTimer();
    } else {
        m_timer->stop();
        is_connecting = true;
        qDebug()<<"connection lost";
        emit signalConnectionLost();
        QTimer::singleShot(0, this, &database::reconnect);
    }
}

void database::reconnect()
{
    if (!is_connecting)
        return;

    while (openDb(_db)) {
        QStringList notifyList = _db.driver()->subscribedToNotifications();
        if (!notifyList.isEmpty()) {
            for (const QString& notifyName : notifyList)
                _db.driver()->unsubscribeFromNotification(notifyName);
            for (const QString& notifyName : notifyList)
                _db.driver()->subscribeToNotification(notifyName);
            if (!_db.isOpen())
                break; //опять упало во время подписки
        }
        is_connecting = false;
        restartKeepAliveTimer();
        qDebug()<<"reopened";
        emit signalReconnected();
        return;
    }

    QTimer::singleShot(5000, this, &database::reconnect);
}

bool database::openDb(QSqlDatabase &db){
    const int connectionTimeout = 10;
    QString options;
    options = "connect_timeout=" + QString::number(connectionTimeout);

    db.setHostName(db_settings.hostName);
    db.setPort(db_settings.port);
    db.setDatabaseName(db_settings.dbName);
    db.setUserName(db_settings.userName);
    db.setPassword(db_settings.password);
    db.setConnectOptions(options);
    return db.open();

}

bool database::beginTransaction() {
    if (!m_transaction) {
        qDebug()<<"beginTransaction";
        bool success = _db.transaction();
        setLastError(success ? QSqlError() : _db.lastError());
        if (success) {
            m_transaction = true;
            m_transactionLevel = 1;
            m_transactionUuid = QUuid::createUuid();
        } else {
            qDebug()<<"Can't start transaction";
            updateDatabaseState(true);
        }
        return success;
    }

    bool success = executeSimpleCommand(QStringLiteral("SAVEPOINT SAVEPOINT_%1").arg(QString::number(m_transactionLevel)));
    if (success)
        ++m_transactionLevel;
    return success;
}

bool database::beginTransactionSerializable() {
    bool val = beginTransaction();
    if (!val)
        return false;
    executeSimpleCommand("SET TRANSACTION ISOLATION LEVEL SERIALIZABLE READ ONLY");
    return true;
}

bool database::commitTransaction() {
    if (!m_transactionLevel) {
        qDebug()<<"Not in transaction";
        return false;
    }
    if (m_transactionLevel == 1) {
        m_transaction = false;
        m_transactionLevel = 0;
        m_transactionUuid = QUuid();
        bool success = _db.commit();
        setLastError(success ? QSqlError() : _db.lastError());
        if (!m_lastError.isEmpty())
            qDebug()<<m_lastError;
        if (!success) {
            qDebug()<<"Can't commit transaction";
            updateDatabaseState(true);
        }
        return success;
    }
    --m_transactionLevel;
    return true;
}

bool database::rollbackTransaction() {
    if (!m_transactionLevel) {
        qDebug()<<"Not in transaction";
        return false;
    }
    if (m_transactionLevel == 1) {
        qDebug()<<"rollbackTransaction";
        m_transaction = false;
        m_transactionLevel = 0;
        bool success = _db.rollback();
        if (!success){
            qDebug()<<"can't ollbackTransaction";
        }
        updateDatabaseState(!success);
        return success;
    }

    --m_transactionLevel;
    return executeSimpleCommand(QStringLiteral("ROLLBACK TO SAVEPOINT_%1").arg(QString::number(m_transactionLevel)));
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
