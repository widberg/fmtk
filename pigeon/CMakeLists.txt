add_executable(pigeon
	pigeon.yy
	${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp
)
target_include_directories(pigeon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pigeon CLI11::CLI11)
add_dependencies(pigeon win_bison re2c)

add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp
	COMMAND $<TARGET_FILE:win_bison> ${CMAKE_CURRENT_SOURCE_DIR}/pigeon.yy -o ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re -Wcounterexamples
	COMMAND $<TARGET_FILE:re2c> ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp.re -o ${CMAKE_CURRENT_BINARY_DIR}/pigeon.cpp --no-debug-info
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pigeon.yy
)

include(cmake/add_pigeon_target.cmake)

add_pigeon_target(pig
	GLOBAL pig
	INPUT ${CMAKE_BINARY_DIR}/test.api
	OUTPUT ${CMAKE_BINARY_DIR}/out.h
)
