FUNCTION(RegisterCommand, 0x0069a400, void, __usercall, const void* pThis, void* callback)
{
	REG_TO_VAR(edi, LPCSTR, name);

	LOG(trace, FMTK, "Registering command: {}", name);

	Bridge_RegisterCommand(pThis, callback, name);
}

void Bridge_RegisterCommand(const void* pThis, void* callback, LPCSTR name)
{
	__asm
	{
		mov edi, name
		push callback
		push pThis
		call Real_RegisterCommand
	};
}