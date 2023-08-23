#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDriver>
#include <QTimer>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QUuid>
#include <QVariant>
#include <QMap>
#include <QtSql/QSqlField>


    struct dbSettings {
        dbSettings();
        void swap(dbSettings& src) noexcept;
        dbSettings(const QString _hostName,
                   const int _port,
                   const QString _dbName,
                   const QString _userName,
                   const QString _password);
        dbSettings(const dbSettings& src);
        dbSettings(dbSettings&& src) noexcept;
        dbSettings &operator=(const dbSettings & src);
        dbSettings& operator=(dbSettings&& src) noexcept;

        QString hostName;
        int port = 0;
        QString dbName;
        QString userName;
        QString password;
    };
class database : public QObject
{
    Q_OBJECT
public:
    database();
    bool start();
    void stop();
    void restartKeepAliveTimer();
    void setLastError(const QSqlError& error);
    void updateDatabaseState(bool isLastCommandFailed);
    void slotKeepAlive();
    void keepAlive(bool shouldCheckDb);
    void reconnect();
    bool openDb(QSqlDatabase& db);

    bool beginTransaction();
    bool beginTransactionSerializable();
    bool commitTransaction();
    bool rollbackTransaction();

    QSqlQuery executeCommand(const QString& command);
    bool executeSimpleCommand(const QString& command);
    bool executeQuery(QSqlQuery& query);
    static QString paramToString(const QVariant& v);
    QSqlQuery prepareQuery(const QString& sql, bool isSaveToPreparedMap);

    bool prepareQuery(QSqlQuery& query, const QString& queryStr);

signals:
    void signalConnectionLost();
    void signalReconnected();
private:
    QSqlDatabase _db;
    bool is_connected = false;
    bool is_connecting = false;
    bool is_error = false;
    bool m_transaction = false;
    int m_transactionLevel = 0;
    QUuid m_transactionUuid;
    QString m_lastError;
    dbSettings db_settings;
    QTimer *m_timer;
    QMap<QString, QSqlQuery> m_preparedQueries;
};


#endif // DATABASE_H
