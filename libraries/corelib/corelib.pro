QT -= gui

TEMPLATE = lib
CONFIG += staticlib console sql

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CORE_DIR = $${PWD}/../../code/core
INCLUDEPATH += $${CORE_DIR}

SOURCES += \
    $${CORE_DIR}/corelib.cpp

HEADERS += \
    $${CORE_DIR}/corelib.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
