include(CheckFunctionExists)

add_subdirectory(libtools)


# Portability checks
CHECK_FUNCTION_EXISTS(strcasecmp HAVE_STRCASECMP)
if(NOT HAVE_STRCASECMP)
	add_definitions(-Dstrcasecmp=_stricmp)
endif()


add_library(cpc SHARED
	CCPCSnapshot.cpp
	libcpcfs/CCPCAsciiFile.cpp
	libcpcfs/CCPCBinaryFile.cpp
	libcpcfs/CCPCDataDisc.cpp
	libcpcfs/CCPCDisc.cpp
	libcpcfs/CCPCFile.cpp
	libcpcfs/CCPCRomdosD1Disc.cpp
	libcpcfs/CCPCSystemDisc.cpp
	libcpcfs/CCPCUnknownDisc.cpp
	libcpcfs/CDSKFile.cpp
	)

SET_TARGET_PROPERTIES(cpc PROPERTIES COMPILE_FLAGS -DBUILDING_DLL)

install(TARGETS cpc DESTINATION lib)
target_link_libraries (cpc ${LIBDSK_LIBRARY} tools)
