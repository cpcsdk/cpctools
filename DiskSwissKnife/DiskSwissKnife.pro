#-------------------------------------------------
#
# Project created by QtCreator 2011-02-09T21:06:26
#
#-------------------------------------------------

QT       += core gui

TARGET = DiskSwissKnife
TEMPLATE = app


SOURCES += main.cpp\
        diskswissknife.cpp \
    ../iDSK/src/GestDsk.cpp \
    ../iDSK/src/endianPPC.cpp \
    ../iDSK/src/Outils.cpp

HEADERS  += diskswissknife.h \
    ../iDSK/src/GestDsk.h \
    ../iDSK/src/endianPPC.h \
    ../iDSK/src/Outils.h

FORMS    += diskswissknife.ui
