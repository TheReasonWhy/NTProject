TEMPLATE = subdirs
SUBDIRS = corelib netlib backModule deviceModule proxyModule

corelib.subdir = libraries/corelib
netlib.subdir = libraries/netlib
backModule.subdir = modules/backModule
deviceModule.subdir = modules/deviceModule
proxyModule.subdir = modules/proxyModule

backModule.depends = corelib netlib
deviceModule.depends = corelib netlib
proxyModule.depends = corelib netlib

