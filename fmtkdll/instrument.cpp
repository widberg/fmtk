#include "instrument.hpp"

#include <Windows.h>
#include <detours.h>

//#include <dsound.h>

#include <filesystem>
#include <algorithm>
#include <cwctype>
#include <d3dx9shader.h>
#include <fstream>
#include <sinker.hpp>

#include "fmtkdll.hpp"
#include "logging.hpp"
#include "sehexception.hpp"

#define FUNCTION(name, address, returnType, callingConvention, ...) \
returnType (callingConvention * Real_##name)(__VA_ARGS__)           \
	= reinterpret_cast<decltype(Real_##name)>(address);             \
returnType callingConvention FMTK_##name(__VA_ARGS__)

#define ATTACH(x)       DetourAttach(&(PVOID&)Real_##x, FMTK_##x)
#define DETACH(x)       DetourDetach(&(PVOID&)Real_##x, FMTK_##x)

//$ symbol fuel::pGlobalCommandState, "const void**";
//$ address fuel::pGlobalCommandState, [retail], @0x00a7c080;
const void** pGlobalCommandState = reinterpret_cast<const void**>(0x00a7c080);

#define SINKER_SYMBOL(module_name, symbol_name, symbol_type) \
	auto FUEL_ ## module_name ## _ ## symbol_name = static_cast<symbol_type>(nullptr);
#include "fmtk.def"

#include "gadgets/createfilew.hpp"
#include "gadgets/registercommand.hpp"
#include "gadgets/scriptmanagerinit.hpp"
#include "gadgets/winmain.hpp"
#include "gadgets/coremainloop.hpp"
#include "gadgets/outputdebugstringa.hpp"
#include "gadgets/outputdebugstringw.hpp"
#include "gadgets/runcommand.hpp"
#include "gadgets/d3dxcompileshaderfromfilea.hpp"
#include "gadgets/validatememory.hpp"
#include "gadgets/createwindowexw.hpp"
#include "gadgets/getplayerposition.hpp"
#include "gadgets/readfile.hpp"
#include "gadgets/createdialogparama.hpp"
//#include "gadgets/idirectsoundbufferplay.hpp"

// Instrument
LONG AttachDetours()
{
    LOG(trace, FMTK, "Concretizing modules");

	sinker::Context ctx;
	std::ifstream in("mods/fmtk/fmtk.skr", std::ios::binary);
	if (!in.good())
	{
    	LOG(critical, FMTK, "Can't open sinker file!");
		return 0;
	}

	if (!ctx.interpret(in, sinker::Language::SINKER, "fmtk.skr"))
	{
    	LOG(critical, FMTK, "Interpreting sinker file failed!");
		return 0;
	}

#define SINKER_MODULE(module_name) \
	if (!ctx.get_module(#module_name)->concretize()) \
	{ \
    	LOG(critical, FMTK, "Concretizing module \"" #module_name "\" failed!"); \
		return 0; \
	}
#include "fmtk.def"

    LOG(trace, FMTK, "Attaching detours");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

#define SINKER_TAG_hook_SYMBOL(module_name, symbol_name, symbol_type) \
	auto module_name ## _ ## symbol_name ## _ ## calculated_address = \
	ctx.get_module(#module_name)->get_symbol(#symbol_name)->calculate_address<symbol_type>(); \
	if (!module_name ## _ ## symbol_name ## _ ## calculated_address && ctx.get_module(#module_name)->get_symbol(#symbol_name)->get_attribute<bool>("required").value_or(true)) \
	{ \
    	LOG(critical, FMTK, "Calculate symbol \"" #module_name "_" #symbol_name "_calculated_address" "\" failed!"); \
		return 0; \
	} \
	ATTACH(symbol_name);
#include "fmtk.def"

	//HINSTANCE hiDSOUND = GetModuleHandleA("dsound.dll");

	//ATTACHDSOUND(IDirectSoundBuffer_Play);
	//ATTACHDSOUND(IDirectSoundBuffer_SetVolume);
	//ATTACHDSOUND(IDirectSoundBuffer_Release);
	//ATTACHDSOUND(IDirectSound_CreateSoundBuffer);

    LOG(trace, FMTK, "Ready to commit");

    return DetourTransactionCommit();
}

LONG DetachDetours()
{
    LOG(trace, FMTK, "Detaching detours");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

#define SINKER_TAG_hook_SYMBOL(module_name, symbol_name, symbol_type) \
	DETACH(symbol_name);
#include "fmtk.def"

	//DETACHXLIVE(ValidateMemory);

	//DETACHDSOUND(IDirectSoundBuffer_Play);
	//DETACHDSOUND(IDirectSoundBuffer_SetVolume);
	//DETACHDSOUND(IDirectSoundBuffer_Release);
	//DETACHDSOUND(IDirectSound_CreateSoundBuffer);

    LOG(trace, FMTK, "Ready to commit");

    return DetourTransactionCommit();
}
