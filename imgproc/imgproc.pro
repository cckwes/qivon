TEMPLATE = lib
DESTDIR = $$PWD/../build
CONFIG += c++11

HEADERS += include/color.h \
           include/filter.h \
           include/geometry.h

SOURCES += src/color.cpp \
           src/filter.cpp \
           src/geometry.cpp

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/../core/include

LIBS += \
    -L$$PWD/../build -lcore
