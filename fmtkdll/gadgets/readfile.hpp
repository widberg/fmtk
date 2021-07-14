FUNCTION(ReadFile, ReadFile, BOOL, WINAPI,
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
{
	static CHAR pszFilename[MAX_PATH + 1];

	if (GetFinalPathNameByHandleA(hFile, pszFilename, sizeof(pszFilename), FILE_NAME_NORMALIZED))
	{
		std::filesystem::path path(pszFilename);

		// FMTK_BREAKPOINT_IF(path.extension() == ".DPC");
		FMTK_BREAKPOINT_IF(path.filename() == "P_MOTO.DPC" || path.filename() == "p_moto.dpc");
		// FMTK_BREAKPOINT_IF(strcmp(pszFilename, "d:\\steamlibrary\\steamapps\\common\\fuel\\datas\\p_moto.dpc") == 0);
	}
	
	return Real_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}
