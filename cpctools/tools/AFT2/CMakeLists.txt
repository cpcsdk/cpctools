project(AFT2)
cmake_minimum_required(VERSION 2.8)

set(librs232 RS232)
set(libaft libaft)
set(aft aft)
add_subdirectory(${librs232})
add_subdirectory(${libaft})
add_subdirectory(${aft})

include_directories(${librs232} ${libaft})

add_executable(aft2
	${aft_sources}
	${librs232_sources}
	${libaft_sources}
)

target_link_libraries(aft2 libcpcfs tools)
