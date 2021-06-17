add_library(fmtkdll SHARED
	fmtkdll.cpp fmtkdll.hpp
	instrument.cpp instrument.hpp
)
target_link_libraries(fmtkdll common fmtksdk detours spdlog d3d9 dbghelp tomlplusplus::tomlplusplus)
target_link_options(fmtkdll PRIVATE /export:DetourFinishHelperProcess,@1,NONAME)
target_compile_options(fmtkdll PRIVATE /EHa)

set_target_properties(fmtkdll PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
