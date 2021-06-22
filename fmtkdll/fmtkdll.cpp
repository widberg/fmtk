#include <Windows.h>
#include <detours.h>
#include <cstring>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cwctype>

#include <filesystem>
#include <toml++/toml.h>
#include <optional>

#include "fmtkdll.hpp"

#include "instrument.hpp"
#include "logging.hpp"

constexpr const char* FMTK_TOML_PATH = "fmtk.toml";
std::vector<FMTKMod*> mods;

void print(const char* msg)
{
    LOG(trace, FMTK, "{}", msg);
}

std::string fmtkModsDirectoryPath;

const char* GetModsDirectoryPath()
{
    return fmtkModsDirectoryPath.c_str();
}

void Log(LogLevel level, const char* source, const char* msg)
{
    switch (level)
    {
    case LogLevel::TRACE:
        spdlog::trace("[" + std::string(source) + "] {}", msg);
        break;
    case LogLevel::DEBUG:
        spdlog::debug("[" + std::string(source) + "] {}", msg);
        break;
    case LogLevel::INFO:
        spdlog::info("[" + std::string(source) + "] {}", msg);
        break;
    case LogLevel::WARN:
        spdlog::warn("[" + std::string(source) + "] {}", msg);
        break;
    case LogLevel::ERR:
        spdlog::error("[" + std::string(source) + "] {}", msg);
        break;
    case LogLevel::CRITICAL:
        spdlog::critical("[" + std::string(source) + "] {}", msg);
        break;
    default:
        break;
    }
}

void Alias(const char* originalPath, const char* newPath)
{
    std::wstring originalPathString = std::filesystem::absolute(originalPath).wstring();
    std::transform(originalPathString.begin(), originalPathString.end(), originalPathString.begin(), std::towlower);

    std::wstring newPathString = std::filesystem::absolute(newPath).wstring();
    std::transform(newPathString.begin(), newPathString.end(), newPathString.begin(), std::towlower);

    aliases[originalPathString] = newPathString;
}

void Unalias(const char* originalPath)
{
    std::wstring originalPathString = std::filesystem::absolute(originalPath).wstring();
    std::transform(originalPathString.begin(), originalPathString.end(), originalPathString.begin(), std::towlower);

    aliases.erase(originalPathString);
}

FMTKApi fmtkApi
{
    GetModsDirectoryPath,
    GetPlayerPosition,
    RunCommand,
    RegisterCommand,
    UnregisterCommand,
    Log,
    Alias,
    Unalias,
};

bool loadModDll(const std::filesystem::path& modDllPath)
{
    HMODULE handle = LoadLibraryA(modDllPath.string().c_str());
    if (!handle)
    {
        LOG(error, FMTK, "Could not LoadLibrary: {}", modDllPath.string());
        return false;
    }

    const FMTKVersion* (*GetFMTKVersion)() = reinterpret_cast<const FMTKVersion* (*)()>(GetProcAddress(handle, "GetFMTKVersion"));
    if (!handle)
    {
        LOG(error, FMTK, "Does not implement GetFMTKVersion: {}", modDllPath.string());
        return false;
    }

    const FMTKVersion* modFMTKVersion = GetFMTKVersion();
    if (FMTK_VERSION_MAJOR != modFMTKVersion->major || FMTK_VERSION_MINOR != modFMTKVersion->minor ||
        FMTK_VERSION_PATCH != modFMTKVersion->patch || FMTK_VERSION_TWEAK != modFMTKVersion->tweak)
    {
        LOG(error, FMTK, "The FMTK version and the mods FMTKSDK version must match exactly for the time being.");
        return false;
    }

    FMTKMod* (*GetMod)(const FMTKApi * fmtkApi) = reinterpret_cast<FMTKMod * (*)(const FMTKApi * fmtkApi)>(GetProcAddress(handle, "GetMod"));
    if (!handle)
    {
        LOG(error, FMTK, "Does not implement GetMod: {}", modDllPath.string());
        return false;
    }

    mods.push_back(GetMod(&fmtkApi));

    return true;
}

