# CLI11
if(NOT TARGET CLI11::CLI11)
    add_subdirectory(CLI11)
endif()

# D3D9
add_subdirectory(d3d9)

# Discord GameSDK
add_subdirectory(discord_game_sdk)

# LuaJIT
set(BUILD_SHARED_LIBS ON CACHE PATH "LuaJIT Shared")
set(LUAJIT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/LuaJIT" CACHE PATH "LuaJIT directory")
add_subdirectory(luajit-cmake)
set_target_properties(libluajit PROPERTIES OUTPUT_NAME "lua5.1")

# re2c
if(NOT TARGET re2c)
    add_subdirectory(re2c)
endif()

# sinker
add_subdirectory(sinker)

# sol2
add_subdirectory(sol2)

# spdlog
add_subdirectory(spdlog)
target_compile_definitions(spdlog PUBLIC "SPDLOG_WCHAR_TO_UTF8_SUPPORT" "SPDLOG_DISABLE_DEFAULT_LOGGER")

# tomlplusplus
add_subdirectory(tomlplusplus)

# bison
find_package(BISON 3.4.1 REQUIRED)
