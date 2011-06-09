set(libtools_dir ../../../lib/Common/libtools)
set(libcpcfs_dir ../../../lib/Common/libcpcfs)
add_executable (cpcfs cpcfs.cpp ${libtools_dir}/COptionParser.cpp
	${libtools_dir}/CError.cpp ${libcpcfs_dir}/CCPCDisc.cpp
	${libcpcfs_dir}/CCPCFile.cpp ${libcpcfs_dir}/CCPCAsciiFile.cpp
	${libcpcfs_dir}/CCPCBinaryFile.cpp ${libcpcfs_dir}/CDSKFile.cpp
	${libcpcfs_dir}/CCPCSystemDisc.cpp ${libcpcfs_dir}/CCPCRomdosD1Disc.cpp
	${libcpcfs_dir}/CCPCDataDisc.cpp ${libcpcfs_dir}/CCPCUnknownDisc.cpp
	${libtools_dir}/optionParser.c
)

target_link_libraries (cpcfs dsk z bz2)
