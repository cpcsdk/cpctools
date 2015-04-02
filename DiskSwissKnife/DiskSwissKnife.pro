#-------------------------------------------------
#
# Project created by QtCreator 2011-02-09T21:06:26
#
#-------------------------------------------------

QT       += widgets

TARGET = DiskSwissKnife
TEMPLATE = app


SOURCES += main.cpp\
        diskswissknife.cpp \
    ../Reloaded/src/core/dsk.cpp \
    qhexedit/src/qhexedit.cpp \
    qhexedit/src/commands.cpp \
    qhexedit/src/xbytearray.cpp \
    qhexedit/src/qhexedit_p.cpp

HEADERS  += diskswissknife.h \
    ../Reloaded/src/core/dsk.h \
    qhexedit/src/qhexedit.h \
    qhexedit/src/qhexedit_p.h \
    ../Reloaded/src/core/fdc_old.h

FORMS    += diskswissknife.ui
