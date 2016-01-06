TEMPLATE = app
DESTDIR = $$PWD/../build
CONFIG += c++11

SOURCES += src/main.cpp

INCLUDEPATH += \
    $$PWD/../core/include \
    $$PWD/../io/include \
    $$PWD/../imgproc/include

LIBS += \
    -L$$PWD/../build -lcore -lio -limgproc
