cmake_minimum_required(VERSION 3.1.0)
project(Reloaded)

# Sorry, we need C++11!
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_RELOADED "Compile Reloaded" ON)
option(USE_SNAMP "Compile SNAMP - SNApshot Music Player" ON)

set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

IF (HAIKU)
	set(USE_THREAD ON)
ELSE (HAIKU)
    option(USE_THREAD "Compile in multi-thread mode" OFF)
ENDIF (HAIKU)

# TODO check if possible for the compiler
option(USE_LTO "Use Link-time optimizer" OFF)
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
if (WIN32)
	include_directories("${CMAKE_SOURCE_DIR}/../libstsound/")
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

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
