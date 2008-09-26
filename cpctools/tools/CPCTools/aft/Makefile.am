bin_PROGRAMS = aft 

#a modifier pour windows
LDADD = $(srcdir)/../../../lib/Common/libaft/libaft.la $(srcdir)/../../../lib/Common/libtools/libtools.la $(srcdir)/../../../lib/Common/libcpcfs/libcpcfs.la -lserial
AM_CXXFLAGS = -I$(srcdir)/../../../lib/Common/libaft \
              -I$(srcdir)/../../../lib/Common/libtools \
              -I $(srcdir)/../../../lib/Common/libcpcfs
aft_SOURCES = Aft.cpp
