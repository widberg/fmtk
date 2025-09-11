#include <fmtksdk/fmtksdk.hpp>
#include <lua.h>
#include <lualib.h>
#include <LuaBridge/LuaBridge.h>

class FMTKLua : FMTKMod
{
	public:
	virtual void Initialize() override
	{
		L = luaL_newstate();
		if (!L) {
			fmtk->Log(LogLevel::ERR, "FMTKLua", "Failed to init luau");
			return;
		}
		
		luaL_openlibs(L);

		luabridge::getGlobalNamespace(L)
			.beginNamespace("fmtk")
			.endNamespace();
	}
	
	virtual void Tick() override
	{
	}
	
	virtual void ScriptManagerInitialize() override
	{
	}

	virtual void Shutdown() override
	{
	}
private:
	lua_State* L;
};

REGISTER_MOD(FMTKLua);
