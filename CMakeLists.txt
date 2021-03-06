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

project(fmtk VERSION 0.1.0.0 LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17 CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")
set(CMAKE_GENERATOR_PLATFORM Win32 CACHE INTERNAL "")
set(CMAKE_VS_PLATFORM_NAME Win32 CACHE INTERNAL "")
set(CMAKE_VS_PLATFORM_NAME_DEFAULT Win32 CACHE INTERNAL "")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" CACHE INTERNAL "")
set_property(GLOBAL PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE INTERNAL "")

add_subdirectory(dependencies)
find_package(FUEL REQUIRED)
find_package(D3D9 REQUIRED)

add_subdirectory(common)
add_subdirectory(secudll)
add_subdirectory(fmtkdll)
add_subdirectory(fmtk)

add_subdirectory(dpc)
add_subdirectory(dpcc)
add_subdirectory(crc32)
add_subdirectory(crc32gpu)
add_subdirectory(lz)
add_subdirectory(noesis_fuel_pc)

add_custom_target(package COMMAND
    ${CMAKE_COMMAND} -E tar "cf" "${CMAKE_CURRENT_BINARY_DIR}/fmtk-${fmtk_VERSION_MAJOR}_${fmtk_VERSION_MINOR}_${fmtk_VERSION_PATCH}_${fmtk_VERSION_TWEAK}.zip" --format=zip
       "$<TARGET_FILE:fmtk>"
	   "$<TARGET_FILE:fmtkdll>"
	   "$<TARGET_FILE:secudll>"
	WORKING_DIRECTORY "$<TARGET_FILE_DIR:fmtk>"
	DEPENDS fmtk
	BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/fmtk-${fmtk_VERSION_MAJOR}_${fmtk_VERSION_MINOR}_${fmtk_VERSION_PATCH}_${fmtk_VERSION_TWEAK}.zip"
)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT fmtk)
