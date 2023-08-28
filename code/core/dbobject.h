#ifndef DBOBJECT_H
#define DBOBJECT_H

#include <QObject>
#include <QString>
#include <QList>

/*
    dbobject
        QString name
        QList<dbobject> connected_tables

    table_object : dbobject
        QString name
        table_type type
        QList<field_object> fields

    module_object : table_object

    field_object
        QString name
        field_type type
    field_type
        uid
        string
        int
        timestamp
    table_type
        uid
        string
        int
        timestamp

*/

template<typename T>
class field_object{

public:
    field_object();

protected:
    QString name;
    T value;
};

class dbobject{

public:
    dbobject();

protected:
    QString name;
    QList<dbobject> connected_tables;//данные из других таблиц, ссылающихся на эту
    QList<dbobject> link_tables;//данные из других таблиц, на к-рые эта ссылается
};

#endif // DBOBJECT_H
