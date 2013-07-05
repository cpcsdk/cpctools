project (CPCTools)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")

set(libtools_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtools)
set(libcpcfs_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcpcfs)
set(libsnapshot_dir ${CMAKE_CURRENT_SOURCE_DIR}/lib/libsnapshot)

include_directories (${libcpcfs_dir})
include_directories (${libtools_dir})
include_directories (${libsnapshot_dir})
add_subdirectory(tools)
add_subdirectory(lib)
