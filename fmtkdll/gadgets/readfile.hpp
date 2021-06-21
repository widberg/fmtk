FUNCTION(ReadFile, ReadFile, BOOL, WINAPI,
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
{
	static TCHAR pszFilename[MAX_PATH + 1];

	//if (GetFinalPathNameByHandleA(hFile, pszFilename, sizeof(pszFilename), FILE_NAME_NORMALIZED))
	//{
	//	std::filesystem::path path(pszFilename);

	//	FMTK_HIGHLIGHT(lpBuffer);
	//	//FMTK_BREAKPOINT_IF(path.extension() == ".DPC");
	//	FMTK_BREAKPOINT_IF(path.filename() == "BUGGY.DPC");
	//}
	
	return Real_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}
