add_executable(dpc
	dpc.cpp
)

target_link_libraries(dpc PRIVATE nlohmann_json::nlohmann_json)

set_target_properties(dpc PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
	
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
