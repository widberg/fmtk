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
    // if (!wcscmp(lpValueName, L"VideoResolution"))
    // {
    //     if (lpData)
    //     {
    //         DWORD width = 600;
    //         DWORD height = 400;
    //         *(DWORD*)lpData = (height << 16) | width;
    //         return ERROR_SUCCESS;
    //     }
    // }
    return real_advapi32_RegQueryValueExW(hKey, lpValueName, lpReserved, lpType, lpData, lpcbData);
}
