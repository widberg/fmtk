macro(add_pigeon_target PIGEON_TARGET)
	set(options )
	set(oneValueArgs OUTPUT)
	set(multiValueArgs INPUT)
	cmake_parse_arguments(PIGEON "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )
	add_custom_command(
		OUTPUT ${PIGEON_OUTPUT}
		COMMAND_EXPAND_LISTS
		VERBATIM
		COMMAND $<TARGET_FILE:pigeon> ${PIGEON_INPUT} $<$<BOOL:${PIGEON_OUTPUT}>:-o> ${PIGEON_OUTPUT}
		DEPENDS pigeon ${PIGEON_INPUT}
	)
	add_custom_target(${PIGEON_TARGET} DEPENDS ${PIGEON_OUTPUT})
endmacro()
