#ifndef SERIALIZABLEK_H
#define SERIALIZABLEK_H
#include <QDataStream>
#include <QUuid>
#include <QList>
#include <QMap>
#include "coreenums.h"
#include "period.h"


using namespace corespace;
class point3D{
public:
    point3D(const double x_, const double y_,const double z_):x(x_),y(y_),z(z_){};

    friend QDataStream & operator << (QDataStream & out, const point3D &u){
        out << u.x << u.y << u.z;
        return out;
    }
    friend QDataStream & operator >> (QDataStream & out, point3D &u){
        out >> u.x >> u.y >> u.z;
        return out;
    }

    double x;
    double y;
    double z;
};

class task{
public:
    task(const taskType type, const point3D point):m_type(type),m_point(point){

    };
    friend QDataStream & operator << (QDataStream & out, const task &u){
        out << u.m_uid << u.m_type << u.m_point;
        return out;
    }
    friend QDataStream & operator >> (QDataStream & out, task &u){
        out >> u.m_uid >> u.m_type >> u.m_point;
        return out;
    }
    QUuid m_uid;
    taskType m_type;
    point3D m_point;
};



class serializable
{
public:
    serializable();
    deviceType m_type;
    QUuid m_uid;
    point3D m_point;
    void dispense(){

    };
    QMap<QUuid, QList<task>> m_map;
};

























#endif // SERIALIZABLEK_H
