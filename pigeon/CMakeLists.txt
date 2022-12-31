


add_executable(pigeon
	pigeon.cpp
	pigeon.yy
)
target_link_libraries(pigeon CLI11::CLI11)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re ${CMAKE_CURRENT_SOURCE_DIR}/pigeon.cpp
	COMMAND bison ${CMAKE_CURRENT_SOURCE_DIR}/pigeon.yy -o ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re -Wcounterexamples
	COMMAND re2c ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re -o ${CMAKE_CURRENT_SOURCE_DIR}/pigeon.cpp --no-debug-info
	DEPENDS pigeon.yy
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

include(cmake/add_pigeon_target.cmake)

add_pigeon_target(pig
	INPUT ${CMAKE_BINARY_DIR}/test.api
	OUTPUT ${CMAKE_BINARY_DIR}/out.h
)
