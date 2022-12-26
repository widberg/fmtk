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

# sol2
add_subdirectory(sol2)

# spdlog
add_subdirectory(spdlog)
target_compile_definitions(spdlog PUBLIC "SPDLOG_WCHAR_TO_UTF8_SUPPORT" "SPDLOG_DISABLE_DEFAULT_LOGGER")

# tomlplusplus
add_subdirectory(tomlplusplus)

# usercall.hpp
SET(USERCALL_HPP_CHECK_RETURN           ON CACHE BOOL "")
SET(USERCALL_HPP_USE_HEXRAYS_DEFS       ON CACHE BOOL "")
SET(USERCALL_HPP_USE_REALLY_SHORT_NAMES ON CACHE BOOL "")
add_subdirectory(usercall.hpp)
