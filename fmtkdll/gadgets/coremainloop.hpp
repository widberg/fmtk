FUNCTION(CoreMainLoop, 0x00688bf0, void, __stdcall)
{
	BROADCAST(Tick);

	Real_CoreMainLoop();
}
