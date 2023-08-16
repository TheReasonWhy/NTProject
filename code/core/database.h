#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>

class database : public QObject
{
    Q_OBJECT
public:
    database();;
    bool start();
    void stop();
private:

};

#endif // DATABASE_H
