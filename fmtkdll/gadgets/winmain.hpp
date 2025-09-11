//$ symbol fuel::WinMain, "INT (WINAPI *)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)";
//$ tag fuel::WinMain, hook;
//$ address fuel::WinMain, [retail], @0x0081e340;

INT WINAPI wrap_fuel_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                             LPSTR lpCmdLine, int nShowCmd) {
  // volatile bool x = true;
  // while (x);

  LOG(trace, FMTK, "Entry Point");

  int result = 0;

  backward::SignalHandling sh;

	BROADCAST(Initialize);

	result = real_fuel_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	LOG(trace, FMTK, "FUEL returned {}", result);

  return result;
}
