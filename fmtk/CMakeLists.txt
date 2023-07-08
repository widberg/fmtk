add_executable(fmtk
	fmtk.cpp
	fmtk.rc
)
target_link_libraries(fmtk common detours spdlog tomlplusplus::tomlplusplus)
add_dependencies(fmtk fmtkdll fmtklua fmtkdiscord)

if (FUEL_FOUND)
	add_custom_target(run
		COMMAND ${CMAKE_COMMAND} -E copy_directory  "${CMAKE_BINARY_DIR}/mods" "${FUEL_DIR}/mods"
		COMMAND ${CMAKE_COMMAND} -E env "FMTK_SOURCE_PREFIXES=${PROJECT_SOURCE_DIR}" $<TARGET_FILE:fmtk>
		WORKING_DIRECTORY "${FUEL_DIR}/mods/fmtk"
		DEPENDS package_fmtk
	)
endif()
