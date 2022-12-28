add_executable(sinker
		sinker.yy
		sinker.hpp
		sinker.cpp # Let's check this into vc so that re2c and bison aren't required dependencies.
		             # I know codegen'd files shouldn't get checked in but re2c and bison are a pain
		             # to setup on windows.
		main.cpp
		impl.cpp
)
target_link_libraries(sinker CLI11::CLI11)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re ${CMAKE_CURRENT_SOURCE_DIR}/sinker.cpp
	COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/sinker.yy -o ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re -Wcounterexamples
	COMMAND re2c ${CMAKE_CURRENT_BINARY_DIR}/sinker.cpp.re -o ${CMAKE_CURRENT_SOURCE_DIR}/sinker.cpp --no-debug-info
	DEPENDS sinker.yy
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include(cmake/add_sinker_target.cmake)

add_sinker_target(sink
	INPUT ${CMAKE_BINARY_DIR}/test.skr
	OUTPUT ${CMAKE_BINARY_DIR}/out.skr
	DEFINITIONS ${CMAKE_BINARY_DIR}/out.def
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
