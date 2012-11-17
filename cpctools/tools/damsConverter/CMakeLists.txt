add_executable (damsConverter
	damsConverter.cpp
)

target_link_libraries (damsConverter libcpcfs tools)

set(CXXFLAGS -Wall -O3)

set_target_properties(damsConverter PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(damsConverter PROPERTIES COMPILE_FLAGS "-Wall -Wno-multichar -O3 -march=k6")

