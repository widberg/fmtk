#include "instrument.hpp"

#include <Windows.h>
#include <winternl.h>
#include <DbgHelp.h>
#include <d3dx9shader.h>
#include <detours.h>
#include <intrin.h>

#include <algorithm>
#include <filesystem>
#include <cwctype>
#include <csignal>
#include <sstream>

#include "fmtkdll.hpp"
#include "logging.hpp"

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
	SEHException(const EXCEPTION_POINTERS* ep) noexcept : context(*ep->ContextRecord), er(*ep->ExceptionRecord) { setupMsg(); }

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
		ss << "\n";

		BOOL    result;
		HANDLE  process;
		HANDLE  thread;
		HMODULE hModule;
		HMODULE hLastModule = NULL;

		STACKFRAME64        stack;
		ULONG               frame;
		DWORD64             displacement;
		char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
		pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		pSymbol->MaxNameLen = MAX_SYM_NAME;

		DWORD disp;

		char module[MAX_PATH + 1];

		memset(&stack, 0, sizeof(STACKFRAME64));

		process = GetCurrentProcess();
		thread = GetCurrentThread();
		displacement = 0;
#if !defined(_M_AMD64)
		stack.AddrPC.Offset = context.Eip;
		stack.AddrPC.Mode = AddrModeFlat;
		stack.AddrStack.Offset = context.Esp;
		stack.AddrStack.Mode = AddrModeFlat;
		stack.AddrFrame.Offset = context.Ebp;
		stack.AddrFrame.Mode = AddrModeFlat;
#endif

		SymInitialize(process, NULL, TRUE); //load symbols

		for (frame = 0; ; frame++)
		{
			//get next call from stack
			result = StackWalk64
			(
#if defined(_M_AMD64)
				IMAGE_FILE_MACHINE_AMD64
#else
				IMAGE_FILE_MACHINE_I386
#endif
				,
				process,
				thread,
				&stack,
				(PVOID)&context,
				NULL,
				SymFunctionTableAccess64,
				SymGetModuleBase64,
				NULL
			);

			if (!result)
			{
				break;
			}

			hModule = NULL;
			GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
				(LPCTSTR)(stack.AddrPC.Offset), &hModule);
			if (hModule != NULL)
			{
				if (hModule != hLastModule)
				{
					GetModuleFileNameA(hModule, module, MAX_PATH + 1);
					ss << "in " << module << "\n";
					hLastModule = hModule;
				}
			}
			else
			{
				ss << "in unknown module" << "\n";
				hLastModule = NULL;
			}

			ss << "\tat ";
			

			if (SymFromAddr(process, (ULONG64)stack.AddrPC.Offset, &displacement, pSymbol))
			{
				ss << pSymbol->Name;

				IMAGEHLP_LINE64 line;
				line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

				if (SymGetLineFromAddr64(process, stack.AddrPC.Offset, &disp, &line))
				{
					ss << " in " << line.FileName << ":" << std::dec << line.LineNumber;
				}

				ss << ", ";
			}
			
			ss << "address 0x" << std::hex << std::setw(8) << std::setfill('0') << (UINT32)stack.AddrPC.Offset << "\n";
		}

		msg = ss.str();
	}

	CONTEXT context;
	EXCEPTION_RECORD er;
	std::string msg;
};

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

bool AttachDetoursXLive()
{
	HINSTANCE hiXLive = GetModuleHandleA("xlive.dll");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//ATTACHXLIVE(ValidateMemory);

	ULONG result = DetourTransactionCommit();
	
	return result;
}

bool DetachDetoursXLive()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	//DETACHXLIVE(ValidateMemory);

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
	ATTACH(ReadFile);
	//ATTACH(SetUnhandledExceptionFilter);
	//ATTACH(UnhandledExceptionFilter);
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
	DETACH(ReadFile);
	//DETACH(SetUnhandledExceptionFilter);
	//DETACH(UnhandledExceptionFilter);
	//DETACH(CRC32);

    LOG(trace, FMTK, "Ready to commit");

    return DetourTransactionCommit();
}
