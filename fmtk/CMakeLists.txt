add_executable(fmtk
	fmtk.cpp
	fmtk.rc
)
target_link_libraries(fmtk common detours spdlog tomlplusplus::tomlplusplus)
add_dependencies(fmtk fmtkdll secudll)

if (FUEL_FOUND)
	set_target_properties(fmtk PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
	)
endif()

set_target_properties(fmtk PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
