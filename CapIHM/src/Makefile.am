bin_PROGRAMS = caprice

DIST_SUBDIRS = core gui asm
SUBDIRS = core gui asm

#TODO : achieve to put the compilation of first 2files in libcapricewx.a and the other in libcapricecore.a
caprice_SOURCES = gui/video.cpp core/emulator.cpp 
caprice_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -Icore -Igui
caprice_LDADD = $(SDL_LIBS) $(SOUNDLIB) $(LIB765) $(WX_LIBS) \
                -L/usr/local/lib \
                core/libcapricecore.a \
                core/libdisass.a \
				asm/libsjasm.a \
				asm/libsjasminternal.a

# TODO : clean this up and move it to wx folder
caprice_LDADD += gui/wx/libfoo.a
caprice_LDADD += gui/wx/libcapricewx.a
caprice_LDADD += gui/wx/IDE/libcapricewxide.a

#Add CLI files if activate
if CLI
DIST_SUBDIRS += cli
SUBDIRS += cli
caprice_CPPFLAGS += -Icli
caprice_LDADD += cli/libcapricecli.a
endif

#Add aylet files if used as backend
if AYLET
  caprice_CPPFLAGS += -Iaylet
  caprice_SOURCES += aylet/sound.cc \
		     aylet/sound.h \
		     aylet/drv-oss.cc \
		     aylet/driver.h 
endif

if RELEASE
  caprice_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
endif
