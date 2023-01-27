FUNCTION(RunCommand, 0x0069a590, bool, __stdcall, LPCVOID pState, LPCSTR cmd, DWORD depth)
{
	LOG(trace, FUEL, "Running command: {} | {}", cmd, depth);

	return Real_RunCommand(pState, cmd, depth);
}

bool RunCommand(const char* cmd)
{
	return Real_RunCommand(*pGlobalCommandState, cmd, 0);
}
