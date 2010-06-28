ALL=	raw2crtc png2crtc


CC=gcc
CCFLAGS=-Os -W -Wall -std=c99 -g
#CCFLAGS=-O3 -W -Wall -pedantic -ansi

all: $(ALL)

clear :
	rm -f a.out *.o core $(ALL)
raw2crtc : raw2crtc.o libraw2crtc.o
	$(CC) $(CCFLAGS) raw2crtc.o libraw2crtc.o -o raw2crtc
png2crtc : png2crtc.o libraw2crtc.o
	$(CC) $(CCFLAGS) png2crtc.o libraw2crtc.o -o png2crtc -lpng -lz
.c.o :
	$(CC) $(CCFLAGS) -c $<
