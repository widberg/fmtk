//$ symbol kernel32::OutputDebugStringA, "void (WINAPI *)(LPCSTR lpOutputString)";
//$ tag kernel32::OutputDebugStringA, hook;
//$ address kernel32::OutputDebugStringA, [*], !kernel32::OutputDebugStringA;

void WINAPI wrap_kernel32_OutputDebugStringA(LPCSTR lpOutputString)
{
	LOG(debug, FUEL, "{}", lpOutputString);
}
