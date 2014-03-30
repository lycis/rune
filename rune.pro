QT       += network testlib

QT       -= gui

TARGET = rune
TEMPLATE = lib

DEFINES += RUNE_LIBRARY

SOURCES += \
    src/engine.cpp \
    src/entity.cpp \
    src/entitymanager.cpp \
    src/worldmap.cpp

HEADERS +=\
        include/rune/rune_global.h \
    include/rune/engine.h \
    include/rune/entity.h \
    include/rune/entitymanager.h \
    include/rune/worldmap.h

INCLUDEPATH += include/

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib -llibyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib -llibyaml-cppd
else:unix: -llibyaml-cpp

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
