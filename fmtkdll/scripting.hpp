#ifndef SCRIPTING_HPP
#define SCRIPTING_HPP

#include <sol/sol.hpp>
#include <unordered_map>

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

bool FMTKEmitEventCallback();
extern std::unordered_map<std::string, EventType> eventNames;

#endif // !SCRIPTING_HPP
