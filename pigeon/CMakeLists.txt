add_executable(pigeon
	pigeon.yy
	${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp
)
target_include_directories(pigeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pigeon CLI11::CLI11)
add_dependencies(pigeon re2c)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp
	BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re
	COMMAND ${BISON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pigeon.yy -o ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re -Wcounterexamples
	COMMAND $<TARGET_FILE:re2c> ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re -o ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp --no-debug-info
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pigeon.yy
)

macro(add_pigeon_target PIGEON_TARGET)
	set(options )
	set(oneValueArgs OUTPUT GLOBAL)
	set(multiValueArgs INPUT)
	cmake_parse_arguments(PIGEON "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	add_custom_command(
		OUTPUT ${PIGEON_OUTPUT}
		COMMAND_EXPAND_LISTS
		VERBATIM
		COMMAND $<TARGET_FILE:pigeon> ${PIGEON_INPUT} $<$<BOOL:${PIGEON_OUTPUT}>:-o> ${PIGEON_OUTPUT} -g ${PIGEON_GLOBAL}
		DEPENDS pigeon ${PIGEON_INPUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
	add_custom_target(${PIGEON_TARGET} DEPENDS ${PIGEON_OUTPUT})
endmacro()
