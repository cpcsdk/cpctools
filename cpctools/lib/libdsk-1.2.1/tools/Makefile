# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4-p6 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/libdsk
pkglibdir = $(libdir)/libdsk
pkgincludedir = $(includedir)/libdsk

top_builddir = ..

ACLOCAL = aclocal-1.4
AUTOCONF = autoconf
AUTOMAKE = automake-1.4
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
build_alias = i686-pc-linux-gnu
build_triplet = i686-pc-linux-gnu
host_alias = i686-pc-linux-gnu
host_triplet = i686-pc-linux-gnu
target_alias = i686-pc-linux-gnu
target_triplet = i686-pc-linux-gnu
AS = @AS@
CC = gcc
CPP = gcc -E
DLLTOOL = @DLLTOOL@
ECHO = echo
EXEEXT = 
HAVE_LIB = @HAVE_LIB@
JAR = true
JAVA = true
JAVACFLAGS = 
JAVADOC = true
JAVAFLAGS = 
JAVAH = true
JAVAPREFIX = 
LIB = @LIB@
LIBDSKJAR = 
LIBTOOL = $(SHELL) $(top_builddir)/libtool
LN_S = ln -s
LTLIB = @LTLIB@
MAKEINFO = makeinfo
OBJDUMP = @OBJDUMP@
OBJEXT = o
PACKAGE = libdsk
RANLIB = ranlib
STRIP = strip
TOOLCLASSES = 
VERSION = 1.2.1
_ACJNI_JAVAC = 
uudecode = 

INCLUDES = -I$(top_srcdir)/include
LDADD = ../lib/libdsk.la

CLASSPATH = 
JAVAC = true 

bin_PROGRAMS = dsktrans dskform dskid dskdump dskscan dskutil md3serial apriboot
dsktrans_SOURCES = dsktrans.c utilopts.c utilopts.h formname.c formname.h 		 apriboot.h bootsec.c

apriboot_SOURCES = apriboot.c bootsec.c apriboot.h formname.c formname.h 		 utilopts.c utilopts.h

dskform_SOURCES = dskform.c utilopts.c utilopts.h formname.c formname.h
dskid_SOURCES = dskid.c utilopts.c utilopts.h
md3serial_SOURCES = md3serial.c utilopts.c utilopts.h
dskscan_SOURCES = dskscan.c utilopts.c utilopts.h formname.c formname.h
dskdump_SOURCES = dskdump.c utilopts.c utilopts.h formname.c formname.h
dskutil_SOURCES = dskutil.c utilopts.c utilopts.h formname.c formname.h
forkslave_SOURCES = forkslave.c
serslave_SOURCES = serslave.c crc16.c crc16.h
dsktest_SOURCES = dsktest.c utilopts.c utilopts.h

noinst_PROGRAMS =  forkslave dsktest serslave 
EXTRA_PROGRAMS = 
EXTRA_DIST = DskTrans.java DskFormat.java DskID.java FormatNames.java UtilOpts.java ScreenReporter.java

