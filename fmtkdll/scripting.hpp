#ifndef SCRIPTING_HPP
#define SCRIPTING_HPP

#include <sol/sol.hpp>

extern sol::state lua;

bool ScriptingInit();
bool ScriptingShutdown();

enum EventType
{
	ENTRY,
	EXIT,
	TICK,
	DEATH,
	COMMAND_INIT,
	LOAD,
	LOAD_DATA,
	LOAD_SHADERS,
};

void ScriptingEvent(EventType eventType);

#endif // !SCRIPTING_HPP
