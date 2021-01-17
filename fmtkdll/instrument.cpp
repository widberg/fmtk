#include "instrument.hpp"

#include <Windows.h>
#include <d3dx9shader.h>
#include <detours.h>

#include <algorithm>
#include <filesystem>
#include <sol/sol.hpp>
#include <cwctype>
#include <csignal>

#include "fmtkdll.hpp"
#include "logging.hpp"
#include "scripting.hpp"

#define FUNCTION(name, address, returnType, callingConvention, ...) \
returnType (callingConvention * Real_##name)(__VA_ARGS__)           \
	= reinterpret_cast<decltype(Real_##name)>(address);             \
returnType callingConvention FMTK_##name(__VA_ARGS__)

#define NAKEDFUNCTION(name, address)                    \
void (* Real_##name)(void)                              \
	= reinterpret_cast<decltype(Real_##name)>(address); \
__declspec(naked) void FMTK_##name(void)

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

class SEHException : public std::exception
{
public:
	SEHException(unsigned int code) noexcept : er{ code, 0, nullptr, nullptr, 0 } { setupMsg(); }
	SEHException(const EXCEPTION_POINTERS* ep) noexcept : er(*ep->ExceptionRecord) { setupMsg(); }

	DWORD code() const { return er.ExceptionCode; }
	PVOID address() const { return er.ExceptionAddress; }

	const char* what() const noexcept { return msg.c_str(); };
private:
	void setupMsg()
	{
		std::stringstream ss;
		ss << "SEH Exception 0x";
		ss << std::hex << std::setw(8) << std::setfill('0') << code();
		ss << " at 0x";
		ss << std::hex << std::setw(8) << std::setfill('0') << address();
		if (code() == 0xc0000005)
		{
			ss << " this could indicate that xlive has detected a debugger attached to FUEL or you were playing around in the developer menus";
		}

		msg = ss.str();
	}

	EXCEPTION_RECORD er;
	std::string msg;
};

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
		LOGW(trace, CORE, "FUEL opening file: {} which resolved to {}", fileName, alias);
		fileName = alias;
	}
	else
	{
		LOGW(trace, CORE, "FUEL opening file: {}", fileName);
	}

	HANDLE rv = Real_CreateFileW(fileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	if (rv == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		LPSTR lpBuffer = nullptr;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpBuffer, 0, NULL);

		LOG(error, CORE, "Error {} : {}", dwError, lpBuffer);

		LocalFree(lpBuffer);
	}

	return rv;
}

FUNCTION(RegisterCommand, 0x0069a400, void, __stdcall, const void* pThis, void* callback)
{
	// uses a non standard calling convention, first 2 args on stack right to left, third arg in edi, a result of msvc avoiding trashing the stack
	LPCSTR name;
	__asm
	{
		mov name, edi
	};

	LOG(trace, CORE, "Registering command: {}", name);

	Bridge_RegisterCommand(pThis, callback, name);
}

void Bridge_RegisterCommand(const void* pThis, void* callback, LPCSTR name)
{
	__asm
	{
		mov edi, name
		push callback
		push pThis
		call Real_RegisterCommand
	};
}

FUNCTION(ScriptManagerInit, 0x0081cdb0, void, __fastcall, DWORD x, DWORD y, DWORD z)
{
	LOG(trace, CORE, "{} {} {}", x, y, z);
	Real_ScriptManagerInit(x, y, z);

	LOG(trace, CORE, "ScriptManagerInit");
	LOG(trace, CORE, "Setting our callback");

	Bridge_RegisterCommand(*pGlobalCommandState, FMTKEmitEventCallback, "FMTKEmitEvent");
}

NAKEDFUNCTION(Load, 0x00689256)
{
	ScriptingEmitEvent(EventType::LOAD);

	__asm
	{
		jmp Real_Load
	}
}

FUNCTION(WinMain, 0x0081e340, INT, WINAPI, HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	LOG(trace, CORE, "Entry Point");

	AttachDetoursXLive();

	ScriptingEmitEvent(EventType::ENTRY);

	int result = 0;

	_set_se_translator([](unsigned int code, EXCEPTION_POINTERS* pEP)
	{
		LOG(critical, CORE, "Translating SEH Exception");
		throw SEHException(pEP);
	});

	try
	{
		result = Real_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
	}
	catch (const std::exception& e)
	{
		LOG(critical, CORE, "{}", e.what());
	}
	catch (...)
	{
		LOG(critical, CORE, "Unhandled exception");
	}

	return result;
}

FUNCTION(CoreMainLoop, 0x00688bf0, void, __cdecl, void)
{
	ScriptingEmitEvent(EventType::TICK);

	Real_CoreMainLoop();
}

FUNCTION(OutputDebugStringA, OutputDebugStringA, void, WINAPI, LPCSTR lpOutputString)
{
	LOG(debug, FUEL, "{}", lpOutputString);
}

FUNCTION(OutputDebugStringW, OutputDebugStringW, void, WINAPI, LPCWSTR lpOutputString)
{
	LOGW(debug, FUEL, "{}", lpOutputString);
}

FUNCTION(RunCommand, 0x0069a590, bool, __stdcall, const void* pState, const char* cmd, CommandSource commandSource)
{
	LOG(trace, FUEL, "Running command: {} | {}", cmd, commandSource);

	return Real_RunCommand(pState, cmd, commandSource);
}

FUNCTION(D3DXCompileShaderFromFileA, 0x008b11e6, HRESULT, WINAPI,
	LPCSTR                          pSrcFile,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs,
	LPD3DXCONSTANTTABLE*            ppConstantTable)
{
	LOG(trace, CORE, "Compiling shader: {}", pSrcFile);

	*ppErrorMsgs = nullptr;
	LPD3DXBUFFER errorBuffer;

	HRESULT result = Real_D3DXCompileShaderFromFileA(pSrcFile, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, &errorBuffer, ppConstantTable);

	if (errorBuffer != nullptr)
	{
		LOG(error, CORE, "{}", (char*)errorBuffer->GetBufferPointer());
	}

	return result;
}

FUNCTION(TerminateProcess, TerminateProcess, BOOL, WINAPI,
	HANDLE hProcess,
	UINT   uExitCode)
{
	LOG(trace, CORE, "Pray to god you see this {}", uExitCode);

	return TerminateProcess(hProcess, uExitCode);
}

FUNCTION(LoadLibraryA, LoadLibraryA, HMODULE, WINAPI,
	LPCSTR lpLibFileName)
{
	LOG(trace, CORE, "ALoading dll: {}", lpLibFileName);
	
	return Real_LoadLibraryA(lpLibFileName);
}

FUNCTION(LoadLibraryExA, LoadLibraryExA, HMODULE, WINAPI,
	LPCSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags)
{
	LOG(trace, CORE, "ExALoading dll: {} 0x{0:x}", lpLibFileName, dwFlags);

	return Real_LoadLibraryExA(lpLibFileName, hFile, dwFlags);
}

FUNCTION(LoadLibraryW, LoadLibraryW, HMODULE, WINAPI,
	LPCWSTR lpLibFileName)
{
	LOGW(trace, CORE, "WLoading dll: {}", lpLibFileName);

	return Real_LoadLibraryW(lpLibFileName);
}

FUNCTION(LoadLibraryExW, LoadLibraryExW, HMODULE, WINAPI,
	LPCWSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags)
{
	LOGW(trace, CORE, "ExWLoading dll: {} 0x{0:x}", lpLibFileName, dwFlags);

	return Real_LoadLibraryExW(lpLibFileName, hFile, dwFlags);
}

FUNCTIONXLIVE(ValidateMemory, 0x004f36b3, INT, WINAPI, DWORD, DWORD, DWORD)
{
	//LOG(trace, CORE, "ValidateMemory");

	return 0;
}

FUNCTION(CreateWindowExW, CreateWindowExW, HWND, WINAPI,
	DWORD     dwExStyle,
	LPCWSTR   lpClassName,
	LPCWSTR   lpWindowName,
	DWORD     dwStyle,
	int       X,
	int       Y,
	int       nWidth,
	int       nHeight,
	HWND      hWndParent,
	HMENU     hMenu,
	HINSTANCE hInstance,
	LPVOID    lpParam)
{
	LOG(trace, CORE, "Creating window");

	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	return Real_CreateWindowExW(dwExStyle, lpClassName, lpWindowName, WS_POPUP, 0, 0, w, h, hWndParent, hMenu, hInstance, lpParam);
}

NAKEDFUNCTION(IsCarInGame, 0x0060f2a9)
{
	__asm
	{
		jmp Real_IsCarInGame
	}
}

FUNCTION(Death, 0x0052f370, void, __cdecl, void)
{
	void* p;

	__asm
	{
		mov p, esi
	}

	LOG(trace, CORE, "DEATH");

	__asm
	{
		mov esi, p
		call Real_Death
	}
}

bool AttachDetoursXLive()
{
	HINSTANCE hiXLive = GetModuleHandleA("xlive.dll");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	ATTACHXLIVE(ValidateMemory);

	ULONG result = DetourTransactionCommit();
	
	return result;
}

bool DetachDetoursXLive()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DETACHXLIVE(ValidateMemory);

	return DetourTransactionCommit();
}

// Instrument
LONG AttachDetours()
{
    LOG(trace, CORE, "Attaching detours");

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

	ATTACH(CreateFileW);
	ATTACH(WinMain);
	//ATTACH(IsDebuggerPresent);
	ATTACH(CoreMainLoop);
	//ATTACH(OutputDebugStringA);
	//ATTACH(OutputDebugStringW);
	ATTACH(RunCommand);
	ATTACH(D3DXCompileShaderFromFileA);
	//ATTACH(LoadLibraryA);
	//ATTACH(LoadLibraryW);
	//ATTACH(LoadLibraryExA);
	//ATTACH(LoadLibraryExW);
	//ATTACH(TerminateProcess);
	//ATTACH(CreateWindowExW);
	ATTACH(RegisterCommand);
	ATTACH(ScriptManagerInit);
	ATTACH(Load);
	//ATTACH(IsCarInGame);
	ATTACH(Death);

    LOG(trace, CORE, "Ready to commit");

    return DetourTransactionCommit();
}

LONG DetachDetours()
{
    LOG(trace, CORE, "Detaching detours");

	DetachDetoursXLive();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

	DETACH(CreateFileW);
	DETACH(WinMain);
	//DETACH(IsDebuggerPresent);
	DETACH(CoreMainLoop);
	//DETACH(OutputDebugStringA);
	//DETACH(OutputDebugStringW);
	DETACH(RunCommand);
	DETACH(D3DXCompileShaderFromFileA);
	//DETACH(LoadLibraryA);
	//DETACH(LoadLibraryW);
	//DETACH(LoadLibraryExA);
	//DETACH(LoadLibraryExW);
	//DETACH(TerminateProcess);
	//DETACH(CreateWindowExW);
	DETACH(RegisterCommand);
	DETACH(ScriptManagerInit);
	DETACH(Load);
	//DETACH(IsCarInGame);
	DETACH(Death);

    LOG(trace, CORE, "Ready to commit");

    return DetourTransactionCommit();
}