check_PROGRAMS = check1 check2 check3
check1_SOURCES = check1.c
check2_SOURCES = check2.c
check3_SOURCES = check3.c
CLEANFILES = *.class
mkinstalldirs = $(SHELL) $(top_srcdir)/config/mkinstalldirs
CONFIG_HEADER = ../config.h
CONFIG_CLEAN_FILES = 
bin_PROGRAMS =  dsktrans$(EXEEXT) dskform$(EXEEXT) dskid$(EXEEXT) \
dskdump$(EXEEXT) dskscan$(EXEEXT) dskutil$(EXEEXT) md3serial$(EXEEXT) \
apriboot$(EXEEXT)
check_PROGRAMS =  check1$(EXEEXT) check2$(EXEEXT) check3$(EXEEXT)
noinst_PROGRAMS =   forkslave$(EXEEXT) dsktest$(EXEEXT) \
serslave$(EXEEXT)
PROGRAMS =  $(bin_PROGRAMS) $(noinst_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I..
CPPFLAGS =  -DAUTOSHARE=${prefix}/share/LibDsk
LDFLAGS = 
LIBS = -lz 
dsktrans_OBJECTS =  dsktrans.$(OBJEXT) utilopts.$(OBJEXT) \
formname.$(OBJEXT) bootsec.$(OBJEXT)
dsktrans_LDADD = $(LDADD)
dsktrans_DEPENDENCIES =  ../lib/libdsk.la
dsktrans_LDFLAGS = 
dskform_OBJECTS =  dskform.$(OBJEXT) utilopts.$(OBJEXT) \
formname.$(OBJEXT)
dskform_LDADD = $(LDADD)
dskform_DEPENDENCIES =  ../lib/libdsk.la
dskform_LDFLAGS = 
dskid_OBJECTS =  dskid.$(OBJEXT) utilopts.$(OBJEXT)
dskid_LDADD = $(LDADD)
dskid_DEPENDENCIES =  ../lib/libdsk.la
dskid_LDFLAGS = 
dskdump_OBJECTS =  dskdump.$(OBJEXT) utilopts.$(OBJEXT) \
formname.$(OBJEXT)
dskdump_LDADD = $(LDADD)
dskdump_DEPENDENCIES =  ../lib/libdsk.la
dskdump_LDFLAGS = 
dskscan_OBJECTS =  dskscan.$(OBJEXT) utilopts.$(OBJEXT) \
formname.$(OBJEXT)
dskscan_LDADD = $(LDADD)
dskscan_DEPENDENCIES =  ../lib/libdsk.la
dskscan_LDFLAGS = 
dskutil_OBJECTS =  dskutil.$(OBJEXT) utilopts.$(OBJEXT) \
formname.$(OBJEXT)
dskutil_LDADD = $(LDADD)
dskutil_DEPENDENCIES =  ../lib/libdsk.la
dskutil_LDFLAGS = 
md3serial_OBJECTS =  md3serial.$(OBJEXT) utilopts.$(OBJEXT)
md3serial_LDADD = $(LDADD)
md3serial_DEPENDENCIES =  ../lib/libdsk.la
md3serial_LDFLAGS = 
apriboot_OBJECTS =  apriboot.$(OBJEXT) bootsec.$(OBJEXT) \
formname.$(OBJEXT) utilopts.$(OBJEXT)
apriboot_LDADD = $(LDADD)
apriboot_DEPENDENCIES =  ../lib/libdsk.la
apriboot_LDFLAGS = 
check1_OBJECTS =  check1.$(OBJEXT)
check1_LDADD = $(LDADD)
check1_DEPENDENCIES =  ../lib/libdsk.la
check1_LDFLAGS = 
check2_OBJECTS =  check2.$(OBJEXT)
check2_LDADD = $(LDADD)
check2_DEPENDENCIES =  ../lib/libdsk.la
check2_LDFLAGS = 
check3_OBJECTS =  check3.$(OBJEXT)
check3_LDADD = $(LDADD)
check3_DEPENDENCIES =  ../lib/libdsk.la
check3_LDFLAGS = 
forkslave_OBJECTS =  forkslave.$(OBJEXT)
forkslave_LDADD = $(LDADD)
forkslave_DEPENDENCIES =  ../lib/libdsk.la
forkslave_LDFLAGS = 
dsktest_OBJECTS =  dsktest.$(OBJEXT) utilopts.$(OBJEXT)
dsktest_LDADD = $(LDADD)
dsktest_DEPENDENCIES =  ../lib/libdsk.la
dsktest_LDFLAGS = 
serslave_OBJECTS =  serslave.$(OBJEXT) crc16.$(OBJEXT)
serslave_LDADD = $(LDADD)
serslave_DEPENDENCIES =  ../lib/libdsk.la
serslave_LDFLAGS = 
CFLAGS = -Wall -DNOTWINDLL -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
LTCOMPILE = $(LIBTOOL) --mode=compile $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(LIBTOOL) --mode=link $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
SOURCES = $(dsktrans_SOURCES) $(dskform_SOURCES) $(dskid_SOURCES) $(dskdump_SOURCES) $(dskscan_SOURCES) $(dskutil_SOURCES) $(md3serial_SOURCES) $(apriboot_SOURCES) $(check1_SOURCES) $(check2_SOURCES) $(check3_SOURCES) $(forkslave_SOURCES) $(dsktest_SOURCES) $(serslave_SOURCES)
OBJECTS = $(dsktrans_OBJECTS) $(dskform_OBJECTS) $(dskid_OBJECTS) $(dskdump_OBJECTS) $(dskscan_OBJECTS) $(dskutil_OBJECTS) $(md3serial_OBJECTS) $(apriboot_OBJECTS) $(check1_OBJECTS) $(check2_OBJECTS) $(check3_OBJECTS) $(forkslave_OBJECTS) $(dsktest_OBJECTS) $(serslave_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .lo .o .obj .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --foreign --include-deps tools/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(LIBTOOL)  --mode=install $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	    $(LIBTOOL)  --mode=install $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

mostlyclean-checkPROGRAMS:

clean-checkPROGRAMS:
	-test -z "$(check_PROGRAMS)" || rm -f $(check_PROGRAMS)

distclean-checkPROGRAMS:

maintainer-clean-checkPROGRAMS:

mostlyclean-noinstPROGRAMS:

clean-noinstPROGRAMS:
	-test -z "$(noinst_PROGRAMS)" || rm -f $(noinst_PROGRAMS)

distclean-noinstPROGRAMS:

maintainer-clean-noinstPROGRAMS:

.c.o:
	$(COMPILE) -c $<

# FIXME: We should only use cygpath when building on Windows,
# and only if it is available.
.c.obj:
	$(COMPILE) -c `cygpath -w $<`

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core
	-rm -f *.$(OBJEXT)

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

.c.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.s.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

.S.lo:
	$(LIBTOOL) --mode=compile $(COMPILE) -c $<

mostlyclean-libtool:
	-rm -f *.lo

clean-libtool:
	-rm -rf .libs _libs

distclean-libtool:

maintainer-clean-libtool:

dsktrans$(EXEEXT): $(dsktrans_OBJECTS) $(dsktrans_DEPENDENCIES)
	@rm -f dsktrans$(EXEEXT)
	$(LINK) $(dsktrans_LDFLAGS) $(dsktrans_OBJECTS) $(dsktrans_LDADD) $(LIBS)

dskform$(EXEEXT): $(dskform_OBJECTS) $(dskform_DEPENDENCIES)
	@rm -f dskform$(EXEEXT)
	$(LINK) $(dskform_LDFLAGS) $(dskform_OBJECTS) $(dskform_LDADD) $(LIBS)

dskid$(EXEEXT): $(dskid_OBJECTS) $(dskid_DEPENDENCIES)
	@rm -f dskid$(EXEEXT)
	$(LINK) $(dskid_LDFLAGS) $(dskid_OBJECTS) $(dskid_LDADD) $(LIBS)

dskdump$(EXEEXT): $(dskdump_OBJECTS) $(dskdump_DEPENDENCIES)
	@rm -f dskdump$(EXEEXT)
	$(LINK) $(dskdump_LDFLAGS) $(dskdump_OBJECTS) $(dskdump_LDADD) $(LIBS)

dskscan$(EXEEXT): $(dskscan_OBJECTS) $(dskscan_DEPENDENCIES)
	@rm -f dskscan$(EXEEXT)
	$(LINK) $(dskscan_LDFLAGS) $(dskscan_OBJECTS) $(dskscan_LDADD) $(LIBS)

dskutil$(EXEEXT): $(dskutil_OBJECTS) $(dskutil_DEPENDENCIES)
	@rm -f dskutil$(EXEEXT)
	$(LINK) $(dskutil_LDFLAGS) $(dskutil_OBJECTS) $(dskutil_LDADD) $(LIBS)

md3serial$(EXEEXT): $(md3serial_OBJECTS) $(md3serial_DEPENDENCIES)
	@rm -f md3serial$(EXEEXT)
	$(LINK) $(md3serial_LDFLAGS) $(md3serial_OBJECTS) $(md3serial_LDADD) $(LIBS)

apriboot$(EXEEXT): $(apriboot_OBJECTS) $(apriboot_DEPENDENCIES)
	@rm -f apriboot$(EXEEXT)
	$(LINK) $(apriboot_LDFLAGS) $(apriboot_OBJECTS) $(apriboot_LDADD) $(LIBS)

check1$(EXEEXT): $(check1_OBJECTS) $(check1_DEPENDENCIES)
	@rm -f check1$(EXEEXT)
	$(LINK) $(check1_LDFLAGS) $(check1_OBJECTS) $(check1_LDADD) $(LIBS)

check2$(EXEEXT): $(check2_OBJECTS) $(check2_DEPENDENCIES)
	@rm -f check2$(EXEEXT)
	$(LINK) $(check2_LDFLAGS) $(check2_OBJECTS) $(check2_LDADD) $(LIBS)

check3$(EXEEXT): $(check3_OBJECTS) $(check3_DEPENDENCIES)
	@rm -f check3$(EXEEXT)
	$(LINK) $(check3_LDFLAGS) $(check3_OBJECTS) $(check3_LDADD) $(LIBS)

forkslave$(EXEEXT): $(forkslave_OBJECTS) $(forkslave_DEPENDENCIES)
	@rm -f forkslave$(EXEEXT)
	$(LINK) $(forkslave_LDFLAGS) $(forkslave_OBJECTS) $(forkslave_LDADD) $(LIBS)

dsktest$(EXEEXT): $(dsktest_OBJECTS) $(dsktest_DEPENDENCIES)
	@rm -f dsktest$(EXEEXT)
	$(LINK) $(dsktest_LDFLAGS) $(dsktest_OBJECTS) $(dsktest_LDADD) $(LIBS)

serslave$(EXEEXT): $(serslave_OBJECTS) $(serslave_DEPENDENCIES)
	@rm -f serslave$(EXEEXT)
	$(LINK) $(serslave_LDFLAGS) $(serslave_OBJECTS) $(serslave_LDADD) $(LIBS)

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags -o $$here/TAGS $(ETAGS_ARGS) $$tags  $$unique $(LISP))

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = tools

distdir: $(DISTFILES)
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
	$(MAKE) $(AM_MAKEFLAGS) $(check_PROGRAMS)
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-binPROGRAMS
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir)


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-binPROGRAMS mostlyclean-checkPROGRAMS \
		mostlyclean-noinstPROGRAMS mostlyclean-compile \
		mostlyclean-libtool mostlyclean-tags \
		mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-binPROGRAMS clean-checkPROGRAMS clean-noinstPROGRAMS \
		clean-compile clean-libtool clean-tags clean-generic \
		mostlyclean-am

