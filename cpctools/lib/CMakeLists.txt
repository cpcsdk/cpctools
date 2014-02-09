add_subdirectory(libtools)

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

install(TARGETS cpc LIBRARY DESTINATION lib)
target_link_libraries (cpc dsk tools)
