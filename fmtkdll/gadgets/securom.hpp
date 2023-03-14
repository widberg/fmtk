//$ symbol fuel::securom, "int (__stdcall *)()";
//$ tag fuel::securom, hook;
//$ address fuel::securom, [retail], @0x008443C0;

//$ symbol fuel::securom_buffer, "std::uint8_t*";
//$ address fuel::securom_buffer, [retail], @0x00A44BE0;

#define SECUROM_BUFFER_LEN (76u)

int __stdcall wrap_fuel_securom() {
    std::memset(real_fuel_securom_buffer, 0xFF, SECUROM_BUFFER_LEN);
    return 1;
}

//$ symbol user32::SendMessageA, "LRESULT (WINAPI *)(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)";
//$ tag user32::SendMessageA, hook;
//$ address user32::SendMessageA, [*], !user32::SendMessageA;

LRESULT WINAPI wrap_user32_SendMessageA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (hWnd == (HWND)0xFFFFFFFF && Msg == (UINT)0xFFFFFFFF && wParam == (WPARAM)0x00000000 && lParam == (LPARAM)0x00000000)
    {
        return 0;
    }

    return real_user32_SendMessageA(hWnd, Msg, wParam, lParam);
}
