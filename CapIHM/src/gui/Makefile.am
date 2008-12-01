noinst_LIBRARIES = libcapricewx.a

libcapricewx_a_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -I@top_srcdir@/src/core -DDATA_PATH=\"$(pkgdatadir)//\" 
libcapricewx_a_SOURCES = \
CapriceAboutImpl.cpp \
CapriceAboutImpl.h\
CapriceApp.cpp\
CapriceApp.h\
CapriceRegistersDialogImpl.cpp\
CapriceRegistersDialogImpl.h\
CapriceUI.cpp\
CapriceUI.h\
CapriceWindowImpl.cpp\
CapriceWindowImpl.h\
CapriceInputSettingsImpl.h\
CapriceInputSettingsImpl.cpp
#video.cpp \
#input.cpp


if RELEASE
  libcapricewx_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
endif
