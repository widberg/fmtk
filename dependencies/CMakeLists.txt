# Detours
add_subdirectory(detours)

# json
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)

# LuaJIT
add_subdirectory(luajit)

# sol2
add_subdirectory(sol2)

# spdlog
add_subdirectory(spdlog)
target_compile_definitions(spdlog PUBLIC "SPDLOG_WCHAR_TO_UTF8_SUPPORT" "SPDLOG_DISABLE_DEFAULT_LOGGER")

# tomlplusplus
add_subdirectory(tomlplusplus)
