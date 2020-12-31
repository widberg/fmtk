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

project(fmtk VERSION 0.1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_GENERATOR_PLATFORM Win32)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_subdirectory(dependencies)
find_package(FUEL REQUIRED)

add_subdirectory(common)
add_subdirectory(fmtkdll)
add_subdirectory(fmtk)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT fmtk)
