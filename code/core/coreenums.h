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
        const QString hostName = "localhost";
        const QString port = QString::number(5432);
        const QString dbName = "testDb";
        const QString userName = "postgres";
        const QString password = "postgres";
    }
};
#endif // COREENUMS_H
