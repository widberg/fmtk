FUNCTION(OutputDebugStringA, OutputDebugStringA, void, WINAPI, LPCSTR lpOutputString)
{
	LOG(debug, FUEL, "{}", lpOutputString);
}