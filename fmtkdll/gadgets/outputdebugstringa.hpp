//$ symbol kernel32::OutputDebugStringA, "void (WINAPI *)(LPCSTR lpOutputString)";
//$ tag kernel32::OutputDebugStringA, hook;
//$ address kernel32::OutputDebugStringA, [], !kernel32::OutputDebugStringA;

FUNCTION(OutputDebugStringA, OutputDebugStringA, void, WINAPI, LPCSTR lpOutputString)
{
	LOG(debug, FUEL, "{}", lpOutputString);
}
