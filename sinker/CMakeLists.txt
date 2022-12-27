add_executable(sinker
		sinker.yy
		sinker.hpp
		sinker.cpp # Let's check this into vc so that re2c and bison aren't required dependencies.
		             # I know codegen'd files shouldn't get checked in but re2c and bison are a pain
		             # to setup on windows.
)
target_link_libraries(sinker common CLI11::CLI11)

set_target_properties(sinker PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
)

set_target_properties(sinker PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re ${CMAKE_CURRENT_SOURCE_DIR}/sinker.cpp
	COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/sinker.yy -o ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re -Wcounterexamples
	COMMAND re2c ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re -o ${CMAKE_CURRENT_SOURCE_DIR}/sinker.cpp --no-debug-info
	DEPENDS sinker.yy
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

macro(sinkerc)
	set(options )
	set(oneValueArgs TARGET OUTPUT DEFINITIONS WORKING_DIRECTORY)
	set(multiValueArgs INPUT)
	cmake_parse_arguments(SINKERC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	message(STATUS "INPUT = ${SINKERC_INPUT}")
	message(STATUS "OUTPUT = ${SINKERC_OUTPUT}")
	message(STATUS "DEFINITIONS = ${SINKERC_DEFINITIONS}")
	message(STATUS "WORKING_DIRECTORY = ${SINKERC_WORKING_DIRECTORY}")
	add_custom_command(
		OUTPUT ${SINKERC_OUTPUT}
		COMMAND_EXPAND_LISTS
		VERBATIM
		COMMAND $<TARGET_FILE:sinker> ${SINKERC_INPUT} $<$<BOOL:${SINKERC_OUTPUT}>:-o> ${SINKERC_OUTPUT} $<$<BOOL:${SINKERC_DEFINITIONS}>:-d>  ${SINKERC_DEFINITIONS}
		DEPENDS sinker ${SINKERC_INPUT}
		WORKING_DIRECTORY ${SINKERC_WORKING_DIRECTORY}
	)
	add_custom_target(${SINKERC_TARGET} DEPENDS ${SINKERC_OUTPUT})
endmacro()

sinkerc(TARGET sink INPUT ${CMAKE_BINARY_DIR}/test.skr OUTPUT ${CMAKE_BINARY_DIR}/out.skr DEFINITIONS ${CMAKE_BINARY_DIR}/out.def WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
