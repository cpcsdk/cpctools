bin_PROGRAMS = caprice

DIST_SUBDIRS = core gui cli asm
SUBDIRS = core gui asm $(MAYBE_CLI)

#TODO : achieve to put the compilation of first 2files in libcapricewx.a and the other in libcapricecore.a
caprice_SOURCES = gui/video.cpp core/emulator.cpp 
caprice_CPPFLAGS = $(WX_CPPFLAGS) $(SDL_CFLAGS) -Icore -Igui -Icli
caprice_LDADD = $(SDL_LIBS) $(SOUNDLIB) $(LIB765) $(WX_LIBS) \
                -L/usr/local/lib -l765 -ldsk  \
                gui/libcapricewx.a gui/libfoo.a core/libcapricecore.a $(CLI_LIB) core/libdisass.a  asm/libsjasm.a 

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
