#ifndef BACKMODULE_H
#define BACKMODULE_H

#include <QObject>
#include "module.h"

class database;
class initializer_back;

class backModule : public module
{
    Q_OBJECT
public:
    backModule(QObject *parent = nullptr);
    backModule(QSharedPointer<initializer_back> _initializer, QObject *parent = nullptr);
    ~backModule();
    bool parseIniFile()override;
    virtual bool start()override;

private:
    database *m_db;
};

#endif // BACKMODULE_H
