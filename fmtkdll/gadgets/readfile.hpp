//$ symbol kernel32::ReadFile, "BOOL (WINAPI *)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)";
//$ tag kernel32::ReadFile, hook;
//$ address kernel32::ReadFile, [*], !kernel32::ReadFile;

BOOL WINAPI wrap_kernel32_ReadFile(
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
{
	static CHAR pszFilename[MAX_PATH + 1];

	//if (GetFinalPathNameByHandleA(hFile, pszFilename, sizeof(pszFilename), FILE_NAME_NORMALIZED))
	//{
	//	std::filesystem::path path(pszFilename);

	//	// FMTK_BREAKPOINT_IF(path.extension() == ".DPC");
	//	FMTK_BREAKPOINT_IF(path.filename() == "SHARED.DPC");
	//	// FMTK_BREAKPOINT_IF(strcmp(pszFilename, "d:\\steamlibrary\\steamapps\\common\\fuel\\datas\\p_moto.dpc") == 0);
	//}
	
	return real_kernel32_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}
