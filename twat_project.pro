#-------------------------------------------------
#
# Project created by QtCreator 2013-07-27T11:36:57
#
#-------------------------------------------------

QT       += core gui \
            printsupport #qcustomplot
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = twat_project
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++0x
OBJECTS_DIR = ./obj
INCLUDEPATH +=  src \
                ..
LIBS += -lz #zlib


PNGLITE_SRC += ../pnglite/pnglite.c
PNGLITE_HDR += ../pnglite/pnglite.h

client{
Q_CUSTOMPLOT_SRC += ../qcustomplot/qcustomplot.cpp
Q_CUSTOMPLOT_HDR += ../qcustomplot/qcustomplot.h

SOURCES +=  $$system("find ./src -name '*.cpp'") \
            $$Q_CUSTOMPLOT_SRC \
            $$PNGLITE_SRC
HEADERS +=  $$system("find ./src -name '*.h'") \
            $$Q_CUSTOMPLOT_HDR \
            $$PNGLITE_HDR
FORMS   +=  $$system("find ./src -name '*.ui'")
RESOURCES += data/ressources.qrc
}

test_map_access{
SOURCES +=  tests/map_access.cpp \
            src/base/system.cpp \
            src/core/shared/io_file.cpp \
            src/core/shared/datafile.cpp \
            src/core/shared/graphics.cpp \
            src/core/tw_stripped/stuff.cpp \
            src/core/tools/tw/datafiles/map.cpp \
            src/core/tools/tw/datafiles/map_datafile.cpp \
            $$PNGLITE_SRC

HEADERS +=  src/base/system.h \
            src/core/shared/io_file.h \
            src/core/shared/datafile.h \
            src/core/shared/graphics.h \
            src/core/tw_stripped/stuff.h \
            src/core/tools/tw/datafiles/map.h \
            src/core/tools/tw/datafiles/map_datafile.h \
            $$PNG_LITE_HDR
}

HEADERS += \
    src/core/tw_stripped/stuff.h
