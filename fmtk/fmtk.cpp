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
#include <codecvt>
#include <vector>

#include "debug.hpp"
#include "logging.hpp"

constexpr const char* FMTK_TOML_PATH = "fmtk.toml";

void LogLastError();
bool FirstTimeSetup();

int main(int argc, char** argv)
{
    LOG_INIT("fmtk.log");

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

    std::wstring arguments;
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    for (int i = 1; i < argc; ++i)
    {
        arguments += L'\"' + conv.from_bytes(argv[i]) + L"\" ";
    }

    std::optional<std::wstring> toml_args = tbl["fmtk"]["args"].value<std::wstring>();
    if (toml_args.has_value())
    {
        arguments += toml_args.value();
    }

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;

    std::wstringstream ss;

    ss << L"\"../../SecuLauncher.exe\" " << arguments;

    std::vector<wchar_t> buf;
    buf.resize(ss.str().size() + 1);
    ss.str().copy(buf.data(), ss.str().size());
    buf.back() = L'\0';

    LOG(info, FMTK, "{}", std::filesystem::absolute("../..").string());

    if (!DetourCreateProcessWithDllExW(NULL, buf.data(),
        NULL, NULL, TRUE, dwFlags, NULL,
        std::filesystem::absolute("../..").wstring().c_str(), &si, &pi, "mods/fmtk/secudll.dll", NULL))
    {
        LOG(error, FMTK, "DetourCreateProcessWithDllEx failed: {}\n", GetLastError());
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
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpBuffer, 0, NULL);

        LOG(error, FMTK, "{}", lpBuffer);

        LocalFree(lpBuffer);
    }
}

bool FirstTimeSetup()
{
    LOG(info, FMTK, "Performing first time setup");

    bool success = true;
    auto tbl = toml::table{{
       { "fmtk", toml::table{{
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
