QT       += network testlib

QT       -= gui

TARGET = rune
TEMPLATE = lib

DEFINES += RUNE_LIBRARY

SOURCES += \
    src/engine.cpp \
    src/entitiymanager.cpp \
    src/entity.cpp

HEADERS +=\
        include/rune/rune_global.h \
    include/rune/engine.h \
    include/rune/entitiymanager.h \
    include/rune/entity.h

INCLUDEPATH += include/

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibyaml-cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibyaml-cppd
else:unix: LIBS += -L$$PWD/lib/ -llibyaml-cpp

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
