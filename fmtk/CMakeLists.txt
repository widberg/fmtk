add_executable(fmtk
	fmtk.cpp
	fmtk.rc
)
target_link_libraries(fmtk common detours spdlog tomlplusplus::tomlplusplus)
add_dependencies(fmtk fmtkdll secudll fmtklua)

if (FUEL_FOUND)
	set_target_properties(fmtk PROPERTIES
		VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
		VS_DEBUGGER_ENVIRONMENT "PATH=$ENV{PATH};$<TARGET_FILE_DIR:fmtk>"
	)

	add_custom_target(copy_mods
		COMMAND ${CMAKE_COMMAND} -E copy_directory  "${CMAKE_SOURCE_DIR}/mods" "${FUEL_DIR}/mods"
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtklua>" "${FUEL_DIR}/mods/native/fmtklua.dll"
	)
	add_dependencies(copy_mods fmtklua)
	add_dependencies(fmtk copy_mods)
endif()

set_target_properties(fmtk PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
