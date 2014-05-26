add_executable(dataLinker dataLinker.cpp CDataLink.cpp)
install(TARGETS dataLinker DESTINATION bin)
target_link_libraries(dataLinker tools)
