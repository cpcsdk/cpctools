noinst_LIBRARIES = libcapricecore.a

libcapricecore_a_LIBADD = 
libcapricecore_a_SOURCES = \
	audioPlugin.h \
c_bool.h\
c_inifile.cpp\
c_inifile.h\
cap32.h\
cap32type.h\
config_manager.cpp\
config_manager.h\
crtc.cpp\
crtc.h\
crtc1.cpp\
crtc1.h\
debug.h\
dsk.cpp\
dsk.h\
emulator.cpp\
emulator.h\
error.h\
fdc.cpp\
fdc.h\
fdc_765.cpp\
fdc_765.h\
filetools.cpp\
filetools.h\
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
video.cpp\
video.h\
z80.cpp\
z80.h\
z80daa.h\
zip.cpp\
zip.h\
IMG_savepng.h \
IMG_savepng.cpp \
clock_gettime_stub.cpp clock_gettime_stub.h \
synchro.cpp synchro.h \
log.cpp log.h \
Desass.cpp Desass.h

if PORTAUDIO
  libcapricecore_a_SOURCES += portAudioAudioPlugin.cpp \
							  portAudioAudioPlugin.h
endif

if LIB765
  libcapricecore_a_LIBADD += /usr/local/lib/lib765.a /usr/local/lib/libdsk.a
endif

if RELEASE
  libcapricecore_a_CPPFLAGS = -O2 -funroll-loops -ffast-math -fomit-frame-pointer -fno-strength-reduce -finline-functions -s
endif
