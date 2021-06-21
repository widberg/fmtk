FUNCTION(ScriptManagerInit, 0x0081cdb0, void, __fastcall, DWORD x, DWORD y, DWORD z)
{
	LOG(trace, FMTK, "{} {} {}", x, y, z);
	Real_ScriptManagerInit(x, y, z);

	LOG(trace, FMTK, "ScriptManagerInit");
	
	LOG(trace, FMTK, "Setting our callback");

	// Bridge_RegisterCommand(*pGlobalCommandState, FMTKEmitEventCallback, "FMTKEmitEvent");


	BROADCAST(ScriptManagerInitialize);
}