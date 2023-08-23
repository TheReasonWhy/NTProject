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
#        $${CORE_DIR}/database.cpp \
#        $${CORE_DIR}/initializer.cpp \
#        $${CORE_DIR}/module.cpp \
#        $${CORE_DIR}/moduleexecutor.cpp \
#        $${CORE_DIR}/netlib.cpp \
#        $${CORE_DIR}/period.cpp \
#        $${CORE_DIR}/space.cpp \
#        $${CORE_DIR}/task.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#unix: LIBS += -Wl,--start-group
win32:CONFIG(release, debug|release): LIBS += -L$${OUT_ROOT_DIR}/libraries/corelib/release/ -lcorelib
else:win32:CONFIG(debug, debug|release): LIBS += -L$${OUT_ROOT_DIR}/libraries/corelib/debug/ -lcorelib
else:unix: LIBS += -L$${OUT_ROOT_DIR}/libraries/corelib/ -lcorelib
unix: PRE_TARGETDEPS += $${OUT_ROOT_DIR}/libraries/corelib/libcorelib.a

#HEADERS += \
#    $${CORE_DIR}/coreenums.h \
#    $${CORE_DIR}/database.h \
#    $${CORE_DIR}/enums.h \
#    $${CORE_DIR}/initializer.h \
#    $${CORE_DIR}/module.h \
#    $${CORE_DIR}/moduleexecutor.h \
#    $${CORE_DIR}/netlib.h \
#    $${CORE_DIR}/period.h \
#    $${CORE_DIR}/space.h \
#    $${CORE_DIR}/task.h
