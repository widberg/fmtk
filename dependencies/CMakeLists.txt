# CLI11
if(NOT TARGET CLI11::CLI11)
    add_subdirectory(CLI11 EXCLUDE_FROM_ALL SYSTEM)
endif()

# D3D9
add_subdirectory(d3d9 EXCLUDE_FROM_ALL SYSTEM)

# Discord GameSDK
add_subdirectory(discord_game_sdk EXCLUDE_FROM_ALL SYSTEM)

# LuaBridge3
add_subdirectory(LuaBridge3 EXCLUDE_FROM_ALL SYSTEM)

# Luau
set(LUAU_BUILD_CLI OFF CACHE BOOL "" FORCE)
set(LUAU_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LUAU_STATIC_CRT ON CACHE BOOL "" FORCE)
add_subdirectory(luau EXCLUDE_FROM_ALL SYSTEM)

# sinker
add_subdirectory(sinker EXCLUDE_FROM_ALL SYSTEM)

# spdlog
add_subdirectory(spdlog EXCLUDE_FROM_ALL SYSTEM)
target_compile_definitions(spdlog PUBLIC "SPDLOG_WCHAR_TO_UTF8_SUPPORT" "SPDLOG_DISABLE_DEFAULT_LOGGER")

# tomlplusplus
add_subdirectory(tomlplusplus EXCLUDE_FROM_ALL SYSTEM)
