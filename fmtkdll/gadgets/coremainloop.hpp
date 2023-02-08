//$ symbol fuel::CoreMainLoop, "void (__stdcall *)()";
//$ tag fuel::CoreMainLoop, hook;
//$ address fuel::CoreMainLoop, [retail], @0x00688bf0;
void __stdcall wrap_fuel_CoreMainLoop()
{
	BROADCAST(Tick);

	real_fuel_CoreMainLoop();
}
