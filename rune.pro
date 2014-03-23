QT       += network testlib

QT       -= gui

TARGET = rune
TEMPLATE = lib

DEFINES += RUNE_LIBRARY

SOURCES += \
    engine.cpp

HEADERS +=\
        rune_global.h \
    engine.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
