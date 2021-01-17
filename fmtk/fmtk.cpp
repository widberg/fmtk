#include <windows.h>
#include <detours.h>
#include <stdio.h>
#include <strsafe.h>
#include <tchar.h>
#include <sstream>
#include <string>

#include "debug.hpp"
#include "logging.hpp"

void LogLastError();

int CDECL main(int argc, char** argv)
{
    LOG_INIT();

    std::stringstream environment;

    LPCH lpEnvironmentString = GetEnvironmentStrings();
    LPCH lpEnvironmentStringVariable = lpEnvironmentString;

    while (true)
    {
        std::string variable(lpEnvironmentStringVariable);
        if (variable.empty())
        {
            break;
        }
        else if (variable.find("Path") == 0)
        {
            TCHAR lpModuleFileName[MAX_PATH + 1];
            GetModuleFileName(nullptr, lpModuleFileName, _countof(lpModuleFileName));
            *_tcsrchr(lpModuleFileName, '\\') = '\0';

            environment << variable << TEXT(";") << lpModuleFileName;
        }
        else
        {
            environment << lpEnvironmentStringVariable;
        }

        environment.write("\0", 1);

        lpEnvironmentStringVariable += variable.size() + 1;
    }

    environment.write("\0", 1);

    FreeEnvironmentStrings(lpEnvironmentString);

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

    if (!DetourCreateProcessWithDllExW(L"SecuLauncher.exe", NULL,
        NULL, NULL, TRUE, dwFlags, reinterpret_cast<LPVOID>(environment.str().data()), NULL,
        &si, &pi, "secudll.dll", NULL))
    {
        LOG(error, FMTK, "DetourCreateProcessWithDllEx failed: %ld\n", GetLastError());
        LogLastError();
        return 9009;
    }

    LOG(trace, FMTK, "Resuming SecuLauncher");

    ResumeThread(pi.hThread);

    LOG(trace, FMTK, "SecuLauncher resumed");

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwResult = 0;
    if (!GetExitCodeProcess(pi.hProcess, &dwResult)) {
        LOG(error, FMTK, "GetExitCodeProcess failed: %ld\n", GetLastError());
        LogLastError();
        return 9010;
    }
    else
    {
        LOG(trace, FMTK, "SecuLauncher exited with code: 0x{0:x}", dwResult);
    }

    return dwResult;
}

void LogLastError()
{
    DWORD dwError = GetLastError();
    if (dwError)
    {
        LPSTR lpBuffer = nullptr;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpBuffer, 0, NULL);

        LOG(error, FMTK, "{}", lpBuffer);

        LocalFree(lpBuffer);
    }
}
