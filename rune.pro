QT       += network testlib

QT       -= gui

TARGET = rune
TEMPLATE = lib

DEFINES += RUNE_LIBRARY

SOURCES += \
    src/engine.cpp

HEADERS +=\
        include/rune/rune_global.h \
    include/rune/engine.h

INCLUDEPATH += include/

unix {
    target.path = /usr/lib
    INSTALLS += target
}
