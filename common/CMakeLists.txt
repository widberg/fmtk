add_library(common INTERFACE
	include/configure.hpp.in
	include/debug.hpp
	include/logging.hpp
)

target_include_directories(common INTERFACE
	include
	${CMAKE_CURRENT_BINARY_DIR}/include
)

configure_file(include/configure.hpp.in include/configure.hpp @ONLY)
