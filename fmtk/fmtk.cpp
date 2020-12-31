#include <Windows.h>
#include <detours.h>
#include <tchar.h>
#include <sstream>
#include <stdio.h>

#include "debug.hpp"
#include "logging.hpp"

void LogLastError();

int main()
{
    LOG_INIT();

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    SecureZeroMemory(&si, sizeof(si));
    SecureZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    // Add fmtk.exe directory to FUEL's PATH to it can access fmtkdll.dll
    TCHAR lpModuleFileName[MAX_PATH + 1];
    GetModuleFileName(nullptr, lpModuleFileName, _countof(lpModuleFileName));
    *_tcsrchr(lpModuleFileName, '\\') = '\0';

    std::stringstream path;
    path << TEXT("PATH=") << getenv(TEXT("PATH")) << TEXT(";") << lpModuleFileName;

    std::string environment = path.str();
    LPSTR lpEndOfEnvironment = reinterpret_cast<LPSTR>(environment.data() + environment.size());
    environment.resize(environment.size() + sizeof(TCHAR) * 2);
    lpEndOfEnvironment[0] = static_cast<TCHAR>(0);
    lpEndOfEnvironment[1] = static_cast<TCHAR>(0);

    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

#ifdef UNICODE
    dwFlags |= CREATE_UNICODE_ENVIRONMENT;
#endif

    LPSTR lpCommand = "";

    if (!DetourCreateProcessWithDllEx("SecuLauncher.exe", lpCommand, nullptr, nullptr,
        TRUE, dwFlags, reinterpret_cast<LPVOID>(environment.data()), nullptr,
        &si, &pi, "fmtkdll.dll", nullptr))
    {
        DWORD dwError = GetLastError();
        LOG(error, CORE, "DetourCreateProcessWithDllEx failed: {}", dwError);
        if (dwError == ERROR_INVALID_HANDLE) {
#if DETOURS_64BIT
            LOG(error, CORE, "Can't detour a 32-bit target process from a 64-bit parent process.");
#else
            LOG(error, CORE, "Can't detour a 64-bit target process from a 32-bit parent process.");
#endif
        }
        else
        {
            LogLastError();
        }
        return 9009;
    }

    ResumeThread(pi.hThread);

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD dwResult = 0;
    if (!GetExitCodeProcess(pi.hProcess, &dwResult)) {
        LogLastError();
        return 9010;
    }
    else
    {
        LOG(trace, CORE, "FUEL.exe exited with code: {}", dwResult);
        LogLastError();
    }

    return dwResult;
}

void LogLastError()
{
    DWORD dwError = GetLastError();
    if (dwError)
    {
        LOG(error, CORE, "GetExitCodeProcess failed: {}", dwError);

        LPSTR lpBuffer = nullptr;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpBuffer, 0, NULL);

        LOG(error, CORE, "{}", lpBuffer);

        LocalFree(lpBuffer);
    }
}
