add_library(fmtkdll SHARED
	fmtkdll.cpp
	instrumented.cpp
	win32def.hpp fmtkdef.hpp
)
target_link_libraries(fmtkdll common detours spdlog luajit sol2)
target_link_options(fmtkdll PRIVATE /export:DetourFinishHelperProcess,@1,NONAME)

set_target_properties(fmtkdll PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
