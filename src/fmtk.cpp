#include <sol/sol.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <cassert>

#include <string>
#include <vector>
#include <unordered_map>

#include "configure.hpp"

#define LOG(level, source, format, ...) { spdlog::##level("[" + std::string(#source) + "] " ##format, __VA_ARGS__); }

enum CallbackType
{
    Tick
};

int main() {
    spdlog::set_pattern("%^[%L] %v%$");
    spdlog::set_level(spdlog::level::trace);
    LOG(trace, CORE, "Initializing FUEL Modding Toolkit v{}", FMTK_VERSION);
    std::unordered_map<CallbackType, std::vector<sol::function>> callbacks;
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::string, sol::lib::io, sol::lib::jit, sol::lib::bit32, sol::lib::ffi, sol::lib::utf8, sol::lib::debug, sol::lib::math);
}
