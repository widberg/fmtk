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
	LOGW(trace, FMTK, "Aliasing {} as {}", originalPath, newPath);

	originalPath = std::filesystem::absolute(originalPath);
	std::transform(originalPath.begin(), originalPath.end(), originalPath.begin(), std::towlower);

	newPath = std::filesystem::absolute(newPath);
	std::transform(newPath.begin(), newPath.end(), newPath.begin(), std::towlower);

	aliases[originalPath] = newPath;
}

int hook(EventType eventType, sol::function function)
{
	LOG(trace, FMTK, "Hook added for {}", eventType);
	hooks[eventType].push_back(function);
	return 0;
}

bool run_command(std::string command)
{
	LOG(trace, FMTK, "Running command: {}", command);

	return Real_RunCommand(*pGlobalCommandState, command.c_str(), CommandSource::GAME);
}

std::string get_current_file_path(sol::this_state ts)
{
	return "";
}

std::unordered_map<std::string, sol::function> modCommands;

bool ModCommandCallback()
{
	const char** pArg0 = (const char**)((char*)*pGlobalCommandState + 0xa3b0);
	const char* commandName = *pArg0;

	if (modCommands.count(commandName))
	{
		DWORD argc = *(DWORD*)((char*)*pGlobalCommandState + 0x23ac);

		std::vector<std::string> argv;
		argv.resize(argc);

		for (int i = 0; i < argc; ++i)
		{
			argv.push_back(*(pArg0 + i));
		}

		sol::function func = modCommands.at(commandName);
		return func(argc, argv);
	}

	return false;
}

int register_command(std::string name, sol::function func)
{
	modCommands.insert({name, func});
	Bridge_RegisterCommand(*pGlobalCommandState, ModCommandCallback, name.c_str());

	return 0;
}

void ScriptingEmitEvent(EventType eventType)
{
	for (sol::function function : hooks[eventType])
	{
		LOG(trace, FMTK, "Calling function for {}", eventType);

		auto result = function();
		LOG(trace, FMTK, "Error checking");
		if (!result.valid())
		{
			sol::error error = result;
			LOG(critical, FMTK, "Lua error: {}", error.what());
			FMTK_ASSERT(false, "lua error");
		}

		LOG(trace, FMTK, "Ran successfully");
	}
}

#define EVENTNAME(name) { "FMTK_EVENT_" #name, EventType::##name }

std::unordered_map<std::string, EventType> eventNames = {
	EVENTNAME(ENTRY),
	EVENTNAME(EXIT),
	EVENTNAME(TICK),
	EVENTNAME(DEATH),
	EVENTNAME(COMMAND_INIT),
	EVENTNAME(LOAD),
	EVENTNAME(LOAD_DATA),
	EVENTNAME(LOAD_SHADERS),
};

bool FMTKEmitEventCallback()
{
	DWORD argc = *(DWORD*)((char*)*pGlobalCommandState + 0x23ac);
	if (argc < 2)
	{
		return false;
	}

	const char** pArg0 = (const char**)((char*)*pGlobalCommandState + 0xa3b0);
	const char* arg1 = *(pArg0 + 1);

	if (eventNames.count(arg1))
	{
		EventType eventType = eventNames.at(arg1);
		ScriptingEmitEvent(eventType);

		return true;
	}

	return false;
}

sol::state lua;

bool ScriptingInit()
{
	LOG(trace, FMTK, "Opening lua libraries");

	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::jit, sol::lib::utf8);

	LOG(trace, FMTK, "Creating fmtk table");

	auto fmtk_table = lua["fmtk"].get_or_create<sol::table>();
	fmtk_table.set_function("alias", &alias);
	fmtk_table.set_function("hook", &hook);
	fmtk_table.set_function("run_command", &run_command);
	fmtk_table.set_function("get_current_file_path", &get_current_file_path);
	fmtk_table.set_function("register_command", &register_command);

	for (auto it : eventNames)
	{
		lua.set(it.first, it.second);
	}
	
	LOG(trace, FMTK, "Autorun");

	std::function<void(std::filesystem::path)> autorun;
	autorun = [&](std::filesystem::path dir)
	{
		LOG(trace, FMTK, "Autorun dir: {}", dir.string());

		lua["FMTK_CURRENT_DIR"] = dir.string();

		for (auto it : std::filesystem::directory_iterator(dir))
		{
			if (it.is_directory() && it.path().filename().string().c_str()[0] != '_')
			{
				autorun(it.path());
			}
			else if (it.path().filename() == "autorun.lua")
			{
				LOG(trace, FMTK, "Autorun script: {}", it.path().string());

				auto result = lua.safe_script_file(it.path().string(), sol::script_pass_on_error);
				LOG(trace, FMTK, "Error checking");
				if (!result.valid())
				{
					sol::error error = result;
					LOG(critical, FMTK, "Lua error: {}", error.what());
					FMTK_ASSERT(false, "lua error");
				}

				LOG(trace, FMTK, "Ran successfully");
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
	ScriptingEmitEvent(EventType::EXIT);
	return true;
}
