#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <QObject>
#include <QSettings>
#include <QJsonObject>

class initializer : public QObject
{
    Q_OBJECT
public:
    enum settings_module{
        moduleuuid,
        client_ip,
        server_ip
    };
    initializer();
    virtual ~initializer();
    void set_default_MODULE(const settings_module sett_);

    void sync();
    bool readSettings();
    QJsonObject read_property_group(const QString code);

protected:
    QString m_path;
    QSettings *m_settings;
};

class initializer_back : public initializer
{
    Q_OBJECT
public:
    enum settings_db{
        hostName,
        port,
        dbName,
        userName,
        password
    };
    initializer_back();
    ~initializer_back()override;

    void set_default_DATABASE(const settings_db sett_);
    bool readSettings();
};

#endif // INITIALIZER_H





















