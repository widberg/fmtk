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

#define FUNCTION(address, returnType, callingConvention, name, ...) \
returnType (callingConvention * Real_##name)(__VA_ARGS__)           \
	= reinterpret_cast<decltype(Real_##name)>(address);             \
returnType callingConvention FMTK_##name(__VA_ARGS__)

#define ATTACH(x)       DetourAttach(&(PVOID&)Real_##x, FMTK_##x)
#define DETACH(x)       DetourDetach(&(PVOID&)Real_##x, FMTK_##x)

#define XLIVE_DLL_BASE_ADDRESS (0x400000)
#define ATTACHXLIVE(x)  Real_XLive_##x = reinterpret_cast<decltype(Real_XLive_##x)>((DWORD_PTR)hiXLive +  (DWORD_PTR)Real_XLive_##x - (DWORD_PTR)XLIVE_DLL_BASE_ADDRESS); DetourAttach(&(PVOID&)Real_XLive_##x, FMTK_##x)
#define DETACHXLIVE(x)  DetourDetach(&(PVOID&)Real_XLive_##x, FMTK_##x)

bool AttachDetoursXLive();
bool DetachDetoursXLive();

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

// CreateFileW
HANDLE (WINAPI * Real_CreateFileW)(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
        = CreateFileW;

HANDLE WINAPI FMTK_CreateFileW(
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile
)
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

// RegisterCommand (uses a non standard calling convention, first 2 args on stack right to left, third arg in edi, a result of msvc avoiding mangling the stack)
void(* Real_RegisterCommand)(void* pThis, void* callback, LPCSTR name)
= reinterpret_cast<void(*)(void*, void*, LPCSTR)>(0x0069a400);

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

void __stdcall FMTK_RegisterCommand(const void* pThis, void* callback)
{
	LPCSTR name;
	__asm
	{
		mov name, edi
	};

	LOG(trace, CORE, "Registering command: {}", name);

	Bridge_RegisterCommand(pThis, callback, name);
}

bool FMTK_CommandCallback()
{
	DWORD argc = *(DWORD*)((char*)*pGlobalCommandState + 0x23ac);
	LOG(trace, CORE, "oh god oh fuck | {}", argc);

	const char** pArg0 = (const char**)((char*)*pGlobalCommandState + 0xa3b0);
	for (int i = 0; i < argc; ++i)
	{

		LOG(trace, CORE, "arg{} = {}", i, *(pArg0 + i));
	}

	return 1;
}

const void** pGlobalCommandState = reinterpret_cast<const void**>(0x00a7c080);


FUNCTION(0x0081cdb0, void, __fastcall, ScriptManagerInit, DWORD x, DWORD y, DWORD z)
{
	LOG(trace, CORE, "{} {} {}", x, y, z);
	Real_ScriptManagerInit(x, y, z);

	LOG(trace, CORE, "ScriptManagerInit");
	LOG(trace, CORE, "Setting our callback");

	Bridge_RegisterCommand(*pGlobalCommandState, FMTK_CommandCallback, "FMTK");
	Bridge_RegisterCommand(*pGlobalCommandState, FMTKEmitEventCallback, "FMTKEmitEvent");
}

// Load
void (*Real_Load)()
	= reinterpret_cast<void(*)()>(0x00689256);

__declspec(naked) void FMTK_Load()
{
	ScriptingEvent(EventType::LOAD);

	__asm
	{
		jmp Real_Load
	}
}

// WinMain
int (WINAPI* Real_WinMain)(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
	= reinterpret_cast<int (WINAPI*)(HINSTANCE, HINSTANCE, LPSTR, int)>(0x0081e340);

int WINAPI FMTK_WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	LOG(trace, CORE, "Entry Point");

	AttachDetoursXLive();

	ScriptingEvent(EventType::ENTRY);

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

// CoreMainLoop
void (__cdecl * Real_CoreMainLoop)(void)
	= reinterpret_cast<void (__cdecl*)()>(0x00688bf0);

void __cdecl FMTK_CoreMainLoop()
{
	ScriptingEvent(EventType::TICK);

	Real_CoreMainLoop();
}

// OutputDebugStringA

void (WINAPI * Real_OutputDebugStringA)(LPCSTR lpOutputString)
	= OutputDebugStringA;

void WINAPI FMTK_OutputDebugStringA(LPCSTR lpOutputString)
{
	LOG(debug, FUEL, "{}", lpOutputString);
}

// OutputDebugStringW

void (WINAPI * Real_OutputDebugStringW)(LPCWSTR lpOutputString)
	= OutputDebugStringW;

void WINAPI FMTK_OutputDebugStringW(LPCWSTR lpOutputString)
{
	LOGW(debug, FUEL, "{}", lpOutputString);
}

// RunCommand

bool (__stdcall* Real_RunCommand)(const void* pState, const char* cmd, CommandSource commandSource)
	= reinterpret_cast<bool(__stdcall*)(const void*, const char*, CommandSource)>(0x0069a590);

bool __stdcall FMTK_RunCommand(const void* pState, const char* cmd, CommandSource commandSource)
{
	LOG(trace, FUEL, "Running command: {} | {}", cmd, commandSource);

	return Real_RunCommand(pState, cmd, commandSource);
}

// IsDebuggerPresent
BOOL (WINAPI * Real_IsDebuggerPresent)()
	= IsDebuggerPresent;

BOOL(WINAPI* Real_XLive_IsDebuggerPresent)()
= reinterpret_cast<BOOL(WINAPI*)()>(0x00401368);

BOOL WINAPI FMTK_IsDebuggerPresent()
{
	LOG(trace, CORE, "xlive being snoopy");

	return FALSE;
}

// D3DXCompileShaderFromFileA
HRESULT(WINAPI *Real_D3DXCompileShaderFromFileA)(
	LPCSTR                          pSrcFile,
	CONST D3DXMACRO*                pDefines,
	LPD3DXINCLUDE                   pInclude,
	LPCSTR                          pFunctionName,
	LPCSTR                          pProfile,
	DWORD                           Flags,
	LPD3DXBUFFER*                   ppShader,
	LPD3DXBUFFER*                   ppErrorMsgs,
	LPD3DXCONSTANTTABLE*            ppConstantTable)
		=  reinterpret_cast<HRESULT(WINAPI *)(LPCTSTR, const D3DXMACRO*, LPD3DXINCLUDE, LPCSTR, LPCSTR, DWORD, LPD3DXBUFFER*, LPD3DXBUFFER*, LPD3DXCONSTANTTABLE*)>(0x008b11e6); // ensure we're hooking FUELs copy of d3dx9

HRESULT WINAPI FMTK_D3DXCompileShaderFromFileA(
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

// TerminateProcess
BOOL (WINAPI * Real_TerminateProcess)(
	HANDLE hProcess,
	UINT   uExitCode)
		= TerminateProcess;

BOOL WINAPI FMTK_TerminateProcess(
	HANDLE hProcess,
	UINT   uExitCode
)
{
	LOG(trace, CORE, "Pray to god you see this {}", uExitCode);

	return TerminateProcess(hProcess, uExitCode);
}

// LoadLibraryA
HMODULE (WINAPI * Real_LoadLibraryA)(
	LPCSTR lpLibFileName)
		= LoadLibraryA;

HMODULE (WINAPI * Real_XLive_LoadLibraryA)(
	LPCSTR lpLibFileName)
		= reinterpret_cast<HMODULE(WINAPI *)(LPCSTR)>(0x00401344);

HMODULE WINAPI FMTK_LoadLibraryA(
	LPCSTR lpLibFileName
)
{
	LOG(trace, CORE, "ALoading dll: {}", lpLibFileName);
	
	return Real_XLive_LoadLibraryA(lpLibFileName);
}

// LoadLibraryExA
HMODULE (WINAPI * Real_XLive_LoadLibraryExA)(
	LPCSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags)
		= LoadLibraryExA;

HMODULE (WINAPI * Real_LoadLibraryExA)(
	LPCSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags)
		= reinterpret_cast<HMODULE(WINAPI *)(LPCSTR lpLibFileName, HANDLE hFile, DWORD  dwFlags)>(0x00401364);

HMODULE WINAPI FMTK_LoadLibraryExA(
	LPCSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags
)
{
	LOG(trace, CORE, "ExALoading dll: {} 0x{0:x}", lpLibFileName, dwFlags);

	return Real_XLive_LoadLibraryExA(lpLibFileName, hFile, dwFlags);
}

// LoadLibraryW
HMODULE (WINAPI * Real_LoadLibraryW)(
	LPCWSTR lpLibFileName)
		= LoadLibraryW;

HMODULE(WINAPI* Real_XLive_LoadLibraryW)(
	LPCWSTR lpLibFileName)
	= reinterpret_cast<HMODULE(WINAPI*)(LPCWSTR)>(00401310);

HMODULE WINAPI FMTK_LoadLibraryW(
	LPCWSTR lpLibFileName
)
{
	LOGW(trace, CORE, "WLoading dll: {}", lpLibFileName);

	return Real_XLive_LoadLibraryW(lpLibFileName);
}

// LoadLibraryExW
HMODULE(WINAPI* Real_LoadLibraryExW)(
	LPCWSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags)
		= LoadLibraryExW;

HMODULE(WINAPI* Real_XLive_LoadLibraryExW)(
	LPCWSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags)
	= reinterpret_cast<HMODULE(WINAPI*)(LPCWSTR lpLibFileName, HANDLE hFile, DWORD  dwFlags)>(0x004012ec);

HMODULE WINAPI FMTK_LoadLibraryExW(
	LPCWSTR lpLibFileName,
	HANDLE hFile,
	DWORD  dwFlags
)
{
	LOGW(trace, CORE, "ExWLoading dll: {} 0x{0:x}", lpLibFileName, dwFlags);

	return Real_XLive_LoadLibraryExW(lpLibFileName, hFile, dwFlags);
}

// ValidateMemory
INT (__stdcall * Real_XLive_ValidateMemory)(DWORD, DWORD, DWORD)
	= reinterpret_cast<INT (__stdcall *)(DWORD, DWORD, DWORD)>(0x004f36b3);

INT __stdcall FMTK_ValidateMemory(DWORD, DWORD, DWORD)
{
	//LOG(trace, CORE, "ValidateMemory");

	return 0;
}

// DebuggerCheck
DWORD(__stdcall * Real_XLive_DebuggerCheck)(DWORD, DWORD, DWORD)
	= reinterpret_cast<DWORD(__stdcall *)(DWORD, DWORD, DWORD)>(0x00546c70);

DWORD __stdcall FMTK_DebuggerCheck(DWORD, DWORD, DWORD)
{
	return 0;
}

// DebuggerCheck
DWORD(__stdcall * Real_XLive_DebuggerCheck2)()
	= reinterpret_cast<DWORD(__stdcall *)()>(0x00546c70);

DWORD __stdcall FMTK_DebuggerCheck2()
{
	return 0;
}

// CreateWindowExW
HWND (WINAPI * Real_CreateWindowExW)(
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
		= CreateWindowExW;

HWND WINAPI FMTK_CreateWindowExW(
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
	LPVOID    lpParam
)
{
	LOG(trace, CORE, "Creating window");

	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	return Real_CreateWindowExW(dwExStyle, lpClassName, lpWindowName, WS_POPUP, 0, 0, w, h, hWndParent, hMenu, hInstance, lpParam);
}

// IsCarInGame
void(* Real_IsCarInGame)(void)
	= reinterpret_cast<void(*)(void)>(0x0060f2a9);

__declspec(naked) void FMTK_IsCarInGame(void)
{
	__asm
	{
		jmp Real_IsCarInGame
	}
}

// Death
void(* Real_Death)(void* p)
	= reinterpret_cast<void(*)(void*)>(0x0052f370);

void __fastcall FMTK_Death()
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

	//const char* wtf = "\x56\xC7\x46\x38\xCC\xCC\xCC\xCC\x90\x90";
	//memcpy_s(reinterpret_cast<void*>((DWORD_PTR)hiXLive + (DWORD_PTR)0x005458d4 - (DWORD_PTR)XLIVE_DLL_BASE_ADDRESS), 10, wtf, 10);

	//ATTACHXLIVE(IsDebuggerPresent);
	ATTACHXLIVE(ValidateMemory);
	//ATTACHXLIVE(DebuggerCheck);
	//ATTACHXLIVE(DebuggerCheck2);

	ULONG result = DetourTransactionCommit();
	
	return result;
}

bool DetachDetoursXLive()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//DETACHXLIVE(IsDebuggerPresent);
	DETACHXLIVE(ValidateMemory);
	//DETACHXLIVE(DebuggerCheck);
	//DETACHXLIVE(DebuggerCheck2);

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
	//ATTACH(Death);

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
	//DETACH(Death);

    LOG(trace, CORE, "Ready to commit");

    return DetourTransactionCommit();
}
