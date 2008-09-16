# makefile for aylet

# You need an ANSI C compiler. gcc is probably best.
#
CC=gcc

# Compiler options (and sound driver) to use. This one is for Linux,
# and other OSS-using systems.
#
CFLAGS=-O -Wall -DDRIVER_OSS

# for OpenBSD, uncomment this:
#CFLAGS=-O -Wall -DDRIVER_OPENBSD

# Set how to link the curses lib - this is for Linux.
# (Most non-Linux systems will probably want `-lcurses' instead.)
#
CURSES_LIB=-lncurses

# dest for make install
#
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
XBINDIR=$(BINDIR)
MANDIR=$(PREFIX)/man/man1

# if you want the X version to be installed in the usual X executables
# directory, uncomment this (altering if necessary):
#
#XBINDIR=/usr/X11R6/bin


# you shouldn't need to edit the rest
#-----------------------------------------------------------------

# this looks wrong, but *ops.c are actually #include'd by z80.c
OBJS=main.o sound.o ui.o z80.o drv-oss.o drv-obsd.o
XOBJS=main.o sound.o uigtk.o z80.o drv-oss.o drv-obsd.o

all: aylet xaylet

aylet: $(OBJS)
	$(CC) -o aylet $(OBJS) $(CURSES_LIB)

xaylet: $(XOBJS)
	$(CC) -o xaylet $(XOBJS) `gtk-config --libs`

uigtk.o: uigtk.c
	$(CC) $(CFLAGS) `gtk-config --cflags` -c uigtk.c -o uigtk.o

installdirs:
	/bin/sh ./mkinstalldirs $(BINDIR) $(XBINDIR) $(MANDIR)

install: installdirs
	if [ -f aylet ]; then install -m 755 aylet $(BINDIR); fi
	if [ -f xaylet ]; then install -m 755 xaylet $(XBINDIR); fi
	install -m 644 aylet.1 $(MANDIR)
	ln -sf $(MANDIR)/aylet.1 $(MANDIR)/xaylet.1

uninstall:
	$(RM) $(BINDIR)/aylet $(XBINDIR)/xaylet
	$(RM) $(MANDIR)/aylet.1* $(MANDIR)/xaylet.1*

clean:
	$(RM) *.o *~ aylet xaylet

# dependencies
cbops.o: cbops.c
drv-obsd.o: drv-obsd.c
drv-oss.o: drv-oss.c
edops.o: edops.c
main.o: main.c main.h sound.h ui.h z80.h
sound.o: sound.c main.h z80.h sound.h driver.h
ui.o: ui.c main.h ui.h
uigtk.o: uigtk.c main.h ui.h button1.xpm button2.xpm button3.xpm \
 button4.xpm button5.xpm
z80.o: z80.c main.h z80.h z80ops.c cbops.c edops.c
z80ops.o: z80ops.c cbops.c edops.c


VERS=0.5

tgz: ../aylet-$(VERS).tar.gz
  
../aylet-$(VERS).tar.gz: clean
	$(RM) ../aylet-$(VERS)
	@cd ..;ln -s aylet aylet-$(VERS)
	cd ..;tar zchvf aylet-$(VERS).tar.gz aylet-$(VERS)
	@cd ..;$(RM) aylet-$(VERS)
