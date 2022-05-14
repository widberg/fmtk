cmake_minimum_required(VERSION 3.15)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the detours source code and call cmake from there")
endif()

if (POLICY CMP0048)
  cmake_policy(SET CMP0048 NEW)
endif()

if (POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

project(detours VERSION 4.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(detours STATIC
	Detours/src/creatwth.cpp
    Detours/src/detours.cpp
    Detours/src/detours.h
    Detours/src/detver.h
    Detours/src/disasm.cpp
    Detours/src/disolarm.cpp
    Detours/src/disolarm64.cpp
    Detours/src/disolia64.cpp
    Detours/src/disolx64.cpp
    Detours/src/disolx86.cpp
    Detours/src/image.cpp
    Detours/src/modules.cpp
	Detours/src/uimports.cpp
)

# This file is included and not compiled on its own
set_source_files_properties(Detours/src/uimports.cpp PROPERTIES
	HEADER_FILE_ONLY true
)

target_include_directories(detours PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/debug)
	
target_include_directories(detours PUBLIC Detours/src)
