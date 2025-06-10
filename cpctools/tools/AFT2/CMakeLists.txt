cmake_minimum_required(VERSION 2.8...3.10)
project(AFT2)

set(librs232 RS232)
set(libaft libaft)
set(aft aft)
add_subdirectory(${librs232})
add_subdirectory(${libaft})
add_subdirectory(${aft})

include_directories(${librs232} ${libaft})

if(WIN32)
	add_definitions(-D_WINDOWS)
endif(WIN32)

add_executable(aft2
	${aft_sources}
	${librs232_sources}
	${libaft_sources}
)

target_link_libraries(aft2 cpc tools)
install(TARGETS aft2 RUNTIME DESTINATION bin)
