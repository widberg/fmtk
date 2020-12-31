add_library(fmtkdll SHARED
	fmtkdll.cpp
)
target_link_libraries(fmtkdll common detours spdlog)
target_link_options(fmtkdll PRIVATE /export:DetourFinishHelperProcess,@1,NONAME)

set_target_properties(fmtkdll PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
