cmake_minimum_required(VERSION 3.15)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the fmtk source code and call cmake from there")
endif()

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if (POLICY CMP0091)
	cmake_policy(SET CMP0091 NEW)
endif()

include(cmake/widberg-toolchain.cmake)

project(fmtk VERSION 0.0.7.0 LANGUAGES CXX)

option(FMTK_DOCS        "Build the documentation." OFF)

set(CMAKE_CXX_STANDARD 17 CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
add_definitions(-DUNICODE -D_UNICODE)
add_compile_options("/clang:-fdebug-prefix-map=${PROJECT_SOURCE_DIR}=.")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" CACHE INTERNAL "")

find_package(FUEL)
add_subdirectory(dependencies)

add_subdirectory(fmtksdk)
add_subdirectory(common)
add_subdirectory(fmtklua)
add_subdirectory(fmtkdiscord)
add_subdirectory(fmtkdll)
add_subdirectory(fmtk)
add_subdirectory(package_fmtk)

if(FMTK_DOCS)
	add_subdirectory(docs)
endif()
