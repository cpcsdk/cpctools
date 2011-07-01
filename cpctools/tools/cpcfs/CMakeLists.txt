add_executable (cpcfs cpcfs.cpp
)

find_library(bz2 bz2)
target_link_libraries (cpcfs dsk z ${bz2} libcpcfs tools)
