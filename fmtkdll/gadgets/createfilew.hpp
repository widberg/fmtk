//$ symbol kernel32::CreateFileW, "HANDLE (WINAPI *)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)";
//$ tag kernel32::CreateFileW, hook;
//$ address kernel32::CreateFileW, [*], !kernel32::CreateFileW;
HANDLE WINAPI wrap_kernel32_CreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	std::wstring fileName;

	try
	{
		fileName = std::filesystem::absolute(lpFileName);
	}
	catch (const std::filesystem::filesystem_error&)
	{
		fileName = lpFileName;
	}

	std::transform(fileName.begin(), fileName.end(), fileName.begin(), std::towlower);

	// FMTK_BREAKPOINT_IF(fileName == L"d:\\steamlibrary\\steamapps\\common\\fuel\\datas\\p_moto.dpc");

	if (aliases.count(fileName))
	{
		std::wstring alias = aliases.at(fileName);
		LOGW(trace, FMTK, "FUEL opening file: {} which resolved to {}", fileName, alias);
		fileName = alias;
	}
	else
	{
		LOGW(trace, FMTK, "FUEL opening file: {}", fileName);
	}

	HANDLE rv = real_kernel32_CreateFileW(fileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	if (rv == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		LPSTR lpBuffer = nullptr;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpBuffer, 0, NULL);

		LOG(trace, FMTK, "{} : {}", dwError, lpBuffer);

		LocalFree(lpBuffer);
	}

	return rv;
}
