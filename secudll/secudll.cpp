#include <Windows.h>
#include <detours.h>

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
    PROCESS_INFORMATION pi;
    if (lpProcessInformation == NULL) {
        ZeroMemory(&pi, sizeof(pi));
        lpProcessInformation = &pi;
    }

    BOOL rv = DetourCreateProcessWithDllExW(lpApplicationName,
        lpCommandLine,
        lpProcessAttributes,
        lpThreadAttributes,
        bInheritHandles,
        dwCreationFlags | CREATE_SUSPENDED,
        lpEnvironment,
        lpCurrentDirectory,
        lpStartupInfo,
        lpProcessInformation,
        "fmtkdll.dll",
        Real_CreateProcessW);

    ResumeThread(lpProcessInformation->hThread);

    return rv;
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

        DetourTransactionCommit();

        return TRUE;
    case DLL_PROCESS_DETACH:
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());

        DETACH(CreateProcessW);

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
