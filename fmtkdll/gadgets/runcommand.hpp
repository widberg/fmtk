FUNCTION(RunCommand, 0x0069a590, bool, __stdcall, const void* pState, const char* cmd, CommandSource commandSource)
{
	LOG(trace, FUEL, "Running command: {} | {}", cmd, commandSource);

	return Real_RunCommand(pState, cmd, commandSource);
}