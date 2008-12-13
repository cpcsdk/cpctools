noinst_LIBRARIES = libcapricewx.a

libcapricewx_a_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -I@top_srcdir@/src/core -DDATA_PATH=\"$(pkgdatadir)//\" 
libcapricewx_a_SOURCES = \
CapriceAboutImpl.cpp \
CapriceAboutImpl.h\
CapriceApp.cpp\
CapriceApp.h\
CapriceRegistersDialogImpl.cpp\
CapriceRegistersDialogImpl.h\
CapriceWindowImpl.cpp\
CapriceWindowImpl.h\
CapriceInputSettingsImpl.h\
CapriceInputSettingsImpl.cpp
#video.cpp \
#input.cpp
libcapricewx_a_LDADD = libfoo.a

noinst_LIBRARIES = libfoo.a
libfoo_a_SOURCES = CapriceUI.cpp CapriceUI.h input.cpp
libfoo_a_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -I@top_srcdir@/src/core -DDATA_PATH=\"$(pkgdatadir)//\" -include CPCKeyDefs.h 

if RELEASE
  libcapricewx_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
endif
