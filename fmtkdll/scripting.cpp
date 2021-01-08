#include "scripting.hpp"

#include <algorithm>
#include <cwctype>
#include <filesystem>
#include <sol/sol.hpp>
#include <unordered_map>
#include <vector>

#include "fmtkdll.hpp"
#include "logging.hpp"
#include "instrument.hpp"

std::unordered_map<EventType, std::vector<sol::function>> hooks;

void alias(std::wstring originalPath, std::wstring newPath)
{
	LOGW(trace, CORE, "Aliasing {} as {}", originalPath, newPath);

	originalPath = std::filesystem::absolute(originalPath);
	std::transform(originalPath.begin(), originalPath.end(), originalPath.begin(), std::towlower);

	newPath = std::filesystem::absolute(newPath);
	std::transform(newPath.begin(), newPath.end(), newPath.begin(), std::towlower);

	aliases[originalPath] = newPath;
}

void hook(EventType eventType, sol::function function)
{
	LOG(trace, CORE, "Hook added for {}", eventType);
	hooks[eventType].push_back(function);
}

bool run_command(std::string command)
{
	LOG(trace, CORE, "Running command: {}", command);

	return Real_RunCommand(*pGlobalCommandState, command.c_str(), CommandSource::GAME);
}

std::string get_current_file_path(sol::this_state ts)
{
	return "";
}

void ScriptingEvent(EventType eventType)
{
	for (sol::function function : hooks[eventType])
	{
		LOG(trace, CORE, "Calling function for {}", eventType);

		auto result = function();
		LOG(trace, CORE, "Error checking");
		if (!result.valid())
		{
			sol::error error = result;
			LOG(critical, CORE, "Lua error: {}", error.what());
			FMTK_ASSERT(false, "lua error");
		}

		LOG(trace, CORE, "Ran successfully");
	}
}

sol::state lua;

bool ScriptingInit()
{
	LOG(trace, CORE, "Opening lua libraries");

	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::jit, sol::lib::utf8);

	LOG(trace, CORE, "Creating fmtk table");

	auto fmtk_table = lua["fmtk"].get_or_create<sol::table>();
	fmtk_table.set_function("alias", &alias);
	fmtk_table.set_function("hook", &hook);
	fmtk_table.set_function("run_command", &run_command);
	fmtk_table.set_function("get_current_file_path", &get_current_file_path);

	lua.set("FMTK_EVENT_ENTRY", EventType::ENTRY);
	lua.set("FMTK_EVENT_EXIT", EventType::EXIT);
	lua.set("FMTK_EVENT_TICK", EventType::TICK);
	lua.set("FMTK_EVENT_DEATH", EventType::DEATH);
	lua.set("FMTK_EVENT_LOAD", EventType::LOAD);
	
	LOG(trace, CORE, "Autorun");

	std::function<void(std::filesystem::path)> autorun;
	autorun = [&](std::filesystem::path dir)
	{
		LOG(trace, CORE, "Autorun dir: {}", dir.string());

		lua["FMTK_CURRENT_DIR"] = dir.string();

		for (auto it : std::filesystem::directory_iterator(dir))
		{
			if (it.is_directory() && it.path().filename().string().c_str()[0] != '_')
			{
				autorun(it.path());
			}
			else if (it.path().filename() == "autorun.lua")
			{
				LOG(trace, CORE, "Autorun script: {}", it.path().string());

				auto result = lua.safe_script_file(it.path().string(), sol::script_pass_on_error);
				LOG(trace, CORE, "Error checking");
				if (!result.valid())
				{
					sol::error error = result;
					LOG(critical, CORE, "Lua error: {}", error.what());
					FMTK_ASSERT(false, "lua error");
				}

				LOG(trace, CORE, "Ran successfully");
			}
		}
	};

	std::filesystem::path modDirectory = std::filesystem::absolute("mods");
	autorun(modDirectory);
	
	lua["FMTK_CURRENT_DIR"] = sol::lua_nil;

	return true;
}

bool ScriptingShutdown()
{
	ScriptingEvent(EventType::EXIT);
	return true;
}
