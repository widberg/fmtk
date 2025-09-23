# FUELDecompilation
set(USE_FMTK ON CACHE BOOL "" FORCE)
add_subdirectory(FUELDecompilation)
# The headers that come with DXSDK don't work with modern Windows SDKs
# https://stackoverflow.com/questions/66032294/directx-cmake-includes-break-windows-h
set_target_properties(DXSDK PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/d3d9_headers"
    SYSTEM TRUE
)

# Discord GameSDK
add_subdirectory(discord_game_sdk EXCLUDE_FROM_ALL SYSTEM)

# ImGui
add_subdirectory(imgui EXCLUDE_FROM_ALL SYSTEM)

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
