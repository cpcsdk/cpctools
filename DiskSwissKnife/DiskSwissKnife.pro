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
    ../Reloaded/src/core/dsk.cpp \
    QHexEdit/qhexedit.cpp \
    QHexEdit/qhexedit_p.cpp

HEADERS  += diskswissknife.h \
    ../Reloaded/src/core/dsk.h \
    QHexEdit/qhexedit.h \
    QHexEdit/qhexedit_p.h \
    ../Reloaded/src/core/fdc_old.h

FORMS    += diskswissknife.ui
