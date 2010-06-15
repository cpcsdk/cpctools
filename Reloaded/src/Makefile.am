bin_PROGRAMS = caprice

DIST_SUBDIRS = core gui
SUBDIRS = core gui


if MINGW
MINGW=-lmingw32
else
MINGW=
endif

#TODO : Build video and emulator inside libcapricecore
caprice_SOURCES = core/video.cpp core/emulator.cpp 
caprice_CPPFLAGS = $(WX_CPPFLAGS) -Icore -Igui -DUNICODE
#caprice core needs to be before portaudio and soundlib
#caprice wx needs to be after mingw32 so we can find WinMain ?
caprice_LDADD = $(MINGW) -lpthread \
                gui/libcapricewx.a gui/libfoo.a $(WX_LIBS) \
				core/libcapricecore.a \
                core/libdisass.a \
				$(LIB765) $(PNG_LIB) $(SOUNDLIB) -lportaudio

if WITH_IDE
caprice_LDADD += gui/IDE/libcapricewxide.a
endif

if WITH_ASM
DIST_SUBDIRS += asm
SUBDIRS += asm
caprice_LDADD += asm/libsjasm.a \
				 asm/libsjasminternal.a
endif

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
