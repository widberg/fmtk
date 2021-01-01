#include <Windows.h>
#include <detours.h>
#include <sol/sol.hpp>

#include "logging.hpp"

#define ATTACH(x)       DetourAttach(&(PVOID&)Real_##x, FMTK_##x, #x)
#define DETACH(x)       DetourDetach(&(PVOID&)Real_##x, FMTK_##x, #x)

LONG AttachDetours(VOID)
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    PVOID* ppbFailedPointer = NULL;
    LONG error = DetourTransactionCommitEx(&ppbFailedPointer);
    if (error) {
        printf("traceapi.dll: Attach transaction failed to commit. Error %ld (%p/%p)",
            error, ppbFailedPointer, *ppbFailedPointer);
        return error;
    }
    return 0;
}

LONG DetachDetours(VOID)
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if (DetourTransactionCommit() != 0) {
        PVOID* ppbFailedPointer = NULL;
        LONG error = DetourTransactionCommitEx(&ppbFailedPointer);

        printf("traceapi.dll: Detach transaction failed to commit. Error %ld (%p/%p)",
            error, ppbFailedPointer, *ppbFailedPointer);
        return error;
    }
    return 0;
}

BOOL ProcessEnumerate()
{
    Syelog(SYELOG_SEVERITY_INFORMATION,
        "######################################################### Binaries\n");

    PBYTE pbNext;
    for (PBYTE pbRegion = (PBYTE)0x10000;; pbRegion = pbNext) {
        MEMORY_BASIC_INFORMATION mbi;
        ZeroMemory(&mbi, sizeof(mbi));

        if (VirtualQuery((PVOID)pbRegion, &mbi, sizeof(mbi)) <= 0) {
            break;
        }
        pbNext = (PBYTE)mbi.BaseAddress + mbi.RegionSize;

        // Skip free regions, reserver regions, and guard pages.
        //
        if (mbi.State == MEM_FREE || mbi.State == MEM_RESERVE) {
            continue;
        }
        if (mbi.Protect & PAGE_GUARD || mbi.Protect & PAGE_NOCACHE) {
            continue;
        }
        if (mbi.Protect == PAGE_NOACCESS) {
            continue;
        }

        // Skip over regions from the same allocation...
        {
            MEMORY_BASIC_INFORMATION mbiStep;

            while (VirtualQuery((PVOID)pbNext, &mbiStep, sizeof(mbiStep)) > 0) {
                if ((PBYTE)mbiStep.AllocationBase != pbRegion) {
                    break;
                }
                pbNext = (PBYTE)mbiStep.BaseAddress + mbiStep.RegionSize;
                mbi.Protect |= mbiStep.Protect;
            }
        }

        WCHAR wzDllName[MAX_PATH];
        PIMAGE_NT_HEADERS pinh = NtHeadersForInstance((HINSTANCE)pbRegion);

        if (pinh &&
            Real_GetModuleFileNameW((HINSTANCE)pbRegion, wzDllName, ARRAYSIZE(wzDllName))) {

            Syelog(SYELOG_SEVERITY_INFORMATION,
                "### %p..%p: %ls\n", pbRegion, pbNext, wzDllName);
        }
        else {
            Syelog(SYELOG_SEVERITY_INFORMATION,
                "### %p..%p: State=%04x, Protect=%08x\n",
                pbRegion, pbNext, mbi.State, mbi.Protect);
        }
    }
    Syelog(SYELOG_SEVERITY_INFORMATION, "###\n");

    LPVOID lpvEnv = Real_GetEnvironmentStrings();
    Syelog(SYELOG_SEVERITY_INFORMATION, "### Env= %08x [%08x %08x]\n",
        lpvEnv, ((PVOID*)lpvEnv)[0], ((PVOID*)lpvEnv)[1]);

    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
//
// DLL module information
//
BOOL ThreadAttach(HMODULE hDll)
{
    UNREFERENCED_PARAMETER(hDll);

    return TRUE;
}

BOOL ThreadDetach(HMODULE hDll)
{
    UNREFERENCED_PARAMETER(hDll);

    return TRUE;
}

BOOL ProcessAttach(HMODULE hDll)
{
    ThreadAttach(hDll);

    DetourRestoreAfterWith();

    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        AllocConsole();
    }

    LOG_INIT();
    LOG(trace, CORE, "trcapi" DETOURS_STRINGIFY(DETOURS_BITS) ".dll:"
        " DllMain DLL_PROCESS_ATTACH\n");

    WCHAR wzExeName[MAX_PATH];

    s_hInst = hDll;
    Real_GetModuleFileNameW(hDll, s_wzDllPath, ARRAYSIZE(s_wzDllPath));
    Real_GetModuleFileNameW(NULL, wzExeName, ARRAYSIZE(wzExeName));
    StringCchPrintfA(s_szDllPath, ARRAYSIZE(s_szDllPath), "%ls", s_wzDllPath);

    SyelogOpen("trcapi" DETOURS_STRINGIFY(DETOURS_BITS), SYELOG_FACILITY_APPLICATION);
    ProcessEnumerate();

    LONG error = AttachDetours();
    if (error != NO_ERROR) {
        Syelog(SYELOG_SEVERITY_FATAL, "### Error attaching detours: %d\n", error);
    }

    return TRUE;
}

BOOL ProcessDetach(HMODULE hDll)
{
    ThreadDetach(hDll);

    LONG error = DetachDetours();
    if (error != NO_ERROR) {
        Syelog(SYELOG_SEVERITY_FATAL, "### Error detaching detours: %d\n", error);
    }

    Syelog(SYELOG_SEVERITY_NOTICE, "### Closing.\n");

    return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
    UNREFERENCED_PARAMETER(hModule);
    UNREFERENCED_PARAMETER(lpReserved);

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        return ProcessAttach(hModule);
    case DLL_PROCESS_DETACH:
        return ProcessDetach(hModule);
    case DLL_THREAD_ATTACH:
        return ThreadAttach(hModule);
    case DLL_THREAD_DETACH:
        return ThreadDetach(hModule);
    default:
        break;
    }

    return TRUE;
}
//
///////////////////////////////////////////////////////////////// End of File.
