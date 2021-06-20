#ifndef INSTRUMENT_HPP
#define INSTRUMENT_HPP

#include <string>

long AttachDetours();
long DetachDetours();

float* GetPlayerPosition();
bool RunCommand(const std::string& cmd);
void RegisterCommand(const std::string& name, bool(*callback)());

#define BROADCAST(name, ...) do { for (auto mod : mods) mod->name(__VA_ARGS__); } while(0)

#endif // !INSTRUMENT_HPP
