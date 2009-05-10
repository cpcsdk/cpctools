bin_PROGRAMS = caprice

DIST_SUBDIRS = core gui cli asm
SUBDIRS = core gui

#TODO : achieve to put the compilation of first 2files in libcapricewx.a and the other in libcapricecore.a
caprice_SOURCES = gui/video.cpp core/emulator.cpp 
caprice_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -Icore -Igui
caprice_LDADD = $(SDL_LIBS) $(SOUNDLIB) $(LIB765) $(WX_LIBS) \
                -L/usr/local/lib \
                gui/libcapricewx.a gui/libfoo.a core/libcapricecore.a core/libdisass.a
  

#Add CLI files if activate
if CLI
DIST_SUBDIRS += cli asm
SUBDIRS += cli asm
caprice_CPPFLAGS += -Icli
caprice_LDADD += cli/libcapricecli.a asm/libsjasm.a asm/libsjasminternal.a
endif

#Add aylet files if used as backend
if AYLET
  caprice_CPPFLAGS += -Iaylet
  caprice_SOURCES += aylet/sound.cc \
		     aylet/sound.h \
		     aylet/drv-oss.cc \
		     aylet/driver.h 
endif

#BUG : activated even in debug mode ...
#if RELEASE
#  caprice_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
#endif
