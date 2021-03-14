#include "instrument.hpp"

#include <Windows.h>
#include <d3dx9shader.h>
#include <detours.h>
#include <intrin.h>

#include <algorithm>
#include <filesystem>
#include <sol/sol.hpp>
#include <cwctype>
#include <csignal>

#include "fmtkdll.hpp"
#include "logging.hpp"
#include "scripting.hpp"

#define __usercall __stdcall

#define FUNCTION(name, address, returnType, callingConvention, ...) \
returnType (callingConvention * Real_##name)(__VA_ARGS__)           \
	= reinterpret_cast<decltype(Real_##name)>(address);             \
returnType callingConvention FMTK_##name(__VA_ARGS__)

#define NAKEDFUNCTION(name, address)                    \
void (* Real_##name)(void)                              \
	= reinterpret_cast<decltype(Real_##name)>(address); \
__declspec(naked) void FMTK_##name(void)

#define NAKEDRETURN(name) \
__asm                     \
{                         \
	jmp Real_##name       \
}

#define ATTACH(x)       DetourAttach(&(PVOID&)Real_##x, FMTK_##x)
#define DETACH(x)       DetourDetach(&(PVOID&)Real_##x, FMTK_##x)

#define FUNCTIONXLIVE(name, address, returnType, callingConvention, ...) \
returnType (callingConvention * Real_XLive_##name)(__VA_ARGS__)          \
	= reinterpret_cast<decltype(Real_XLive_##name)>(address);            \
returnType callingConvention FMTK_XLive_##name(__VA_ARGS__)

#define XLIVE_DLL_BASE_ADDRESS (0x400000)
#define ATTACHXLIVE(x)  Real_XLive_##x = reinterpret_cast<decltype(Real_XLive_##x)>((DWORD_PTR)hiXLive +  (DWORD_PTR)Real_XLive_##x - (DWORD_PTR)XLIVE_DLL_BASE_ADDRESS); DetourAttach(&(PVOID&)Real_XLive_##x, FMTK_XLive_##x)
#define DETACHXLIVE(x)  DetourDetach(&(PVOID&)Real_XLive_##x, FMTK_XLive_##x)

#define BACKUP_REGISTER(reg) \
DWORD backup_##reg;          \
__asm                        \
{                            \
	mov backup_##reg, reg    \
}

#define BACKUP_REGISTERS() \
BACKUP_REGISTER(eax);      \
BACKUP_REGISTER(ecx);      \
BACKUP_REGISTER(edx);      \
BACKUP_REGISTER(ebx);      \
BACKUP_REGISTER(esp);      \
BACKUP_REGISTER(ebp);      \
BACKUP_REGISTER(esi);      \
BACKUP_REGISTER(edi);

#define RESTORE_REGISTER(reg) \
__asm                         \
{                             \
	mov reg, backup_##reg     \
}

#define RESTORE_REGISTERS() \
RESTORE_REGISTER(eax);      \
RESTORE_REGISTER(ecx);      \
RESTORE_REGISTER(edx);      \
RESTORE_REGISTER(ebx);      \
RESTORE_REGISTER(esp);      \
RESTORE_REGISTER(ebp);      \
RESTORE_REGISTER(esi);      \
RESTORE_REGISTER(edi);

#define REG_TO_VAR(reg, type, var) \
type var;                          \
__asm                              \
{                                  \
	mov var, reg                   \
};

#define VAR_TO_REG(var, reg) \
__asm                        \
{                            \
	mov reg, var             \
};

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

FUNCTION(ReadFile, ReadFile, BOOL, WINAPI,
	HANDLE       hFile,
	LPVOID       lpBuffer,
	DWORD        nNumberOfBytesToRead,
	LPDWORD      lpNumberOfBytesRead,
	LPOVERLAPPED lpOverlapped)
{
	//static TCHAR pszFilename[MAX_PATH + 1];

	//if (GetFinalPathNameByHandleA(hFile, pszFilename, sizeof(pszFilename), FILE_NAME_NORMALIZED))
	//{
	//	std::filesystem::path path(pszFilename);

	//	FMTK_HIGHLIGHT(lpBuffer);
	//	FMTK_BREAKPOINT_IF(path.extension() == ".DPC");
	//}
	
	return Real_ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

FUNCTION(RegisterCommand, 0x0069a400, void, __usercall, const void* pThis, void* callback)
{
	REG_TO_VAR(edi, LPCSTR, name);

	LOG(trace, FMTK, "Registering command: {}", name);

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

NAKEDFUNCTION(CRC32, 0x00669160)
{
	__asm {
		push ebp
		mov ebp, esp
		sub esp, __LOCAL_SIZE
	}

	BACKUP_REGISTER(eax);
	BACKUP_REGISTER(edx);

	unsigned int initial;
	char* crc32str;
	__asm {
		mov initial, eax
		mov crc32str, edx
	}
	LOG(trace, FMTK, "crc32: {} \"{}\"", initial, crc32str);

	RESTORE_REGISTER(eax);
	RESTORE_REGISTER(edx);
	__asm {
		mov esp, ebp
		pop ebp
	}

	NAKEDRETURN(CRC32);
}

FUNCTION(ScriptManagerInit, 0x0081cdb0, void, __fastcall, DWORD x, DWORD y, DWORD z)
{
	LOG(trace, FMTK, "{} {} {}", x, y, z);
	Real_ScriptManagerInit(x, y, z);

	LOG(trace, FMTK, "ScriptManagerInit");
	LOG(trace, FMTK, "Setting our callback");

	Bridge_RegisterCommand(*pGlobalCommandState, FMTKEmitEventCallback, "FMTKEmitEvent");

	ScriptingEmitEvent(EventType::COMMAND_INIT);
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
	LOG(trace, FMTK, "Entry Point");

	AttachDetoursXLive();

	ScriptingEmitEvent(EventType::ENTRY);

	int result = 0;

	_set_se_translator([](unsigned int code, EXCEPTION_POINTERS* pEP)
	{
		LOG(critical, FMTK, "Translating SEH Exception");
		throw SEHException(pEP);
	});

	try
	{
		result = Real_WinMain(hInstance, hPrevInstance, lpCmdLine, nShowCmd);
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
	LOG(trace, FMTK, "Compiling shader: {}", pSrcFile);

	*ppErrorMsgs = nullptr;
	LPD3DXBUFFER errorBuffer;

	HRESULT result = Real_D3DXCompileShaderFromFileA(pSrcFile, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, &errorBuffer, ppConstantTable);

	if (errorBuffer != nullptr)
	{
		LOG(error, FMTK, "{}", (char*)errorBuffer->GetBufferPointer());
	}

	return result;
}

FUNCTIONXLIVE(ValidateMemory, 0x004f36b3, INT, WINAPI, DWORD, DWORD, DWORD)
{
	return 0;
}

FUNCTIONXLIVE(CreateFileW, 0x0040128c, HANDLE, WINAPI,
	LPCWSTR               lpFileName,
	DWORD                 dwDesiredAccess,
	DWORD                 dwShareMode,
	LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	DWORD                 dwCreationDisposition,
	DWORD                 dwFlagsAndAttributes,
	HANDLE                hTemplateFile)
{
	//LOGW(trace, XLIVE, "XLive opening file: {}", lpFileName);

	return Real_XLive_CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	//return rv;
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
	LOG(trace, FMTK, "Creating window");

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

FUNCTION(Death, 0x0052f370, void, __usercall, void* x)
{
	BACKUP_REGISTER(ecx);
	BACKUP_REGISTER(esi);

	LOG(trace, FMTK, "DEATH");

	RESTORE_REGISTER(ecx);
	RESTORE_REGISTER(esi);
	__asm
	{
		push x
		call Real_Death
	}
}

bool AttachDetoursXLive()
{
	HINSTANCE hiXLive = GetModuleHandleA("xlive.dll");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//ATTACHXLIVE(ValidateMemory);
	//ATTACHXLIVE(CreateFileW);

	ULONG result = DetourTransactionCommit();
	
	return result;
}

bool DetachDetoursXLive()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//DETACHXLIVE(ValidateMemory);
	//DETACHXLIVE(CreateFileW);

	return DetourTransactionCommit();
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
	//ATTACH(OutputDebugStringA);
	//ATTACH(OutputDebugStringW);
	ATTACH(RunCommand);
	ATTACH(D3DXCompileShaderFromFileA);
	//ATTACH(CreateWindowExW);
	ATTACH(RegisterCommand);
	ATTACH(ScriptManagerInit);
	ATTACH(Load);
	//ATTACH(IsCarInGame);
	ATTACH(Death);
	ATTACH(ReadFile);
	//ATTACH(CRC32);

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
	//DETACH(OutputDebugStringA);
	//DETACH(OutputDebugStringW);
	DETACH(RunCommand);
	DETACH(D3DXCompileShaderFromFileA);
	//DETACH(CreateWindowExW);
	DETACH(RegisterCommand);
	DETACH(ScriptManagerInit);
	DETACH(Load);
	//DETACH(IsCarInGame);
	DETACH(Death);
	DETACH(ReadFile);
	//DETACH(CRC32);

    LOG(trace, FMTK, "Ready to commit");

    return DetourTransactionCommit();
}
