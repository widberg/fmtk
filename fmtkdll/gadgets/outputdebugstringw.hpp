FUNCTION(OutputDebugStringW, OutputDebugStringW, void, WINAPI, LPCWSTR lpOutputString)
{
	LOGW(debug, FUEL, "{}", lpOutputString);
}