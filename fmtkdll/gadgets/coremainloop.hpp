//$ symbol fuel::CoreMainLoop, "void (__stdcall *)()";
//$ tag fuel::CoreMainLoop, hook;
//$ address fuel::CoreMainLoop, [retail], @0x00688bf0;
FUNCTION(CoreMainLoop, 0x00688bf0, void, __stdcall)
{
	BROADCAST(Tick);

	Real_CoreMainLoop();
}
