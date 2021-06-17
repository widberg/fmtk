FUNCTION(CoreMainLoop, 0x00688bf0, void, __cdecl, void)
{
	BROADCAST(Tick);

	Real_CoreMainLoop();
}