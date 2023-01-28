//$ symbol fuel::WinMain, "INT (WINAPI *)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)";
//$ tag fuel::WinMain, hook;
//$ address fuel::WinMain, [retail], @0x0081e340;

FUNCTION(WinMain, 0x0081e340, INT, WINAPI, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// while (1);

	LOG(trace, FMTK, "Entry Point");

	AttachDetoursXLive();

	int result = 0;

	_set_se_translator([](unsigned int code, EXCEPTION_POINTERS* pEP)
		{
			LOG(critical, FMTK, "Translating SEH Exception");
			throw SEHException(pEP);
		});

	try
	{
		BROADCAST(Initialize);

		result = Real_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
		LOG(trace, FMTK, "FUEL returned {}", result);
	}
	catch (const std::exception& e)
	{
		LOG(critical, FMTK, "{}", e.what());
	}
	catch (...)
	{
		LOG(critical, FMTK, "Unhandled exception");
	}

	return result;
}
