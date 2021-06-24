#include <filesystem>
#include <string>
#include <list>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include <fmtksdk/fmtksdk.hpp>
#include <sol/sol.hpp>

sol::state lua;

struct CommandName
{
	std::string longName;
	std::string shortName;

	CommandName(const std::string& name)
	{
		for (char c : name)
		{
			longName.push_back(std::toupper(c));
			if (std::isupper(c))
			{
				shortName.push_back(c);
			}
		}
	}

	bool operator==(const char* str) const
	{
		std::string name = str;
		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });
		return (name == longName || name == shortName);
	}

	bool operator==(const std::string& str) const
	{
		std::string name = str;
		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::toupper(c); });
		return (name == longName || name == shortName);
	}

	bool operator==(const CommandName& c) const
	{
		return (c.longName == longName && c.shortName == shortName);
	}
};

// TODO: It's late and I'm tired. There is probably a better way to store these.
std::list<std::pair<CommandName, sol::protected_function>> commandCallbacks;

bool GenericCommandCallback(int argc, const char** argv)
{
	std::string name = argv[0];
	auto commandPair = std::find_if(commandCallbacks.begin(), commandCallbacks.end(), [&](const std::pair<CommandName, sol::protected_function>& x) -> bool { return x.first == name; });
	if (commandPair != commandCallbacks.end())
	{
		std::vector<std::reference_wrapper<const char*>> v(argv, argv + argc);
		auto result = commandPair->second(v);
		if (!result.valid())
		{
			sol::error err = result;
			fmtk->Log(LogLevel::ERR, "FMTKLua", err.what());
		}
	}

	return false;
}

void RegisterCommand(const std::string& cmd, sol::function fn)
{
	commandCallbacks.push_back({ cmd, fn });
	fmtk->RegisterCommand(cmd.c_str(), GenericCommandCallback);
}

void UnregisterCommand(const std::string& name)
{
	std::string capsName = name;
	for (char& c : capsName) c = std::toupper(c);
	auto newEnd = std::remove_if(commandCallbacks.begin(), commandCallbacks.end(), [&](const std::pair<CommandName, sol::function>& x) -> bool { return x.first == capsName; });
	commandCallbacks.erase(newEnd, commandCallbacks.end());
	fmtk->UnregisterCommand(name.c_str());
}

enum class Event
{
	INITIALIZE,
	TICK,
	SCRIPT_MANAGER_INITIALIZE,
	SHUTDOWN,
};

std::unordered_map<Event, std::list<std::pair<std::string, sol::protected_function>>> hooks;

void Hook(Event event, const std::string& id, sol::protected_function fn)
{
	hooks[event].push_back({ id, fn });
}

void Unhook(Event event, const std::string& id)
{
	std::list<std::pair<std::string, sol::protected_function>>& eventHooks = hooks[event];
	auto newEnd = std::remove_if(eventHooks.begin(), eventHooks.end(), [&](const std::pair<std::string, sol::protected_function>& x) -> bool { return x.first == id; });
	eventHooks.erase(newEnd, eventHooks.end());
}

#define BROADCAST(event, ...) \
	for (const std::pair<std::string, sol::protected_function>& pair : hooks[Event::event]) \
	{ \
		auto result = pair.second(__VA_ARGS__); \
		if (!result.valid()) \
		{ \
			sol::error err = result; \
			fmtk->Log(LogLevel::ERR, "FMTKLua", err.what()); \
		} \
	}

class FMTKLua : FMTKMod
{
public:
	virtual void Initialize() override
	{
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::jit, sol::lib::utf8);

		std::string modsDirectory(fmtk->GetModsDirectoryPath());

		const std::string& package_path = lua["package"]["path"];
		lua["package"]["path"] = modsDirectory + "\\lua\\share\\?.lua;" + modsDirectory + "\\lua\\?.lua;" + package_path;

		const std::string& package_cpath = lua["package"]["cpath"];
		lua["package"]["cpath"] = modsDirectory + "\\lua\\share\\?.dll;" + package_cpath;

		auto fmtk_table = lua["fmtk"].get_or_create<sol::table>();

		fmtk_table.new_enum("event",
			"INITIALIZE", Event::INITIALIZE,
			"TICK", Event::TICK,
			"SCRIPT_MANAGER_INITIALIZE", Event::SCRIPT_MANAGER_INITIALIZE,
			"SHUTDOWN", Event::SHUTDOWN
		);

		fmtk_table.set_function("hook", Hook);
		fmtk_table.set_function("unhook", Unhook);

