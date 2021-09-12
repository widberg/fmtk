add_executable(hookcash
	hookcash.cpp
)
target_link_libraries(hookcash common)

set_target_properties(hookcash PROPERTIES
	VS_DEBUGGER_WORKING_DIRECTORY "${FUEL_DIR}"
)

set_target_properties(hookcash PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
