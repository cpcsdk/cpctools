noinst_LIBRARIES = libcapricecore.a libdisass.a

libcapricecore_a_LIBADD =
libcapricecore_a_CPPFLAGS=$(SDL_CFLAGS) -DDATA_PATH=\"$(pkgdatadir)//\" 
libcapricecore_a_SOURCES = \
audio.cpp\
audio.h\
c_bool.h\
c_inifile.cpp\
c_inifile.h\
cap32.h\
cap32type.h\
config.cpp\
config.h\
crtc.cpp\
crtc.h\
debug.h\
dsk.cpp\
dsk.h\
emulator.h\
error.h\
fdc.cpp\
fdc.h\
fdc_765.cpp\
fdc_765.h\
filetools.cpp\
filetools.h\
fileutils.cpp\
fileutils.h\
gatearray.cpp\
gatearray.h\
glfunclist.h\
glfuncs.cpp\
glfuncs.h\
input.h\
input.cpp\
ioport.cpp\
ioport.h\
memory.cpp\
memory.h\
ppi.cpp\
ppi.h\
psg.cpp\
psg.h\
render.cpp\
render.h\
renderData.cpp\
snapshot.cpp\
snapshot.h\
tape.cpp\
tape.h\
vdu.cpp\
vdu.h\
video.h\
z80.cpp\
z80.h\
z80daa.h\
zip.cpp\
zip.h\
IMG_savepng.h \
IMG_savepng.cpp

#Created due to cross references ...
#TODO: change that
libdisass_a_SOURCES = \
Desass.cpp \
Desass.h \
Outils.h \
Outils.cpp \
Itoa.h \
Itoa.cpp

if LIB765
  libcapricecore_a_LIBADD += /usr/local/lib/lib765.a /usr/local/lib/libdsk.a
endif

if RELEASE
  libcapricecore_a_CPPFLAGS += -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s  
endif
