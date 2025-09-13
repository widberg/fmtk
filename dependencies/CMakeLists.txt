# FUELDecompilation
set(FUEL_DECOMPILATION_NAME "fmtk" CACHE STRING "" FORCE)
set(USE_NON_MATCHING ON CACHE BOOL "" FORCE)
set(USE_FMTK ON CACHE BOOL "" FORCE)
add_subdirectory(FUELDecompilation)

# Discord GameSDK
add_subdirectory(discord_game_sdk EXCLUDE_FROM_ALL SYSTEM)

# LuaBridge3
add_subdirectory(LuaBridge3 EXCLUDE_FROM_ALL SYSTEM)

# Luau
set(LUAU_BUILD_CLI OFF CACHE BOOL "" FORCE)
set(LUAU_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(LUAU_STATIC_CRT ON CACHE BOOL "" FORCE)
add_subdirectory(luau EXCLUDE_FROM_ALL SYSTEM)

# spdlog
set(SPDLOG_WCHAR_TO_UTF8_SUPPORT ON CACHE BOOL "" FORCE)
add_subdirectory(spdlog EXCLUDE_FROM_ALL SYSTEM)
