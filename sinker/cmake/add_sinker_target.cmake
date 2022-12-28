macro(add_sinker_target SINKER_TARGET)
	set(options )
	set(oneValueArgs OUTPUT DEFINITIONS WORKING_DIRECTORY)
	set(multiValueArgs INPUT)
	cmake_parse_arguments(SINKER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	add_custom_command(
		OUTPUT ${SINKER_OUTPUT}
		COMMAND_EXPAND_LISTS
		VERBATIM
		COMMAND $<TARGET_FILE:sinker> ${SINKER_INPUT} $<$<BOOL:${SINKER_OUTPUT}>:-o> ${SINKER_OUTPUT} $<$<BOOL:${SINKER_DEFINITIONS}>:-d> ${SINKER_DEFINITIONS}
		DEPENDS sinker ${SINKER_INPUT}
		WORKING_DIRECTORY ${SINKER_WORKING_DIRECTORY}
	)
	add_custom_target(${SINKER_TARGET} DEPENDS ${SINKER_OUTPUT})
endmacro()
