macro(add_sinker_target SINKERC_TARGET)
	set(options )
	set(oneValueArgs OUTPUT DEFINITIONS WORKING_DIRECTORY)
	set(multiValueArgs INPUT)
	cmake_parse_arguments(SINKERC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	add_custom_command(
		OUTPUT ${SINKERC_OUTPUT}
		COMMAND_EXPAND_LISTS
		VERBATIM
		COMMAND $<TARGET_FILE:sinker> ${SINKERC_INPUT} $<$<BOOL:${SINKERC_OUTPUT}>:-o> ${SINKERC_OUTPUT} $<$<BOOL:${SINKERC_DEFINITIONS}>:-d> ${SINKERC_DEFINITIONS}
		DEPENDS sinker ${SINKERC_INPUT}
		WORKING_DIRECTORY ${SINKERC_WORKING_DIRECTORY}
	)
	add_custom_target(${SINKERC_TARGET} DEPENDS ${SINKERC_OUTPUT})
endmacro()
