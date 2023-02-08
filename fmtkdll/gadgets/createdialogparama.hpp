//$ symbol user32::CreateDialogParamA, "HWND (WINAPI *)(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)";
//$ tag user32::CreateDialogParamA, hook;
//$ address user32::CreateDialogParamA, [*], !user32::CreateDialogParamA;

HWND WINAPI wrap_user32_CreateDialogParamA(HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	HWND result = real_user32_CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	//ShowWindow(result, 1);
	return result;
}
