#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <QObject>
#include <QSettings>
#include <QJsonObject>
/*
    QString hostName;
    int port = 0;
    QString dbName;
    QString userName;
    QString password;

 */

class initializer : public QObject
{
    Q_OBJECT
public:
    enum settings{
        hostName,
        port,
        dbName,
        userName,
        password
    };
    initializer();
    ~initializer();

    void set_default_DATABASE(const settings sett_);

    void sync();
    bool readSettings();
    QJsonObject read_property_group(const QString code);

private:
    QString m_path;
    QSettings *m_settings;
};

#endif // INITIALIZER_H





















