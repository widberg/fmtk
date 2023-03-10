#include "instrument.hpp"

#include <Windows.h>

//#include <dsound.h>

#include <filesystem>
#include <algorithm>
#include <cwctype>
#include <d3dx9shader.h>
#include <fstream>
#include <sinker/sinker.hpp>
using namespace sinker;

#include "fmtkdll.hpp"
#include "logging.hpp"
#include "sehexception.hpp"

//$ symbol fuel::pGlobalCommandState, "const void**";
//$ address fuel::pGlobalCommandState, [retail], @0x00a7c080;

#define SINKER_SYMBOL(module_name, symbol_name, symbol_type) \
	auto real_ ## module_name ## _ ## symbol_name = static_cast<symbol_type>(nullptr);
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
#include "gadgets/regqueryvalueexw.hpp"
#include "gadgets/securom.hpp"
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

    LOG(trace, FMTK, "Calculating addresses");

#define SINKER_SYMBOL(module_name, symbol_name, symbol_type) \
	auto module_name ## _ ## symbol_name ## _ ## calculated_address = \
	ctx.get_module(#module_name)->get_symbol(#symbol_name)->calculate_address<decltype(real_ ## module_name ## _ ## symbol_name)>(); \
	if (!module_name ## _ ## symbol_name ## _ ## calculated_address && ctx.get_module(#module_name)->get_symbol(#symbol_name)->get_attribute<bool>("required").value_or(true)) \
	{ \
    	LOG(critical, FMTK, "Calculate symbol \"" #module_name "_" #symbol_name "_calculated_address" "\" failed!"); \
		return 0; \
	} \
	if (module_name ## _ ## symbol_name ## _ ## calculated_address) \
	{ \
		LOG(trace, FMTK, "Calculate symbol \"" #module_name "_" #symbol_name "_calculated_address" "\" = {}", (void*)(module_name ## _ ## symbol_name ## _ ## calculated_address.value())); \
		real_ ## module_name ## _ ## symbol_name = module_name ## _ ## symbol_name ## _ ## calculated_address.value(); \
	}
#include "fmtk.def"

    LOG(trace, FMTK, "Attaching detours");

    Transaction transaction;

#define SINKER_TAG_hook_SYMBOL(module_name, symbol_name, symbol_type) \
	auto detour ## module_name ## _ ## symbol_name = Detour(real_ ## module_name ## _ ## symbol_name, wrap_ ## module_name ## _ ## symbol_name); \
	auto action ## module_name ## _ ## symbol_name = ActionInstall(&detour ## module_name ## _ ## symbol_name); \
	transaction.add(&action ## module_name ## _ ## symbol_name);
#include "fmtk.def"

    LOG(trace, FMTK, "Ready to commit");

    return transaction.commit();
}

LONG DetachDetours()
{
    LOG(trace, FMTK, "Detaching detours");

    Transaction transaction;

#define SINKER_TAG_hook_SYMBOL(module_name, symbol_name, symbol_type) \
	auto detour ## module_name ## _ ## symbol_name = Detour(real_ ## module_name ## _ ## symbol_name, wrap_ ## module_name ## _ ## symbol_name); \
	auto action ## module_name ## _ ## symbol_name = ActionUninstall(&detour ## module_name ## _ ## symbol_name); \
	transaction.add(&action ## module_name ## _ ## symbol_name);
#include "fmtk.def"

    LOG(trace, FMTK, "Ready to commit");

    return transaction.commit();
}
