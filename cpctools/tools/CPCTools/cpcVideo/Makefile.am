bin_PROGRAMS = cpcvideo
LDADD = $(srcdir)/../../../lib/Common/libVideoCPC/libVideoCPC.la $(srcdir)/../../../lib/Common/libtools/libtools.la $(SDL_LIBS) -lAntTweakBar -lGL -lGLU -lX11 -lXext  -lpthread -lm -lglfw -lXxf86vm
AM_CXXFLAGS = -I$(srcdir)/../../../lib/Common/libVideoCPC \
              -I$(srcdir)/../../../lib/Common/libtools \
	      -I../../../lib/AntTweakBar/include \
              $(SDL_CFLAGS)

cpcvideo_SOURCES = CCPCDataView.cpp  CCPCFontView.cpp  CCPCScreenView.cpp  CCPCWindowView.cpp  cpcFileViewer.cpp CCPCDataView.h    CCPCFontView.h    CCPCScreenView.h    CCPCWindowView.h 


