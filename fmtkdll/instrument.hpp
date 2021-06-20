#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <Windows.h>
#include <string>

LONG AttachDetours();
LONG DetachDetours();

bool RunCommand(const std::string& cmd);
extern const void** pGlobalCommandState;

float* GetPlayerPosition();

#define BROADCAST(name, ...) do { for (auto mod : mods) mod->name(__VA_ARGS__); } while(0)

#endif // !INSTRUMENT_HPP
