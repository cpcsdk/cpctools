project (CPCTools)
cmake_minimum_required(VERSION 2.8)

set(libtools_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtools)
set(libcpcfs_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcpcfs)

include_directories (${libcpcfs_dir})
include_directories (${libtools_dir})
add_subdirectory(tools)
add_subdirectory(lib)
