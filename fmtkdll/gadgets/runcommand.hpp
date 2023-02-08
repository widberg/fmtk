//$ symbol fuel::RunCommand, "bool (__stdcall *)(LPCVOID pState, LPCSTR cmd, DWORD depth)";
//$ tag fuel::RunCommand, hook;
//$ address fuel::RunCommand, [retail], @0x0069a590;

bool __stdcall wrap_fuel_RunCommand(LPCVOID pState, LPCSTR cmd, DWORD depth)
{
	LOG(trace, FUEL, "Running command: {} | {}", cmd, depth);

	return real_fuel_RunCommand(pState, cmd, depth);
}

bool RunCommand(const char* cmd)
{
	return real_fuel_RunCommand(*real_fuel_pGlobalCommandState, cmd, 0);
}
