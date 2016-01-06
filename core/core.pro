TEMPLATE = lib
DESTDIR = $$PWD/../build
CONFIG += c++11

HEADERS += include/image.h \
           include/qivon_config.h \
           include/util.h

SOURCES += src/image.cpp

INCLUDEPATH += \
    $$PWD/include
