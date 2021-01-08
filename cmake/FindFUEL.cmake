set(FUEL_SEARCH_PATHS
	"D:\\SteamLibrary\\steamapps\\common\\FUEL"
	"C:\\Program Files (x86)\\Steam\\steamapps\\common\\FUEL"
)

find_program(FUEL FUEL.exe
	HINTS
		${FUEL_SEARCH_PATHS}
	REQUIRED
)
get_filename_component(FUEL_DIR "${FUEL}" DIRECTORY)

file(TO_NATIVE_PATH "${FUEL}" FUEL_EXECUTABLE)
file(TO_NATIVE_PATH "${FUEL_DIR}" FUEL_DIR)

add_custom_target(fuel)
set_target_properties(fuel PROPERTIES
	VS_DEBUGGER_COMMAND "devenv"
	VS_DEBUGGER_COMMAND_ARGUMENTS "/debugexe \"${FUEL_EXECUTABLE}\""
	VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
)

SET(FUEL_EXECUTABLE "${FUEL_EXECUTABLE}" CACHE INTERNAL "")
SET(FUEL_DIR "${FUEL_DIR}" CACHE INTERNAL "")
SET(FUEL_FOUND TRUE CACHE INTERNAL "")
