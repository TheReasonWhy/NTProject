QT -= gui

QT += core network sql
CONFIG += c++20 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CORE_DIR = $${PWD}/../../code/core
INCLUDEPATH += $${CORE_DIR}
ROOT_DIR = $${PWD}/../../
OUT_ROOT_DIR = $$OUT_PWD/../..

SOURCES += \
        backmodule.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


unix: LIBS += -Wl,--start-group
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_ROOT_DIR}/libraries/corelib/release/ -lcorelib
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_ROOT_DIR}/libraries/corelib/debug/ -lcorelib
else:unix: LIBS += -L$${OUT_ROOT_DIR}/libraries/corelib/ -lcorelib
unix: PRE_TARGETDEPS += $${OUT_ROOT_DIR}/libraries/corelib/libcorelib.a
unix: LIBS += -Wl,--end-group

HEADERS += \
    backmodule.h
unix:    LIBS += -lpq -lcrypto

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/release/ -lcorelib
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/debug/ -lcorelib
#else:unix: LIBS += -L$$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/ -lcorelib

#INCLUDEPATH += $$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib
#DEPENDPATH += $$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/release/libcorelib.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/debug/libcorelib.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/release/corelib.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/debug/corelib.lib
#else:unix: PRE_TARGETDEPS += $$PWD/../../../build-NTProject-Desktop-Release/libraries/corelib/libcorelib.a
