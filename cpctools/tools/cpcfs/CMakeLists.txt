add_executable (cpcfs cpcfs.cpp
)

target_link_libraries (cpcfs ${LIBDSK_NAME} cpc tools )
install(TARGETS cpcfs RUNTIME DESTINATION bin)
