#include <Windows.h>
#include <detours.h>

#include "fmtkdll.hpp"

#include "instrument.hpp"
#include "scripting.hpp"
#include "logging.hpp"

std::unordered_map<std::wstring, std::wstring> aliases;

BOOL ProcessAttach(HMODULE hDll)
{
    DetourRestoreAfterWith();

    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        AllocConsole();
    }

    LOG_INIT();
    LOG(trace, CORE, "Attaching to FUEL");

    ScriptingInit();

    LONG error = AttachDetours();
    if (error != NO_ERROR)
    {
        LOG(critical, CORE, "Error attaching detours: {}", error);
    }

    LOG(trace, CORE, "Attached to FUEL");

    return TRUE;
}

BOOL ProcessDetach(HMODULE hDll)
{
    LOG(trace, CORE, "Detaching from FUEL");

    ScriptingShutdown();

    LONG error = DetachDetours();
    if (error != NO_ERROR)
    {
        LOG(critical, CORE, "Error detaching detours: {}", error);
    }


    LOG(trace, CORE, "Detatched from FUEL");

    return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);

    if (DetourIsHelperProcess())
    {
        return TRUE;
    }

    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        return ProcessAttach(hModule);
    case DLL_PROCESS_DETACH:
        return ProcessDetach(hModule);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        break;
    }

    return TRUE;
}
