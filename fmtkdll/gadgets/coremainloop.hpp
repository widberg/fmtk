BF(void __usercall Real_CoreMainLoop)()
	(0x00688bf0);

FF(void __usercall FMTK_CoreMainLoop)()
(
	BROADCAST(Tick);

	Real_CoreMainLoop_trampoline();

	RETURN;
)
