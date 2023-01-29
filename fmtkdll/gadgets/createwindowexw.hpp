//$ symbol user32::CreateWindowExW, "HANDLE (WINAPI *)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)";
// //$ tag user32::CreateWindowExW, hook;
//$ address user32::CreateWindowExW, [], !user32::CreateWindowExW;

FUNCTION(CreateWindowExW, CreateWindowExW, HWND, WINAPI,
	DWORD     dwExStyle,
	LPCWSTR   lpClassName,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam)
{
	LOG(trace, FMTK, "Creating window");

	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	return Real_CreateWindowExW(dwExStyle, lpClassName, lpWindowName, WS_POPUP, 0, 0, w, h, hWndParent, hMenu, hInstance, lpParam);
}
