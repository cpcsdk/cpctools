add_subdirectory (AFT2)
add_subdirectory(cpcfs)
#add_subdirectory(cpcVideo)
add_subdirectory(damsConverter)

add_executable (createSnapshot createSnapshot.cpp)
target_link_libraries (createSnapshot cpc tools)
install(TARGETS createSnapshot RUNTIME DESTINATION bin)
