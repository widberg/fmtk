# CLI11
add_subdirectory(CLI11)

# D3D9
add_subdirectory(d3d9)

# Detours
add_subdirectory(detours)

# hashlibpp
add_subdirectory(hashlibpp)

# LuaJIT
add_subdirectory(luajit)

# re2c
add_subdirectory(re2c)

# sol2
add_subdirectory(sol2)

# spdlog
add_subdirectory(spdlog)
target_compile_definitions(spdlog PUBLIC "SPDLOG_WCHAR_TO_UTF8_SUPPORT" "SPDLOG_DISABLE_DEFAULT_LOGGER")

# tomlplusplus
add_subdirectory(tomlplusplus)

# winflexbison
# add_subdirectory(winflexbison)
