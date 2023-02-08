add_library(fmtksdk INTERFACE)

target_sources(fmtksdk INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/include/fmtksdk/fmtksdk.hpp.in
	${CMAKE_CURRENT_SOURCE_DIR}/include/fmtksdk/placeholder.hpp.in
)

target_include_directories(fmtksdk INTERFACE
	${CMAKE_CURRENT_BINARY_DIR}/include
)

file(READ include/fmtksdk/placeholder.hpp.in FMTK_PLACEHOLDER_HPP_IN)
configure_file(include/fmtksdk/fmtksdk.hpp.in include/fmtksdk/fmtksdk.hpp @ONLY)

configure_file(CMakeListsRedist.txt.in CMakeLists.txt @ONLY)

add_pigeon_target(fmtksdk_pigeon
	INPUT fmtk.api
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fmtksdk_pigeon.hpp
	GLOBAL fmtk
)
