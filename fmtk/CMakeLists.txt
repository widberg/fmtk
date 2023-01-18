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
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:secudll>" "${FUEL_DIR}/mods/fmtk/secudll.dll"
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtkdll>" "${FUEL_DIR}/mods/fmtk/fmtkdll.dll"
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtklua>" "${FUEL_DIR}/mods/native/fmtklua.dll"
		COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:lua51>" "${FUEL_DIR}/mods/path/lua5.1.dll"
	)
	add_dependencies(copy_mods fmtklua lua51 fmtkdll secudll)
	add_dependencies(fmtk copy_mods)
	
	add_custom_target(run
		COMMAND $<TARGET_FILE:fmtk>
		WORKING_DIRECTORY "${FUEL_DIR}"
	)
	add_dependencies(run fmtk)
endif()

set_target_properties(fmtk PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