		fmtk_table.set_function("get_fmtklua_version", [&]() -> sol::table { return lua.create_table_with("major", FMTK_VERSION_MAJOR, "minor", FMTK_VERSION_MINOR, "patch", FMTK_VERSION_PATCH, "tweak", FMTK_VERSION_TWEAK); });
		fmtk_table.set_function("run_command", [&](const std::string& cmd) { fmtk->RunCommand(cmd.c_str()); });
		fmtk_table.set_function("register_command", RegisterCommand);
		fmtk_table.set_function("unregister_command", UnregisterCommand);
		fmtk_table.set_function("get_player_position",
			[&]()
			{
				float* vec;
				if (vec = fmtk->GetPlayerPosition())
				{
					return lua.create_table_with("x", vec[0], "z", vec[1], "y", vec[2]);
				}
				return lua.create_table_with("x", 0, "z", 0, "y", 0);
			});

		auto fmtk_log_table = lua["fmtk"]["log"].get_or_create<sol::table>();
		fmtk_log_table.set_function("trace", [&](const std::string& source, const std::string& msg) { fmtk->Log(LogLevel::TRACE, source.c_str(), msg.c_str()); });
		fmtk_log_table.set_function("debug", [&](const std::string& source, const std::string& msg) { fmtk->Log(LogLevel::DEBUG, source.c_str(), msg.c_str()); });
		fmtk_log_table.set_function("info", [&](const std::string& source, const std::string& msg) { fmtk->Log(LogLevel::INFO, source.c_str(), msg.c_str()); });
		fmtk_log_table.set_function("warn", [&](const std::string& source, const std::string& msg) { fmtk->Log(LogLevel::WARN, source.c_str(), msg.c_str()); });
		fmtk_log_table.set_function("error", [&](const std::string& source, const std::string& msg) { fmtk->Log(LogLevel::ERR, source.c_str(), msg.c_str()); });
		fmtk_log_table.set_function("critical", [&](const std::string& source, const std::string& msg) { fmtk->Log(LogLevel::CRITICAL, source.c_str(), msg.c_str()); });

		auto fmtk_fs_table = lua["fmtk"]["fs"].get_or_create<sol::table>();
		fmtk_fs_table.set_function("alias", fmtk->Alias);
		fmtk_fs_table.set_function("unalias", fmtk->Unalias);
		fmtk_fs_table.set_function("get_mods_directory_path", [&]() { modsDirectory; });
		fmtk_fs_table.set_function("get_current_file_path",
			[&](sol::this_state ts) -> std::string
			{
				lua_State* L = ts;
				lua_Debug ar;

				int level = 1;
				int pre_stack_size = lua_gettop(L);
				if (lua_getstack(L, level, &ar) != 1) {
					// failure: call it quits
					fmtk->Log(LogLevel::TRACE, "FMTKLua", "error: unable to traverse the stack");
					lua_settop(L, pre_stack_size);
					return "";
				}

				if (lua_getinfo(L, "fnluS", &ar) == 0)
				{
					fmtk->Log(LogLevel::TRACE, "FMTKLua", "get_current_file_path failed");
					return "";
				}

				if (ar.source[0] == '@')
				{
					return std::filesystem::absolute(ar.source + 1).string();
				}
				else
				{
					fmtk->Log(LogLevel::TRACE, "FMTKLua", "get_current_file_path not a file");
					return "";
				}
			});

		std::function<void(std::filesystem::path)> autorun;
		autorun = [&](std::filesystem::path dir)
		{
			for (auto it : std::filesystem::directory_iterator(dir))
			{
				if (it.is_directory() && it.path().filename().string()[0] != '_')
				{
					autorun(it.path());
				}
				else if (it.path().filename() == "autorun.lua")
				{
					auto result = lua.safe_script_file(it.path().string(), sol::script_pass_on_error);
					if (!result.valid())
					{
						sol::error error = result;
						fmtk->Log(LogLevel::ERR, "FMTKLua", error.what());
					}
				}
			}
		};

		autorun(modsDirectory + "\\lua");

		BROADCAST(INITIALIZE);
	}

	virtual void Tick() override
	{
		BROADCAST(TICK);
	}

	virtual void ScriptManagerInitialize() override
	{
		BROADCAST(SCRIPT_MANAGER_INITIALIZE);
	}

	virtual void Shutdown() override
	{
		BROADCAST(SHUTDOWN);
	}
};

REGISTER_MOD(FMTKLua);
