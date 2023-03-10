//$ symbol fuel::securom, "int (__stdcall *)()";
//$ tag fuel::securom, hook;
//$ address fuel::securom, [retail], @0x008443C0;

//$ symbol fuel::securom_buffer, "std::uint8_t*";
//$ address fuel::securom_buffer, [retail], @0x00A44BE0;

#define SECUROM_BUFFER_LEN (76u)
// #define NORMAL

int __stdcall wrap_fuel_securom() {
#ifdef NORMAL
  HANDLE hFile = OpenFileMappingW(4u, 0, L"-=[SMS_FUEL.exe_SMS]=-");
  if (hFile) {
    LPVOID data = MapViewOfFile(hFile, 4u, 0, 0, 0);
    if (data) {
      SetLastError(1u);
      std::memcpy(real_fuel_securom_buffer, data, SECUROM_BUFFER_LEN);
      UnmapViewOfFile(data);
      CloseHandle(hFile);
      return 1;
    }
    SetLastError(0xFFFFFFFD);
    return 0;
  }
  SetLastError(0xFFFFFFFE);
  return 0;
#else
    std::memset(real_fuel_securom_buffer, 0xFF, SECUROM_BUFFER_LEN);
    return 1;
#endif
}

//$ symbol user32::SendMessageA, "LRESULT (WINAPI *)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)";
//$ tag user32::SendMessageA, hook;
//$ address user32::SendMessageA, [*], !user32::SendMessageA;

LRESULT WINAPI wrap_user32_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (hWnd == (HWND)0xFFFFFFFF && Msg == (UINT)0xFFFFFFFF && wParam == (WPARAM)0x00000000 && lParam == (LPARAM)0xFFFFFFFF)
    {
        return 0;
    }

    return real_user32_SendMessageA(hWnd, Msg, wParam, lParam);
}
