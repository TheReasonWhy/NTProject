#ifndef BACKMODULE_H
#define BACKMODULE_H

#include <QObject>
#include "module.h"

class database;

class backModule : public module
{
    Q_OBJECT
public:
    backModule(QObject *parent = nullptr);
    backModule(QSharedPointer<initializer> _initializer, QObject *parent = nullptr);
    ~backModule();
    bool parseIniFile()override;

private:
    //database *m_db;
};

#endif // BACKMODULE_H
