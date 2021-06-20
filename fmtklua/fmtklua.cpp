#include <string>

#include <fmtksdk/fmtksdk.hpp>
#include <sol/sol.hpp>

sol::state lua;

class FMTKLua : FMTKMod
{
public:
	virtual void Initialize()
	{
		fmtk->print("TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST");

		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::os, sol::lib::math, sol::lib::table, sol::lib::debug, sol::lib::bit32, sol::lib::io, sol::lib::ffi, sol::lib::jit, sol::lib::utf8);

		const std::string& package_path = lua["package"]["path"];
		lua["package"]["path"] = "D:\\programming\\widberg\\fmtk\\build\\bin\\mods\\lua\\share\\?.lua;" + package_path;

		const std::string& package_cpath = lua["package"]["cpath"];
		lua["package"]["cpath"] = "D:\\programming\\widberg\\fmtk\\build\\bin\\mods\\lua\\share\\?.dll;" + package_cpath;

		auto fmtk_table = lua["fmtk"].get_or_create<sol::table>();
		fmtk_table.set_function("print", [&](const std::string& msg) { fmtk->print(msg.c_str()); });

		auto result = lua.safe_script("socket = require('socket')\nfmtk.print(socket._VERSION)", sol::script_pass_on_error);

		if (!result.valid()) {
			sol::error err = result;
			fmtk->print(err.what());
		}
	}
};

REGISTER_MOD(FMTKLua);