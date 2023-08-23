QT -= gui

TEMPLATE = lib
CONFIG += c++20 core staticlib console sql

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CORE_DIR = $${PWD}/../../code/core
INCLUDEPATH += $${CORE_DIR}
DEPENDPATH += $${CORE_DIR}

SOURCES += \
    $${CORE_DIR}/task.cpp \
    $${CORE_DIR}/workerback.cpp \
    $${CORE_DIR}/workerbase.cpp \
    $${CORE_DIR}/workerfinal.cpp \
    $${CORE_DIR}/workerproxy.cpp \
    $${CORE_DIR}/database.cpp \
    $${CORE_DIR}/space.cpp \
    $${CORE_DIR}/period.cpp \
    $${CORE_DIR}/module.cpp \
    $${CORE_DIR}/moduleexecutor.cpp \
    $${CORE_DIR}/initializer.cpp

HEADERS += \
    $${CORE_DIR}/coreenums.h \
    $${CORE_DIR}/task.h \
    $${CORE_DIR}/workerback.h \
    $${CORE_DIR}/workerbase.h \
    $${CORE_DIR}/workerfinal.h \
    $${CORE_DIR}/workerproxy.h \
    $${CORE_DIR}/database.h \
    $${CORE_DIR}/space.h \
    $${CORE_DIR}/period.h \
    $${CORE_DIR}/module.h \
    $${CORE_DIR}/moduleexecutor.h \
    $${CORE_DIR}/initializer.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
