add_library(fmtklua SHARED
	fmtklua.cpp
)
target_link_libraries(fmtklua fmtksdk sol2 lua51)

set_target_properties(fmtklua PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/mods/native"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/mods/native"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/mods/native"
)
