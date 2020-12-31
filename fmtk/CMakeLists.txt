add_executable(fmtk
	fmtk.cpp
)
target_link_libraries(fmtk common detours spdlog)
add_dependencies(fmtk fmtkdll)

set_target_properties(fmtk PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
	
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