bool loadModsDirectory(const std::filesystem::path& modsDirectoryPath)
{
    std::function<void(const std::filesystem::path&)> loadDirectory;
    loadDirectory = [&](const std::filesystem::path& dir)
    {
        LOG(trace, FMTK, "Load dir: {}", dir.string());

        for (auto it : std::filesystem::directory_iterator(dir))
        {
            LOG(trace, FMTK, "Processing: {}", it.path().string());
            if (it.path().filename().string()[0] != '_')
            {
                if (it.is_directory())
                {
                    loadDirectory(it.path());
                }
                else if (it.path().extension() == ".dll")
                {
                    LOG(trace, FMTK, "Load dll: {}", it.path().string());

                    if (loadModDll(it.path()))
                    {
                        LOG(trace, FMTK, "Loaded successfully");
                    }
                    else
                    {
                        LOG(error, FMTK, "Failed to load: {}", it.path().string());
                    }
                }
            }
        }
    };

    if (std::filesystem::exists(modsDirectoryPath))
    {
        loadDirectory(std::filesystem::absolute(modsDirectoryPath));
    }
    else
    {
        LOG(error, FMTK, "The specified mods directory does not exist");
        return false;
    }

    return true;
}

std::unordered_map<std::wstring, std::wstring> aliases;

BOOL ProcessAttach(HMODULE hDll)
{
#define FMTK_ABI_SIGNATURE_ASSERT(name ,address, ...) \
    BYTE signature_##address[] = { __VA_ARGS__ }; \
    FMTK_ASSERT(memcmp(signature_##address, (const void*)address, sizeof(signature_##address)) == 0, "Failed FMTK_ABI_SIGNATURE_ASSERT for " #name)

    FMTK_ABI_SIGNATURE_ASSERT(CoreMainLoop, 0x00688bf0,
        0x55, 0x8b, 0xec, 0x83, 0xe4, 0xf8, 0x51, 0x53, 0x56, 0x57,
        0xb9, 0x06, 0x00, 0x00, 0x00, 0xbe, 0xb4, 0x7b, 0x9c, 0x00,
        0xbf, 0xf8, 0xea, 0xa2, 0x00, 0xf3, 0xa5, 0x66, 0xa5, 0x33,
        0xdb, 0x38, 0x1d, 0x27, 0xc9, 0xa7, 0x00, 0xa4, 0x74, 0x7b,
        0xd9, 0x05, 0x2c, 0xc9, 0xa7, 0x00, 0xd8, 0x25, 0x60, 0xb6,
        0xa2, 0x00, 0xd9, 0x15, 0x2c, 0xc9, 0xa7, 0x00, 0xd9, 0xee,
        0xd8, 0xd1, 0xdf, 0xe0
    );

    DetourRestoreAfterWith();

    if (!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        AllocConsole();
    }

    LOG_INIT();
    LOG(trace, FMTK, "Attaching to FUEL");

    LONG error = AttachDetours();
    if (error != NO_ERROR)
    {
        LOG(critical, FMTK, "Error attaching detours: {}", error);
    }

    LOG(trace, FMTK, "Attached to FUEL");

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

    std::optional<std::wstring> modsDirectoryPath = tbl["fmtk"]["mods_directory"].value<std::wstring>();
    if (!modsDirectoryPath.has_value())
    {
        LOG(error, FMTK, "No mods directory in toml");
        return FALSE;
    }

    fmtkModsDirectoryPath = (std::filesystem::absolute(modsDirectoryPath.value()) / "").string();

    loadModsDirectory(modsDirectoryPath.value() + L"\\native");

    return TRUE;
}

BOOL ProcessDetach(HMODULE hDll)
{
    BROADCAST(Shutdown);

    LOG(trace, FMTK, "Detaching from FUEL");

    LONG error = DetachDetours();
    if (error != NO_ERROR)
    {
        LOG(critical, FMTK, "Error detaching detours: {}", error);
    }

    LOG(trace, FMTK, "Detatched from FUEL");

    return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved)
{
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
