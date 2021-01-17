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

#endif // !INSTRUMENT_HPP
