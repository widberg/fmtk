//$ symbol kernel32::OutputDebugStringW, "void (WINAPI *)(LPCWSTR lpOutputString)";
//$ tag kernel32::OutputDebugStringW, hook;
//$ address kernel32::OutputDebugStringW, [*], !kernel32::OutputDebugStringW;

void WINAPI wrap_kernel32_OutputDebugStringW(LPCWSTR lpOutputString)
{
	LOGW(debug, FUEL, "{}", lpOutputString);
}
