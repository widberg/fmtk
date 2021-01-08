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

#endif // !INSTRUMENT_HPP
