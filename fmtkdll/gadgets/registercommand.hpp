BF(void __usercall Real_RegisterCommand)(LPCSTR name AT edi, LPCVOID pThis, LPVOID callback)
	(0x0069a400);

FF(void __usercall FMTK_RegisterCommand)(LPCSTR name AT edi, LPCVOID pThis, LPVOID callback)
(
	LOG(trace, FMTK, "Registering command: {}", name);

	Real_RegisterCommand_trampoline(name, pThis, callback);

	RETURN;
)

void RegisterCommand(const std::string& name, bool(*callback)())
{
	Real_RegisterCommand_trampoline(name.c_str(), *pGlobalCommandState, (void*)callback);
}
