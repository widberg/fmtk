#include "instrument.hpp"

#include <Windows.h>
#include <detours.h>

#include <dsound.h>

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

FUNCTIONXLIVE(CreateFileW, CreateFileW, HANDLE, WINAPI,
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

	LOGW(trace, FMTK, "XLive opening file: {}", fileName);

	HANDLE rv = Real_XLive_CreateFileW(fileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

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

const void** pGlobalCommandState = reinterpret_cast<const void**>(0x00a7c080);

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
#include "gadgets/readfile.hpp"
#include "gadgets/idirectsoundbufferplay.hpp"

bool patchXLive = false;

bool AttachDetoursXLive()
{
	ULONG result = 1;

	HINSTANCE hiXLive = GetModuleHandleA("xlive.dll");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//ATTACHXLIVE(ValidateMemory);
	//ATTACHXLIVE(CreateFileW);

	result = DetourTransactionCommit();
	
	return result;
}

bool DetachDetoursXLive()
{
	LONG result = 1;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//DETACHXLIVE(ValidateMemory);
	//DETACHXLIVE(CreateFileW);

	result = DetourTransactionCommit();

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

	ATTACH(IDirectSoundBuffer_Play);

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
