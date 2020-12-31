
#include <cstdio>
#include <Windows.h>
#include <detours.h>

static int (WINAPI * TruewWinMain)(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);

int WINAPI FMTKwWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    return TruewWinMain(hInstance, hPrevInstance, pCmdLine, nCmdShow);
}

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved)
{
    LONG error;
    UNREFERENCED_PARAMETER(hinst);
    UNREFERENCED_PARAMETER(reserved);

    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    if (dwReason == DLL_PROCESS_ATTACH)
    {
        printf("aaaaaa\n");

        DetourRestoreAfterWith();

        TruewWinMain = (int (WINAPI *)(HINSTANCE, HINSTANCE, PWSTR, int))DetourGetEntryPoint(NULL);

        DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
           // DetourAttach(&(PVOID&)TruewWinMain, FMTKwWinMain);
        error = DetourTransactionCommit();

    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            //DetourDetach(&(PVOID&)TruewWinMain, FMTKwWinMain);
        error = DetourTransactionCommit();
    }

    return TRUE;
}
