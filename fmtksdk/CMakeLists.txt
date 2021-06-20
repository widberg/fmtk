add_library(fmtksdk INTERFACE
	include/fmtksdk/fmtksdk.hpp.in
	include/fmtksdk/placeholder.hpp.in
)

target_include_directories(fmtksdk INTERFACE
	${CMAKE_CURRENT_BINARY_DIR}/include
)

file(READ include/fmtksdk/placeholder.hpp.in FMTK_PLACEHOLDER_HPP_IN)
configure_file(include/fmtksdk/fmtksdk.hpp.in include/fmtksdk/fmtksdk.hpp @ONLY)

configure_file(CMakeListsRedist.txt.in CMakeLists.txt @ONLY)
