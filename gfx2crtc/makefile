ALL=	raw2crtc png2crtc


CC=gcc

# Detect gcc2, if we're running it, use gnu9x standard instead of c99...
GCC_MAJOR = $(shell $(CC) -v 2>&1 |grep version |cut -d' ' -f3 |cut -d'.' -f1)

ifeq ($(GCC_MAJOR),2)
	CCFLAGS=-Os -W -Wall -std=gnu9x -g
else
	CCFLAGS=-Os -W -Wall -std=c99 -g
endif

#CCFLAGS=-O3 -W -Wall -pedantic -ansi

all: $(ALL)

clean :
	rm -f a.out *.o core $(ALL)

raw2crtc : raw2crtc.o libraw2crtc.o
	$(CC) $(CCFLAGS) raw2crtc.o libraw2crtc.o -o raw2crtc

png2crtc : png2crtc.o libraw2crtc.o
	$(CC) $(CCFLAGS) png2crtc.o libraw2crtc.o -o png2crtc -lpng -lz

.c.o :
	$(CC) $(CCFLAGS) -c $<
