add_executable (cpcfs cpcfs.cpp
)

Find_Package(BZip2 REQUIRED)

target_link_libraries (cpcfs dsk z ${BZIP2_LIBRARIES} libcpcfs tools )
