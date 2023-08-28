#include "database.h"

#include "postgresql/libpq-fe.h"


database::database(const dbSettings sett, QObject *parent):QObject(parent), m_db_settings(sett),m_timer(new QTimer(this)){

    qDebug()<<"database::database()";
}

database::~database()
{

}

bool database::start(){
    qDebug()<<"database::start()";
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
                break;
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
    db.setHostName(m_db_settings.hostName);
    db.setPort(m_db_settings.port);
    db.setDatabaseName(m_db_settings.dbName);
    db.setUserName(m_db_settings.userName);
    db.setPassword(m_db_settings.password);
    db.setConnectOptions(options);
    bool res = db.open();
    qDebug()<<"db.lastError().text() " << db.lastError().text() << " m_db_settings " << m_db_settings.port;
    return res;

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

database::dbSettings::dbSettings(){}

void database::dbSettings::swap(dbSettings &src) noexcept{
    this->hostName = src.hostName;
    this->port = src.port;
    this->dbName = src.dbName;
    this->userName = src.userName;
    this->password = src.password;
}

database::dbSettings::dbSettings(const QString _hostName, const int _port, const QString _dbName, const QString _userName, const QString _password):
    hostName(_hostName),
    port(_port),
    dbName(_dbName),
    userName(_userName),
    password(_password)
{
    qDebug()<<"dbSettings::dbSettings "<<_hostName<<" "<<_port<<" "<<_dbName<<" "<<_userName<<" "<<_password;
}

database::dbSettings::dbSettings(const dbSettings &src){
    this->hostName = src.hostName;
    this->port = src.port;
    this->dbName = src.dbName;
    this->userName = src.userName;
    this->password = src.password;
}

database::dbSettings::dbSettings(dbSettings &&src) noexcept{
    dbSettings tmp(std::move(src));
    swap(tmp);
}

database::dbSettings &database::dbSettings::operator=(dbSettings &&src) noexcept{
    swap(src);
    return *this;
}

database::dbSettings &database::dbSettings::operator=(const dbSettings &src)
{
    this->hostName = src.hostName;
    this->port = src.port;
    this->dbName = src.dbName;
    this->userName = src.userName;
    this->password = src.password;
    return *this;
}










QString database::selectAllModulesStr(){
    QString query = "with recursive r AS (select uid,parent,name from modules "
                    "where modules.parent = 'eb4ab7e9-ed0e-463a-8d7e-37009015f3e6' "
                    "union select modules.uid,modules.parent,modules.name "
                    "from modules JOIN r ON modules.parent = r.uid) "
                    "SELECT r.uid, r.parent, r.name FROM r;";
    return query;
}

QString database::selectAllTasksStr(){
    QString query = "SELECT tasks.uid, tasks.name, tasks.is_done, tasks.point_uid as Puid, "
                    "P.name as Pname, "
                    "SD.value as Svalue, "
                    "SD.space_uid as Spaceuid, "
                    "S.name as Spacename FROM tasks "
                    "INNER join points P ON P.uid = tasks.point_uid "
                    "INNER join space_data SD ON P.uid = SD.point_uid "
                    "INNER join spaces S ON SD.space_uid = S.uid "
                    "where tasks.is_done = false;";
    return query;
}
/*

with recursive r AS (select uid,parent,name from modules
    where modules.parent = 'eb4ab7e9-ed0e-463a-8d7e-37009015f3e6'
    union select modules.uid,modules.parent,modules.name
        from modules JOIN r ON modules.parent = r.uid)
    SELECT r.uid, r.parent, r.name FROM r;

///////////
///
SELECT tasks.uid, tasks.name, tasks.is_done, tasks.point_uid as Puid, points.name as Pname, space_data.value as Svalue, space_data.space_uid as Spaceuid, spaces.name as Spacename FROM tasks
    LEFT join points ON points.uid = Puid
    LEFT join space_data ON space_data.point_uid = points.uid
    LEFT join spaces ON Spaceuid = spaces.uid
    where tasks.is_done = false;

/////////
///
    with ob_id as (
      insert into objects (uid) values (default)
      returning uid
    )
    INSERT INTO points (uid, name) VALUES ((select uid from ob_id),'point20')
    RETURNING uid

/////////
///
    with ob_id as (
      insert into objects (uid) values (default)
      returning uid
    )
INSERT INTO tasks (uid, point_uid, name, is_done) VALUES ((select uid from ob_id),'58470c75-3e23-466e-b88d-0c88c635c421','point17', false);
INSERT INTO tasks (uid, point_uid, name, is_done)
VALUES ((select uid from ob_id),'00dba189-995b-4f70-b022-e74f4740931d','point18', false);
INSERT INTO tasks (uid, point_uid, name, is_done)
VALUES ((select uid from ob_id),'00dba189-995b-4f70-b022-e74f4740931d','point19', false);
INSERT INTO tasks (uid, point_uid, name, is_done) V
ALUES ((select uid from ob_id),'00dba189-995b-4f70-b022-e74f4740931d','point20', false);
58470c75-3e23-466e-b88d-0c88c635c421
00dba189-995b-4f70-b022-e74f4740931d
79457c4f-7b9e-4e67-801e-6279a30fce76
a1a211d0-30ca-4223-ba6f-857ed4a6e25b

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '0f771a7f-8113-4a04-82df-ef982c15903a',
  '58470c75-3e23-466e-b88d-0c88c635c421',
  2
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '1d027d76-c529-48f0-9a41-a14e94fec20f',
  '58470c75-3e23-466e-b88d-0c88c635c421',
  3
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '8696b1c0-7520-476f-9dd7-f00fb0328708',
  '58470c75-3e23-466e-b88d-0c88c635c421',
  4
);


with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '0f771a7f-8113-4a04-82df-ef982c15903a',
  '00dba189-995b-4f70-b022-e74f4740931d',
  22
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '1d027d76-c529-48f0-9a41-a14e94fec20f',
  '00dba189-995b-4f70-b022-e74f4740931d',
  33
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '8696b1c0-7520-476f-9dd7-f00fb0328708',
  '00dba189-995b-4f70-b022-e74f4740931d',
  44
);


with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '0f771a7f-8113-4a04-82df-ef982c15903a',
  '79457c4f-7b9e-4e67-801e-6279a30fce76',
  222
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '1d027d76-c529-48f0-9a41-a14e94fec20f',
  '79457c4f-7b9e-4e67-801e-6279a30fce76',
  333
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '8696b1c0-7520-476f-9dd7-f00fb0328708',
  '79457c4f-7b9e-4e67-801e-6279a30fce76',
  444
);


with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '0f771a7f-8113-4a04-82df-ef982c15903a',
  'a1a211d0-30ca-4223-ba6f-857ed4a6e25b',
  2222
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '1d027d76-c529-48f0-9a41-a14e94fec20f',
  'a1a211d0-30ca-4223-ba6f-857ed4a6e25b',
  3333
);

with ob_id as (
  insert into objects (uid) values (default)
  returning uid
)
insert into space_data (uid, space_uid, point_uid, value)
values
(
  (select uid from ob_id),
  '8696b1c0-7520-476f-9dd7-f00fb0328708',
  'a1a211d0-30ca-4223-ba6f-857ed4a6e25b',
  4444
);


fun fetchTemplateTreeRecursive(templ: TaskVariantsPair, depId: Int): ArrayList<TaskVariantsPair>{
                var result = ArrayList<TaskVariantsPair>()
                DbHelper.instance.getConnection().use { conn->
                    templ?.let {
                        var parent = templ.task?.template_id

                        var orderBy = "ORDER BY id "
                        var where = " WHERE task_templates.task_group_id=? AND task_templates.dep_id=?"
                        var where2 = " JOIN r ON task_templates.task_group_id = r.id "
                        var leftJoin = "LEFT JOIN tasks_variants_templates ON r.id = tasks_variants_templates.task_id "
                        parent?.let{
                            conn.prepareStatement("WITH RECURSIVE r AS (" +
                                    " SELECT id, task_name, task_type, flag, task_group_id, incoming_data, dep_id, minutes_to_work, kit FROM task_templates $where " +
                                    "UNION SELECT task_templates.id, " +
                                    "task_templates.task_name, task_templates.task_type, task_templates.flag, " +
                                    "task_templates.task_group_id, task_templates.incoming_data, " +
                                    "task_templates.dep_id, task_templates.minutes_to_work, task_templates.kit " +
                                    "FROM task_templates " +
                                    " $where2 ) SELECT  r.id, " +
                                    "r.task_name, r.task_type, r.flag, " +
                                    "r.task_group_id, r.incoming_data, " +
                                    "r.dep_id, r.minutes_to_work, r.kit, " +
                                    "tasks_variants_templates.id AS variant_id, " +
                                    "tasks_variants_templates.task_id, tasks_variants_templates.description, " +
                                    "tasks_variants_templates.result_status, tasks_variants_templates.next_task_id, " +
                                    "tasks_variants_templates.counter, " +
                                    "tasks_variants_templates.data " +
                                    "FROM r $leftJoin $orderBy ").use { stmt->
                                stmt.setInt(1, parent)
                                stmt.setInt(2, depId)
                                stmt.executeQuery().use { rs->
                                    var idd: Int? = null
                                    while (rs.next()){
                                        if(idd != rs.getInt("task_id")){
                                            if(!result.isEmpty()){
                                                TaskVariant.Companion.setDefaultVariants(result?.last())
                                            }
                                            //TaskVariantsPair(taskservice.getNewTaskFromTemplate(
                                            result.add(TaskVariantsPair(taskservice.getNewTaskFromTemplate((TaskTemplate(rs)))))
                                            idd=rs.getInt("task_id")
                                            result.last()?.variants?.add(TaskVariant(VariantTemplate(rs, result.last())))
                                        }else{
                                            result.last()?.variants?.add(TaskVariant(VariantTemplate(rs, result.last())))
                                        }
                                    }
                                    if(!result.isEmpty()){
                                        TaskVariant.Companion.setDefaultVariants(result?.last())
                                    }
                                    buildTree(templ, result)
//                                    fixSequential(b)
                                }
                            }
                        }
                    }
                }
                return result
            }




*/
