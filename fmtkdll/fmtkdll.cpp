#include <Windows.h>
#include <algorithm>
#include <codecvt>
#include <cstring>
#include <cwctype>
#include <detours.h>
#include <locale>

#include <filesystem>
#include <optional>
#include <toml++/toml.h>

#include "fmtkdll.hpp"

#include "instrument.hpp"
#include "logging.hpp"

constexpr const char *FMTK_TOML_PATH = "mods/fmtk/fmtk.toml";
std::vector<FMTKMod *> mods;

void print(const char *msg) { LOG(trace, FMTK, "{}", msg); }

std::string fmtkModsDirectoryPath;

const char *GetModsDirectoryPath() { return fmtkModsDirectoryPath.c_str(); }

void Log(LogLevel level, const char *source, const char *msg) {
  switch (level) {
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

void Alias(const char *originalPath, const char *newPath) {
  std::wstring originalPathString =
      std::filesystem::absolute(originalPath).wstring();
  std::transform(originalPathString.begin(), originalPathString.end(),
                 originalPathString.begin(), std::towlower);

  std::wstring newPathString = std::filesystem::absolute(newPath).wstring();
  std::transform(newPathString.begin(), newPathString.end(),
                 newPathString.begin(), std::towlower);

  aliases[originalPathString] = newPathString;
}

void Unalias(const char *originalPath) {
  std::wstring originalPathString =
      std::filesystem::absolute(originalPath).wstring();
  std::transform(originalPathString.begin(), originalPathString.end(),
                 originalPathString.begin(), std::towlower);

  aliases.erase(originalPathString);
}

FMTKApi fmtkApi{
    GetModsDirectoryPath,
    GetPlayerPosition,
    RunCommand,
    RegisterCommand,
    UnregisterCommand,
    Log,
    Alias,
    Unalias,
};

bool loadModDll(const std::filesystem::path &modDllPath) {
  HMODULE handle = LoadLibraryA(modDllPath.string().c_str());
  if (!handle) {
    LOG(error, FMTK, "Could not LoadLibrary: {}, {}", modDllPath.string(), GetLastError());
    return false;
  }

  const FMTKVersion *(*GetFMTKVersion)() =
      reinterpret_cast<const FMTKVersion *(*)()>(
          GetProcAddress(handle, "GetFMTKVersion"));
  if (!handle) {
    LOG(error, FMTK, "Does not implement GetFMTKVersion: {}",
        modDllPath.string());
    return false;
  }

  const FMTKVersion *modFMTKVersion = GetFMTKVersion();
  if (FMTK_VERSION_MAJOR != modFMTKVersion->major ||
      FMTK_VERSION_MINOR != modFMTKVersion->minor ||
      FMTK_VERSION_PATCH != modFMTKVersion->patch ||
      FMTK_VERSION_TWEAK != modFMTKVersion->tweak) {
    LOG(error, FMTK,
        "The FMTK version and the mods FMTKSDK version must match exactly for "
        "the time being.");
    return false;
  }

  FMTKMod *(*GetMod)(const FMTKApi *fmtkApi) =
      reinterpret_cast<FMTKMod *(*)(const FMTKApi *fmtkApi)>(
          GetProcAddress(handle, "GetMod"));
  if (!handle) {
    LOG(error, FMTK, "Does not implement GetMod: {}", modDllPath.string());
    return false;
  }

  mods.push_back(GetMod(&fmtkApi));

  return true;
}

bool loadModsDirectory(const std::filesystem::path &modsDirectoryPath) {
  std::function<void(const std::filesystem::path &)> loadDirectory;
  loadDirectory = [&](const std::filesystem::path &dir) {
    LOG(trace, FMTK, "Load dir: {}", dir.string());

    for (auto it : std::filesystem::directory_iterator(dir)) {
      LOG(trace, FMTK, "Processing: {}", it.path().string());
      if (it.path().filename().string()[0] != '_') {
        if (it.is_directory()) {
          loadDirectory(it.path());
        } else if (it.path().extension() == ".dll") {
          LOG(trace, FMTK, "Load dll: {}", it.path().string());

          if (loadModDll(it.path())) {
            LOG(trace, FMTK, "Loaded successfully");
          } else {
            LOG(error, FMTK, "Failed to load: {}", it.path().string());
          }
        }
      }
    }
  };

  if (std::filesystem::exists(modsDirectoryPath)) {
    loadDirectory(std::filesystem::absolute(modsDirectoryPath));
  } else {
    LOG(error, FMTK, "The specified mods directory does not exist");
    return false;
  }

  return true;
}

std::unordered_map<std::wstring, std::wstring> aliases;

BOOL ProcessAttach(HMODULE hDll) {
  DetourRestoreAfterWith();

  if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
    AllocConsole();
  }

  LOG_INIT("mods/fmtk/fmtk.log");
  LOG(trace, FMTK, "Attaching to FUEL");

  LONG error = AttachDetours();
  if (error != NO_ERROR) {
    LOG(critical, FMTK, "Error attaching detours: {}", error);
  }

  LOG(trace, FMTK, "Attached to FUEL");

  toml::table tbl;
  try {
    tbl = toml::parse_file(FMTK_TOML_PATH);
  } catch (const toml::parse_error &err) {
    LOG(error, FMTK, "TOML Parsing failed:\n{}", err.what());
    return -2;
  }

  std::wstring modsDirectoryPath = L"mods";

  fmtkModsDirectoryPath =
      (std::filesystem::absolute(modsDirectoryPath) / "").string();

  SetDllDirectory((modsDirectoryPath + L"\\path").c_str());

  loadModsDirectory(modsDirectoryPath + L"\\native");

  return TRUE;
}

BOOL ProcessDetach(HMODULE hDll) {
  BROADCAST(Shutdown);

  LOG(trace, FMTK, "Detaching from FUEL");

  LONG error = DetachDetours();
  if (error != NO_ERROR) {
    LOG(critical, FMTK, "Error detaching detours: {}", error);
  }

  LOG(trace, FMTK, "Detatched from FUEL");

  return TRUE;
}

BOOL APIENTRY DllMain(HINSTANCE hModule, DWORD dwReason, PVOID lpReserved) {

  if (DetourIsHelperProcess()) {
    return TRUE;
  }

  switch (dwReason) {
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
