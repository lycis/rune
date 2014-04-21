QT       += network testlib script

QT       -= gui

TARGET = rune
TEMPLATE = lib

DEFINES += RUNE_LIBRARY RUNE_CIRCLE_ALGORITHM_FILTEREDSQUARE
#RUNE_CIRCLE_ALGORITHM_FILTEREDSQUARE
#RUNE_CIRCLE_ALGORITHM_MIDPOINT

SOURCES += \
    src/engine.cpp \
    src/entity.cpp \
    src/worldmap.cpp \
    src/rune_error.cpp \
    src/scriptinterpreter.cpp

HEADERS +=\
        include/rune/rune_global.h \
    include/rune/engine.h \
    include/rune/entity.h \
    include/rune/worldmap.h \
    include/rune/rune_error.h \
    include/rune/scriptinterpreter.h

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
