#include "instrument.hpp"

#include <Windows.h>
#include <detours.h>

#include <filesystem>

#include <usercall_hpp/usercall.hpp>

#include "fmtkdll.hpp"
#include "logging.hpp"
#include "sehexception.hpp"

#define FUNCTION(name, address, returnType, callingConvention, ...) \
returnType (callingConvention * Real_##name)(__VA_ARGS__)           \
	= reinterpret_cast<decltype(Real_##name)>(address);             \
returnType callingConvention FMTK_##name(__VA_ARGS__)

#define ATTACH(x)       DetourAttach(&(PVOID&)Real_##x, FMTK_##x)
#define DETACH(x)       DetourDetach(&(PVOID&)Real_##x, FMTK_##x)

#define FUNCTIONXLIVE(name, address, returnType, callingConvention, ...) \
returnType (callingConvention * Real_XLive_##name)(__VA_ARGS__)          \
	= reinterpret_cast<decltype(Real_XLive_##name)>(address);            \
returnType callingConvention FMTK_XLive_##name(__VA_ARGS__)

#define XLIVE_DLL_BASE_ADDRESS (0x400000)
#define ATTACHXLIVE(x)  Real_XLive_##x = reinterpret_cast<decltype(Real_XLive_##x)>((DWORD_PTR)hiXLive +  (DWORD_PTR)Real_XLive_##x - (DWORD_PTR)XLIVE_DLL_BASE_ADDRESS); DetourAttach(&(PVOID&)Real_XLive_##x, FMTK_XLive_##x)
#define DETACHXLIVE(x)  DetourDetach(&(PVOID&)Real_XLive_##x, FMTK_XLive_##x)

bool AttachDetoursXLive();
bool DetachDetoursXLive();

const void** pGlobalCommandState = reinterpret_cast<const void**>(0x00a7c080);

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

using p_float_t = float*;

#include "gadgets/createfilew.hpp"
#include "gadgets/registercommand.hpp"
#include "gadgets/scriptmanagerinit.hpp"
#include "gadgets/winmain.hpp"
#include "gadgets/coremainloop.hpp"
#include "gadgets/outputdebugstringa.hpp"
#include "gadgets/outputdebugstringw.hpp"
#include "gadgets/runcommand.hpp"
#include "gadgets/d3dxcompileshaderfromfilea.hpp"
#include "gadgets/validatememory.hpp"
#include "gadgets/createwindowexw.hpp"
#include "gadgets/getplayerposition.hpp"

bool patchXLive = false;

bool AttachDetoursXLive()
{
	ULONG result = 1;

	HINSTANCE hiXLive = GetModuleHandleA("xlive.dll");

    BYTE signature_ValidateMemory[] = {
		0x8b, 0xff, 0x55, 0x8b, 0xec, 0x83, 0xec, 0x20, 0x53, 0x56,
		0x57, 0x8d, 0x45, 0xe0, 0x33, 0xf6, 0x50, 0xff, 0x75, 0x0c,
		0x8b, 0xf9, 0x8b, 0x4d, 0x08, 0x89, 0x75, 0xe0, 0x89, 0x75,
		0xe4, 0x89, 0x75, 0xf8, 0x89, 0x75, 0xf0, 0xe8, 0x3f, 0xf4,
		0xff, 0xff, 0x8b, 0xd8, 0x3b, 0xde, 0x0f, 0x8c, 0x5c, 0x01,
		0x00, 0x00, 0xff, 0x75, 0x0c, 0x8b, 0x4d, 0x08, 0xe8, 0x04,
		0xf3, 0xff, 0xff, 0x8b
	};

	if (memcmp(signature_ValidateMemory, (const void*)((DWORD_PTR)hiXLive + (DWORD_PTR)0x004f36b3 - (DWORD_PTR)XLIVE_DLL_BASE_ADDRESS), sizeof(signature_ValidateMemory)) == 0)
	{
		patchXLive = true;

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		ATTACHXLIVE(ValidateMemory);

		result = DetourTransactionCommit();
	}
	
	return result;
}

bool DetachDetoursXLive()
{
	LONG result = 1;

	if (patchXLive)
	{
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());

		DETACHXLIVE(ValidateMemory);

		result = DetourTransactionCommit();
	}

	return result;
}

// Instrument
LONG AttachDetours()
{
    LOG(trace, FMTK, "Attaching detours");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

	ATTACH(CreateFileW);
	ATTACH(WinMain);
	ATTACH(CoreMainLoop);
	ATTACH(OutputDebugStringA);
	ATTACH(OutputDebugStringW);
	ATTACH(RunCommand);
	ATTACH(D3DXCompileShaderFromFileA);
	//ATTACH(CreateWindowExW);
	ATTACH(RegisterCommand);
	ATTACH(ScriptManagerInit);
	ATTACH(ReadFile);

    LOG(trace, FMTK, "Ready to commit");

    return DetourTransactionCommit();
}

LONG DetachDetours()
{
    LOG(trace, FMTK, "Detaching detours");

	DetachDetoursXLive();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

	DETACH(CreateFileW);
	DETACH(WinMain);
	DETACH(CoreMainLoop);
	DETACH(OutputDebugStringA);
	DETACH(OutputDebugStringW);
	DETACH(RunCommand);
	DETACH(D3DXCompileShaderFromFileA);
	//DETACH(CreateWindowExW);
	DETACH(RegisterCommand);
	DETACH(ScriptManagerInit);
	DETACH(ReadFile);

    LOG(trace, FMTK, "Ready to commit");

    return DetourTransactionCommit();
}
