#include "sehexception.hpp"

#include <DbgHelp.h>
#include <sstream>
#include <iomanip>

SEHException::SEHException(const EXCEPTION_POINTERS* ep) noexcept
{
	const CONTEXT& context = *ep->ContextRecord;
	const EXCEPTION_RECORD& er = *ep->ExceptionRecord;

	std::stringstream ss;
	ss << "SEH Exception 0x";
	ss << std::hex << std::setw(8) << std::setfill('0') << er.ExceptionCode;
	ss << " at 0x";
	ss << std::hex << std::setw(8) << std::setfill('0') << er.ExceptionAddress;
	if (er.ExceptionCode == 0xc0000005)
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

const char* SEHException::what() const noexcept
{
	return msg.c_str();
};
