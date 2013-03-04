cmake_minimum_required(VERSION 2.8)
project(Reloaded)

option(USE_RELOADED "Compile Reloaded" ON)
option(USE_SNAMP "Compile SNAMP - SNApshot Music Player" ON)
IF (HAIKU)
	set(USE_THREAD ON)
ELSE (HAIKU)
    option(USE_THREAD "Compile in multi-thread mode" OFF)
ENDIF (HAIKU)
option(USE_LTO "Use Link-time optimizer" OFF)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ActivateCXX11)

# TODO check if possible for the compiler
if (USE_LTO)
	message(STATUS "We going to try LTO")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -flto")
endif(USE_LTO)

if (STDERR_LOG)
	add_definitions(-DSTDERR_LOG)
endif (STDERR_LOG)

#TODO check for StSound
add_definitions(-DST_SOUND)

IF(USE_THREAD)
	find_package(Threads REQUIRED)
	add_definitions(-DUSE_PTHREAD)
ENDIF(USE_THREAD)

IF (HAIKU)
	add_subdirectory(src/gui_be)
ELSE (HAIKU)
	add_subdirectory(src/gui)
ENDIF (HAIKU)

add_subdirectory(src/core)
add_subdirectory(src/snamp)