clean: clean-am

distclean-am:  distclean-binPROGRAMS distclean-checkPROGRAMS \
		distclean-noinstPROGRAMS distclean-compile \
		distclean-libtool distclean-tags distclean-generic \
		clean-am
	-rm -f libtool

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-binPROGRAMS \
		maintainer-clean-checkPROGRAMS \
		maintainer-clean-noinstPROGRAMS \
		maintainer-clean-compile maintainer-clean-libtool \
		maintainer-clean-tags maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-checkPROGRAMS distclean-checkPROGRAMS clean-checkPROGRAMS \
maintainer-clean-checkPROGRAMS mostlyclean-noinstPROGRAMS \
distclean-noinstPROGRAMS clean-noinstPROGRAMS \
maintainer-clean-noinstPROGRAMS mostlyclean-compile distclean-compile \
clean-compile maintainer-clean-compile mostlyclean-libtool \
distclean-libtool clean-libtool maintainer-clean-libtool tags \
mostlyclean-tags distclean-tags clean-tags maintainer-clean-tags \
distdir info-am info dvi-am dvi check check-am installcheck-am \
installcheck install-exec-am install-exec install-data-am install-data \
install-am install uninstall-am uninstall all-redirect all-am all \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


%.class:        $(srcdir)/%.java
	here=`pwd` && cd $(srcdir) && $(JAVAC) -classpath $(CLASSPATH):$$here/../lib/libdsk.jar -d $$here $<

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
