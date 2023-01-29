add_library(sinker STATIC
	sinker.cpp
	sinker.hpp
	sinker.tpp
	parser.yy
	${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
)
target_include_directories(sinker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sinker hl++)
add_dependencies(sinker re2c)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp.re ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
	COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy -o ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp.re -Wcounterexamples
	COMMAND $<TARGET_FILE:re2c> ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp.re -o ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp --no-debug-info
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy
)

add_executable(sinkerc
		main.cpp
)
target_link_libraries(sinkerc sinker CLI11::CLI11)

macro(add_sinker_target SINKER_TARGET)
	set(options )
	set(oneValueArgs OUTPUT DEFINITIONS)
	set(multiValueArgs INPUT)
	cmake_parse_arguments(SINKER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	add_custom_command(
		OUTPUT ${SINKER_OUTPUT}
		COMMAND_EXPAND_LISTS
		VERBATIM
		COMMAND $<TARGET_FILE:sinkerc> ${SINKER_INPUT} $<$<BOOL:${SINKER_OUTPUT}>:-o> ${SINKER_OUTPUT} $<$<BOOL:${SINKER_DEFINITIONS}>:-d> ${SINKER_DEFINITIONS}
		DEPENDS sinkerc ${SINKER_INPUT}
	)
	add_custom_target(${SINKER_TARGET} DEPENDS ${SINKER_OUTPUT})
endmacro()
