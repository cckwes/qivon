TEMPLATE = lib
DESTDIR = $$PWD/../build
CONFIG += c++11

HEADERS += include/jpgio.h \
           include/pngio.h

SOURCES += src/jpgio.cpp \
           src/pngio.cpp

INCLUDEPATH += \
    $$PWD/include \
    $$PWD/../core/include

LIBS += -L$$PWD/../build -lcore

macx {
    INCLUDEPATH += /usr/local/include

    LIBS += -L/usr/local/lib -lpng -ljpeg
}
