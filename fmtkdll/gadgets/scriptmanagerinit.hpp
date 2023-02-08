//$ symbol fuel::ScriptManagerInit, "void (__fastcall *)(DWORD x, DWORD y, DWORD z)";
//$ tag fuel::ScriptManagerInit, hook;
//$ address fuel::ScriptManagerInit, [retail], @0x0081cdb0;

void __fastcall wrap_fuel_ScriptManagerInit(DWORD x, DWORD y, DWORD z)
{
	LOG(trace, FMTK, "{} {} {}", x, y, z);
	real_fuel_ScriptManagerInit(x, y, z);

	LOG(trace, FMTK, "ScriptManagerInit");
	
	LOG(trace, FMTK, "Setting our callback");

	// Bridge_RegisterCommand(*real_fuel_pGlobalCommandState, FMTKEmitEventCallback, "FMTKEmitEvent");


	BROADCAST(ScriptManagerInitialize);
}
