add_library(secudll SHARED
	secudll.cpp
)
target_link_libraries(secudll common detours tomlplusplus::tomlplusplus)
target_link_options(secudll PRIVATE /export:DetourFinishHelperProcess,@1,NONAME)

set_target_properties(secudll PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
