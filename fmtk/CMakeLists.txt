add_executable(fmtk
	fmtk.cpp
	fmtk.rc
)
target_link_libraries(fmtk common detours spdlog tomlplusplus::tomlplusplus)
add_dependencies(fmtk fmtkdll secudll fmtklua)

if (FUEL_FOUND)
	add_custom_target(run
		COMMAND ${CMAKE_COMMAND} -E copy_directory  "${CMAKE_BINARY_DIR}/mods" "${FUEL_DIR}/mods"
		COMMAND $<TARGET_FILE:fmtk>
		WORKING_DIRECTORY "${FUEL_DIR}/mods/fmtk"
		DEPENDS package_fmtk
	)
endif()
