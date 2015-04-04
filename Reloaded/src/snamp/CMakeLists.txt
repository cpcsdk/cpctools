# In this directory we build the executable without gui (SNAMP)
cmake_minimum_required(VERSION 2.8)
project(SNAMP)

include_directories(..)

if(HAIKU)
	set (AudioPlugin ../gui_be/RAudio.h)
else(HAIKU)
	find_package(Portaudio REQUIRED)

	set (AudioPlugin ../aop/portAudioAudioPlugin.cpp)
	include_directories(${PORTAUDIO_INCLUDE_DIRS})
endif(HAIKU)

add_executable(snamp 
	main.cpp
	${AudioPlugin}
)

add_dependencies(snamp ReloadedCore)
target_link_libraries(snamp ReloadedCore)

target_link_libraries(snamp ${CMAKE_THREAD_LIB_INIT})

if (WIN32)
	target_link_libraries(snamp ${PORTAUDIO_LIBRARIES})
else (WIN32)
	if (NOT HAIKU)
		target_link_libraries(snamp portaudio)
	endif (NOT HAIKU)
endif (WIN32)
