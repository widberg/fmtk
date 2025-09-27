cmake_minimum_required(VERSION 3.25)

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

set(CMAKE_BUILD_TYPE RelWithDebInfo) # Override build type for correct optimization flags
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generate compile_commands.json for clangd
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF) # Disable LTO
cmake_policy(SET CMP0091 NEW) # Enable MSVC runtime library selection
set(CMAKE_MSVC_RUNTIME_LIBRARY "") # Manually set runtime library to avoid warning
add_compile_options("/MT$<$<CONFIG:Debug>:d>" "$<$<COMPILE_LANGUAGE:CXX>:/GR>" "/GS-" "/clang:-msse" "/clang:-msse2" "/clang:-ffast-math" "/clang:-fno-finite-math-only") # Use static runtime libraries
add_link_options("/MANIFEST:NO") # Disable manifest generation
add_compile_definitions("_CRT_SECURE_NO_DEPRECATE" "_CRT_SECURE_NO_WARNINGS" "_WINSOCK_DEPRECATED_NO_WARNINGS") # Disable CRT/Winsock warnings

project(fmtk VERSION 0.0.7.0 LANGUAGES CXX)

option(FMTK_DOCS        "Build the documentation." OFF)

set(CMAKE_CXX_STANDARD 17 CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")
add_compile_options("/clang:-fdebug-prefix-map=${PROJECT_SOURCE_DIR}=.")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" CACHE INTERNAL "")

add_subdirectory(dependencies)

target_sources(defuel PRIVATE
  fmtk/fmtk.cpp
)
target_link_libraries(defuel PUBLIC imgui discord_game_sdk spdlog Luau.VM Luau.Compiler Luau.Common LuaBridge)

if(FMTK_DOCS)
	add_subdirectory(docs)
endif()
