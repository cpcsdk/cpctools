noinst_LIBRARIES = libcapricewx.a libfoo.a

libcapricewx_a_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -I@top_srcdir@/src/cli -I@top_srcdir@/src/core -DDATA_PATH=\"$(pkgdatadir)//\"
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
CPCKeyDefs.h
#video.cpp \
#input.cpp
libcapricewx_a_LIBADD = libfoo.a 

# Ces deux fichiers ont besoin de CPCKeyDefs.h, mais pas les autres. On est obligé de bricoler parce que ces fichiers sont générés par wxFormBuilder...
libfoo_a_SOURCES = CapriceUI.cpp CapriceUI.h
libfoo_a_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -I@top_srcdir@/src/core -DDATA_PATH=\"$(pkgdatadir)//\" -include ../core/CPCKeyDefs.h 

#BUG : activated even in debug mode ...
#if RELEASE
#  libcapricewx_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
#endif
