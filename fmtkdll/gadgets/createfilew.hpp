FUNCTION(CreateFileW, CreateFileW, HANDLE, WINAPI,
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

	HANDLE rv = Real_CreateFileW(fileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	if (rv == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		LPSTR lpBuffer = nullptr;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpBuffer, 0, NULL);

		LOG(error, FMTK, "Error {} : {}", dwError, lpBuffer);

		LocalFree(lpBuffer);
	}

	return rv;
}