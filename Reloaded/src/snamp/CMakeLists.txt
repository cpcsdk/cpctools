# In this directory we build the executable without gui (SNAMP)
cmake_minimum_required(VERSION 2.8)
project(SNAMP)

include_directories(..)

if(HAIKU)
	set (AudioPlugin ../gui_be/RAudio.h)
else(HAIKU)
	set (AudioPlugin ../aop/portAudioAudioPlugin.cpp)
endif(HAIKU)

add_executable(snamp WIN32 
	main.cpp
	${AudioPlugin}
)

add_dependencies(snamp ReloadedCore)
target_link_libraries(snamp ReloadedCore)

target_link_libraries(snamp ${CMAKE_THREAD_LIB_INIT})

if (WIN32)
	target_link_libraries(snamp portaudio_x86)
else (WIN32)
	if (NOT HAIKU)
		target_link_libraries(snamp portaudio)
	endif (NOT HAIKU)
endif (WIN32)
