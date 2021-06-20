BF(bool __usercall Real_RunCommand AT al)(LPCVOID pState, LPCSTR cmd, DWORD depth)
	(0x0069a590);

FF(bool __usercall FMTK_RunCommand AT al)(LPCVOID pState, LPCSTR cmd, DWORD depth)
(
	LOG(trace, FUEL, "Running command: {} | {}", cmd, depth);

	RETURN(Real_RunCommand_trampoline(pState, cmd, depth));
)

bool RunCommand(const std::string& cmd)
{
	return Real_RunCommand_trampoline(*pGlobalCommandState, cmd.c_str(), 0);
}
