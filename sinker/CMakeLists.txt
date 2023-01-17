add_library(sinker STATIC
	sinker.cpp
	sinker.hpp
	sinker.tpp
	parser.yy
	${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
)
target_include_directories(sinker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_dependencies(sinker win_bison re2c)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp.re ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
	COMMAND $<TARGET_FILE:win_bison> ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy -o ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp.re -Wcounterexamples
	COMMAND $<TARGET_FILE:re2c> ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp.re -o ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp --no-debug-info
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/parser.yy
)

add_executable(sinkerc
		main.cpp
)
target_link_libraries(sinkerc sinker CLI11::CLI11)

include(cmake/add_sinker_target.cmake)

add_sinker_target(sink
	INPUT ${CMAKE_BINARY_DIR}/test.skr
	OUTPUT ${CMAKE_BINARY_DIR}/out.skr
	DEFINITIONS ${CMAKE_BINARY_DIR}/out.def
)
