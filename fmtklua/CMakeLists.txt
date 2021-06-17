add_library(fmtklua SHARED
	fmtklua.cpp
)
target_link_libraries(fmtklua fmtksdk)

set_target_properties(fmtklua PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/mods"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/mods"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/mods"
)
