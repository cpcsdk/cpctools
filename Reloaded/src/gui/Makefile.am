noinst_LIBRARIES = libcapricewx.a libfoo.a

libcapricewx_a_CPPFLAGS = $(WX_CPPFLAGS) \
                          -I@top_srcdir@/src/core \
                          -I@top_srcdir@/src/asm 
libcapricewx_a_SOURCES = \
CapriceAboutImpl.cpp \
CapriceAboutImpl.h\
CapriceApp.cpp\
CapriceApp.h\
CapriceRegistersDialogImpl.cpp\
CapriceRegistersDialogImpl.h\
MemoryImpl.cpp\
MemoryImpl.h\
CapriceWindowImpl.cpp\
CapriceWindowImpl.h\
CapriceInputSettingsImpl.h\
CapriceInputSettingsImpl.cpp\
CPCKeyDefs.h \
WXEmulator.h \
WXEmulator.cpp \
WXVideo.cpp \
WXLog.h

libcapricewx_a_LIBADD = libfoo.a

# Ces deux fichiers ont besoin de CPCKeyDefs.h, mais pas les autres. On est obligé de bricoler parce que ces fichiers sont générés par wxFormBuilder...
libfoo_a_SOURCES = CapriceUI.cpp CapriceUI.h
libfoo_a_CPPFLAGS = $(WX_CPPFLAGS) -I@top_srcdir@/src/core -DDATA_PATH=\"$(pkgdatadir)//\" -include ../core/CPCKeyDefs.h

if WITH_IDE
  DIST_SUBDIRS = IDE
  SUBDIRS = IDE
  libcapricewx_a_SOURCES += \
  CapriceIDE.h \
  CapriceIDE.cpp
  libcapricewx_a_CPPFLAGS += -I@top_srcdir@/src/gui/IDE/include/
endif

if CLI
  libcapricewx_a_CPPFLAGS += -I@top_srcdir@/src/cli
endif

if RELEASE
  libcapricewx_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s
endif
