#include <Windows.h>
#include <detours.h>
#include <WinBase.h>
#include <string>
#include <toml++/toml.h>
#include <optional>
#include <debug.hpp>
#include <iostream>
#include <shellapi.h>
#include <sstream>
#include <vector>

#define ATTACH(x)       DetourAttach(&(PVOID&)Real_##x, FMTK_##x)
#define DETACH(x)       DetourDetach(&(PVOID&)Real_##x, FMTK_##x)

BOOL(WINAPI* Real_CreateProcessW)(LPCWSTR a0,
    LPWSTR a1,
    LPSECURITY_ATTRIBUTES a2,
    LPSECURITY_ATTRIBUTES a3,
    BOOL a4,
    DWORD a5,
    LPVOID a6,
    LPCWSTR a7,
    LPSTARTUPINFOW a8,
    LPPROCESS_INFORMATION a9)
        = CreateProcessW;

BOOL WINAPI FMTK_CreateProcessW(
    LPCWSTR               lpApplicationName,
    LPWSTR                lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCWSTR               lpCurrentDirectory,
    LPSTARTUPINFOW        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
)
{
    BOOL rv = DetourCreateProcessWithDllExW(
        lpApplicationName,
        lpCommandLine,
        lpProcessAttributes,
        lpThreadAttributes,
        bInheritHandles,
        dwCreationFlags,
        lpEnvironment,
        lpCurrentDirectory,
        lpStartupInfo,
        lpProcessInformation,
        "mods/fmtk/fmtkdll.dll",
        Real_CreateProcessW);
    
    return rv;
}

BOOL(WINAPI* Real_CreateProcessA)(LPCSTR a0,
    LPSTR a1,
    LPSECURITY_ATTRIBUTES a2,
    LPSECURITY_ATTRIBUTES a3,
    BOOL a4,
    DWORD a5,
    LPVOID a6,
    LPCSTR a7,
    LPSTARTUPINFOA a8,
    LPPROCESS_INFORMATION a9)
    = CreateProcessA;

BOOL WINAPI FMTK_CreateProcessA(
    LPCSTR               lpApplicationName,
    LPSTR                lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL                  bInheritHandles,
    DWORD                 dwCreationFlags,
    LPVOID                lpEnvironment,
    LPCSTR               lpCurrentDirectory,
    LPSTARTUPINFOA        lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
)
{
    FMTK_ASSERT(false, "CreateProcessA used by SecuLauncher. This is unexpected < Did you pirate the game? :( >. CreateProcessW expected. Please report this assertion to an FMTK developer.");
    return FALSE;
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
        DetourRestoreAfterWith();

        if (!AttachConsole(ATTACH_PARENT_PROCESS))
        {
            AllocConsole();
        }

        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        ATTACH(CreateProcessW);
        ATTACH(CreateProcessA);

        DetourTransactionCommit();

        return TRUE;
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DETACH(CreateProcessW);
        DETACH(CreateProcessA);

        DetourTransactionCommit();

        return TRUE;
    case DLL_THREAD_ATTACH:
        return TRUE;
    case DLL_THREAD_DETACH:
        return TRUE;
    default:
        break;
    }

    return TRUE;
}
