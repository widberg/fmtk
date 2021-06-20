#include <filesystem>
#include <string>

#include <fmtksdk/fmtksdk.hpp>
#include <sol/sol.hpp>

sol::state lua;

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
		fmtk_table.set_function("print", [&](const std::string& msg) { fmtk->print(msg.c_str()); });
		fmtk_table.set_function("get_fmtklua_version", [&]() -> sol::table { return lua.create_table_with("major", FMTK_VERSION_MAJOR, "minor", FMTK_VERSION_MINOR, "patch", FMTK_VERSION_PATCH, "tweak", FMTK_VERSION_TWEAK); });

		auto fmtk_fs_table = lua["fmtk"]["fs"].get_or_create<sol::table>();
		fmtk_fs_table.set_function("get_current_file_path",
			[&](sol::this_state ts) -> std::string
			{
				lua_State* L = ts;
				lua_Debug ar;

				int level = 1;
				int pre_stack_size = lua_gettop(L);
				if (lua_getstack(L, level, &ar) != 1) {
					// failure: call it quits
					fmtk->print("error: unable to traverse the stack");
					lua_settop(L, pre_stack_size);
					return "";
				}

				if (lua_getinfo(L, "fnluS", &ar) == 0)
				{
					fmtk->print("get_current_file_path failed");
					return "";
				}

				if (ar.source[0] == '@')
				{
					return std::filesystem::absolute(ar.source + 1).string();
				}
				else
				{
					fmtk->print("get_current_file_path not a file");
					return "";
				}
			});

		auto result = lua.safe_script_file(modsDirectory + "\\lua\\test.lua", sol::script_pass_on_error);

		if (!result.valid()) {
			sol::error err = result;
			fmtk->print(err.what());
		}
	}

	virtual void Tick() override
	{
		static unsigned int timer = 0;
		if (timer >= 1000)
		{
			float* vec;
			if (vec = fmtk->GetPlayerPosition())
			{
				std::string s = "X: " + std::to_string(vec[0]) + ", Z: " + std::to_string(vec[1]) + ", Y: " + std::to_string(vec[2]);
				fmtk->print(s.c_str());
			}
			timer = 0;
		}
		else
		{
			timer++;
		}
	}
};

REGISTER_MOD(FMTKLua);
