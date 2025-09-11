//$ symbol advapi32::RegQueryValueExW, "LSTATUS (WINAPI *)(HKEY hKey, LPCWSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData)";
//$ tag advapi32::RegQueryValueExW, hook;
//$ address advapi32::RegQueryValueExW, [*], !advapi32::RegQueryValueExW;

LSTATUS WINAPI wrap_advapi32_RegQueryValueExW(
  HKEY    hKey,
  LPCWSTR lpValueName,
  LPDWORD lpReserved,
  LPDWORD lpType,
  LPBYTE  lpData,
  LPDWORD lpcbData)
{
    if (lpData && !wcscmp(lpValueName, L"VideoResolution"))
    {
        // DWORD width = 1280;
        // DWORD height = 720;
        DWORD width = 1920;
        DWORD height = 1080;
        *(DWORD*)lpData = (height << 16) | width;
        return ERROR_SUCCESS;
    }
    return real_advapi32_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
