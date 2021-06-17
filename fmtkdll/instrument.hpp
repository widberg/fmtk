#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <Windows.h>

LONG AttachDetours();
LONG DetachDetours();

enum CommandSource : int
{
	GAME,
	DPC,
	TSC,
};

extern bool(__stdcall * Real_RunCommand)(const void* pState, const char* cmd, CommandSource commandSource);
extern const void** pGlobalCommandState;

void Bridge_RegisterCommand(const void* pThis, void* callback, LPCSTR name);

#define BROADCAST(name, ...) do { for (auto mod : mods) mod->name(__VA_ARGS__); } while(0)

#endif // !INSTRUMENT_HPP
