add_executable (cpcfs cpcfs.cpp
)

target_link_libraries (cpcfs dsk cpc tools )
install(TARGETS cpcfs RUNTIME DESTINATION bin)
