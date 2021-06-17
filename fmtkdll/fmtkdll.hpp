#ifndef FMTKDLL_HPP
#define FMTKDLL_HPP

#include <string>
#include <unordered_map>

#include <fmtksdk/fmtksdk.hpp>

extern std::unordered_map<std::wstring, std::wstring> aliases;
extern std::vector<FMTKMod*> mods;

#define ALL_DETOURS

#define ATTACH_ALL() \
	DEF(DETOUR_NAME, ATTACH) \
	ALL_DETOURS \
	UNDEF(DETOUR_NAME)

#define DETACH_ALL() \
	DEF(DETOUR_NAME, DETACH) \
	ALL_DETOURS \
	UNDEF(DETOUR_NAME)

#define ADD_DETOUR(name) DEF(ALL_DETOURS, ALL_DETOURS DETOUR_NAME(name);)

#endif // !FMTKDLL_HPP
