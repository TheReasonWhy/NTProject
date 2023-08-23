#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
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
    void swap(database& src) noexcept;
    database(const dbSettings settings, QObject *parent = nullptr);
    database(const database& src);
    database(database&& src) noexcept;
    database& operator=(const database& src);
    bool start();
    void stop();
private:
    dbSettings m_settings;
};

#endif // DATABASE_H
