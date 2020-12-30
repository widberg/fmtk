add_executable(fmtk
	fmtk.cpp
	configure.hpp.in configure.hpp
)

target_link_libraries(fmtk LuaJIT LuaJIT51 sol2 spdlog)
target_include_directories(fmtk PUBLIC "../dependencies/LuaJIT/src")

configure_file(configure.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/configure.hpp @ONLY)
set_target_properties(fmtk
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)
