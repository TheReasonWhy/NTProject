QT -= gui

CONFIG += c++20 console sql
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CORE_DIR = $${PWD}/../../code/core
INCLUDEPATH += $${CORE_DIR}
ROOT_DIR = $${PWD}/../../
OUT_ROOT_DIR = $$OUT_PWD/../..


SOURCES += \
        ../../code/core/database.cpp \
        ../../code/core/initializer.cpp \
        ../../code/core/module.cpp \
        ../../code/core/moduleexecutor.cpp \
        ../../code/core/netlib.cpp \
        ../../code/core/period.cpp \
        ../../code/core/space.cpp \
        ../../code/core/task.cpp \
        backmodule.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../../code/core/coreenums.h \
    ../../code/core/database.h \
    ../../code/core/enums.h \
    ../../code/core/initializer.h \
    ../../code/core/module.h \
    ../../code/core/moduleexecutor.h \
    ../../code/core/netlib.h \
    ../../code/core/period.h \
    ../../code/core/space.h \
    ../../code/core/task.h \
    backmodule.h
