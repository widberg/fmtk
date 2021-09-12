FUNCTION(CreateDialogParamA, CreateDialogParamA, HWND, WINAPI, HINSTANCE hInstance, LPCSTR lpTemplateName, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam)
{
	HWND result = Real_CreateDialogParamA(hInstance, lpTemplateName, hWndParent, lpDialogFunc, dwInitParam);
	ShowWindow(result, 1);
	return result;
}