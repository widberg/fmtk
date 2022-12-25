add_executable(sinker
		sinker.yy
		sinker.hpp
		sinker.cpp # Let's check this into vc so that re2c and bison aren't required dependencies.
		             # I know codegen'd files shouldn't get checked in but re2c and bison are a pain
		             # to setup on windows.
)
target_link_libraries(sinker common)

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
	COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/sinker.yy -o ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re
	COMMAND re2c ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re -o ${CMAKE_CURRENT_SOURCE_DIR}/sinker.cpp --no-debug-info
	DEPENDS sinker.yy
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
