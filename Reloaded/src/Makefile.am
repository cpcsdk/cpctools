bin_PROGRAMS = caprice

#Contains all the dirs (even the one which are not compiled !)
DIST_SUBDIRS = core gui asm cli
SUBDIRS = core
MINGW_LIB=-lmingw32

#TODO : Build video and emulator inside libcapricecore
caprice_SOURCES = #core/video.cpp core/emulator.cpp
caprice_CPPFLAGS = -I$(srcdir)/core -I$(srcdir)/gui -DUNICODE

#caprice core needs to be before portaudio and soundlib
#caprice wx needs to be after mingw32 so we can find WinMain ?
caprice_LDADD = $(SOUNDLIB) $(SOUND_OUTPUT_LIB)

if MINGW
caprice_LDADD += $(MINGW_LIB)
endif

if WITH_WXWIDGET
SUBDIRS += gui
caprice_LDADD += gui/libcapricewx.a gui/libfoo.a 
if WITH_IDE
#this needs to be after capricewx but before wx
caprice_LDADD += gui/IDE/libcapricewxide.a
endif
caprice_LDADD += $(WX_LIBS)
caprice_CPPFLAGS += $(WX_CPPFLAGS)
endif

if HAIKU
  SUBDIRS += gui_be
  caprice_LDADD += gui_be/libreloadedbe.a -lbe -lgame
endif

caprice_LDADD += \
	core/libcapricecore.a \
    $(LIB765) $(PNG_LIB) $(TIMER_LIB)

# Must not be linked in Haiku
if MINGW
caprice_LDADD += -lpthread
endif


#Add CLI files if activate
if CLI
SUBDIRS += cli
caprice_CPPFLAGS += -Icli
caprice_LDADD += cli/libcapricecli.a
endif


if WITH_ASM
SUBDIRS += asm
caprice_LDADD += asm/libsjasm.a \
                 asm\libsjasminternal.a 
if MINGW 
else
caprice_LDADD += -ldl
endif
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
