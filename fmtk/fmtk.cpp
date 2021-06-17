#include <windows.h>
#include <detours.h>
#include <stdio.h>
#include <strsafe.h>
#include <tchar.h>
#include <sstream>
#include <string>
#include <filesystem>
#include <WinBase.h>
#include <fstream>
#include <toml++/toml.h>
#include <optional>

#include "debug.hpp"
#include "logging.hpp"

constexpr const char* FMTK_TOML_PATH = "fmtk.toml";

void LogLastError();
bool FirstTimeSetup();

int main(int argc, char** argv)
{
    LOG_INIT();

    if (!std::filesystem::exists(FMTK_TOML_PATH))
    {
        if (!FirstTimeSetup())
        {
            return -1;
        }
    }

    toml::table tbl;
    try
    {
        tbl = toml::parse_file(FMTK_TOML_PATH);
    }
    catch (const toml::parse_error& err)
    {
        LOG(error, FMTK, "TOML Parsing failed:\n{}", err.what());
        return -2;
    }

    std::optional<std::wstring> secuLauncherPath = tbl["fmtk"]["secu_launcher"].value<std::wstring>();
    if (!secuLauncherPath.has_value())
    {
        LOG(error, FMTK, "No SecuLauncher path in toml");
        return -3;
    }

    std::optional<std::string> modsPath = tbl["fmtk"]["mods_directory"].value<std::string>();
    if (!modsPath.has_value())
    {
        LOG(error, FMTK, "No mods directory path in toml");
        return -4;
    }

    try
    {
        std::filesystem::create_directories(modsPath.value());
    }
    catch (const std::exception& e)
    {
        LOG(error, FMTK, "{}", e.what());
        return -5;
    }

#if FMTK_DEBUG
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
#endif // FMTK_DEBUG

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

    if (!DetourCreateProcessWithDllExW(secuLauncherPath.value().c_str(), NULL,
        NULL, NULL, TRUE, dwFlags,
#if FMTK_DEBUG
        reinterpret_cast<LPVOID>(environment.str().data()),
#else
        NULL,
#endif // FMTK_DEBUG
        NULL, &si, &pi, "secudll.dll", NULL))
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

bool FirstTimeSetup()
{
    LOG(info, FMTK, "Performing first time setup");

    bool success = true;

    std::wstring fuelExecutablePath;
    if (std::filesystem::exists("FUEL.exe"))
    {
        fuelExecutablePath = L"FUEL.exe";
    }
    else
    {
        LOG(error, FMTK, "Could not locate the FUEL executable. Please edit fmtk.ini.");
        success = false;
    }

    std::wstring gameSetupExecutablePath;
    if (std::filesystem::exists("GameSetup.exe"))
    {
        gameSetupExecutablePath = L"GameSetup.exe";
    }
    else
    {
        LOG(error, FMTK, "Could not locate the GameSetup executable. Please edit fmtk.ini.");
        success = false;
    }

    std::wstring secuLauncherPath;
    if (std::filesystem::exists("SecuLauncher.exe"))
    {
        secuLauncherPath = L"SecuLauncher.exe";
    }
    else
    {
        LOG(error, FMTK, "Could not locate the SecuLauncher executable. Please edit fmtk.ini.");
        success = false;
    }

    auto tbl = toml::table{{
       { "fmtk", toml::table{{
               { "fuel", fuelExecutablePath },
               { "game_setup", gameSetupExecutablePath },
               { "secu_launcher", secuLauncherPath },
               { "mods_directory", "mods" },
               { "args", toml::array{} },
           }}
       },
    }};

    std::ofstream toml_file(FMTK_TOML_PATH);
    if (!toml_file.good())
    {
        return false;
    }

    toml_file << tbl << "\n";

    return success;
}
