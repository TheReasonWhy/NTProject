#ifndef COREENUMS_H
#define COREENUMS_H
#include <QString>
namespace corespace{
    enum deviceType{
        backModule,
        proxyModule,
        frontModule,
    };
    enum taskType{
        takePhoto
    };

    namespace defaults{
        const QString moduleuuid = "eb4ab7e9-ed0e-463a-8d7e-37009015f3e6";
        const QString client_ip = "127.0.0.1";
        const QString server_ip = "127.0.0.1";
        const QString hostName = "localhost";
        const QString port = QString::number(5432);
        const QString dbName = "testDb";
        const QString userName = "postgres";
        const QString password = "postgres";
    }
};
#endif // COREENUMS_H
