//Project: ErrLib
//Author: MSDN.WhiteKnight (https://github.com/MSDN-WhiteKnight)

#define ERRLIB_EXPORTS
#include "StackWalker.h"

#include <locale>
#include <codecvt>
#include <string>
 // event.mc 
 // This is the header section.
 // The following are the categories of events.
//
//  Values are 32 bit values laid out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
//
//  where
//
//      Sev - is the severity code
//
//          00 - Success
//          01 - Informational
//          10 - Warning
//          11 - Error
//
//      C - is the Customer code flag
//
//      R - is a reserved bit
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_SYSTEM                  0x0
#define FACILITY_STUBS                   0x3
#define FACILITY_RUNTIME                 0x2
#define FACILITY_IO_ERROR_CODE           0x4


//
// Define the severity codes
//
#define STATUS_SEVERITY_WARNING          0x2
#define STATUS_SEVERITY_SUCCESS          0x0
#define STATUS_SEVERITY_INFORMATIONAL    0x1
#define STATUS_SEVERITY_ERROR            0x3


//
// MessageId: ERRLIB_CATEGORY
//
// MessageText:
//
// Application Events
//
#define ERRLIB_CATEGORY                  ((WORD)0x00000001L)




// *** internal functions *** 

LONG WINAPI ErrLib_MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo);
LONG WINAPI ErrLib_DefaultExceptionCallback(struct _EXCEPTION_POINTERS* ex, LPCWSTR mes, LPCWSTR stack);

// *** variables ***

CRITICAL_SECTION ErrLib_DbgHlpSync = { 0 }; //DbgHlp functions syncronization object
CRITICAL_SECTION ErrLib_LogSync = { 0 }; //ErrLib logging functions syncronization object
ERRLIB_EXCEPTION_CALLBACK volatile ErrLib_pCurrentExceptionCallback = NULL; //Pointer to a user function called on unhandled exception
LPTOP_LEVEL_EXCEPTION_FILTER volatile ErrLib_pPreviousExceptionFilter = NULL; //Unhandled exception filter which was set before ErrLib
WCHAR ErrLib_LogFilePath[MAX_PATH] = { 0 }; //A path to the current error log file
BOOL volatile ErrLib_fOutputLogFile = TRUE; //Configuration flag: Output errors to log file
BOOL volatile ErrLib_fOutputStderr = TRUE; //Configuration flag: Output errors to stderr stream
BOOL volatile ErrLib_fOutputMbox = FALSE; //Configuration flag: Output errors as message box
BOOL volatile ErrLib_fOutputEventLog = FALSE; //Configuration flag: Output errors into Windows Event Log

/*DWORD ErrLib_LastExceptionCode = 0;
WCHAR ErrLib_LastExceptionMessage[ErrLib_MessageLen]={0};
WCHAR ErrLib_LastExceptionStack[ErrLib_StackLen]={0};
WCHAR ErrLib_StrBuf[ErrLib_MessageLen]={0};
ULONG_PTR ErrLib_ExArgs[2]={0};*/

DWORD volatile ErrLib_tlsiLastExceptionCode = 0;
DWORD volatile ErrLib_tlsiLastExceptionMessage = 0;
DWORD volatile ErrLib_tlsiLastExceptionStack = 0;
DWORD volatile ErrLib_tlsiStrBuf = 0;
DWORD volatile ErrLib_tlsiExArgs = 0;

#ifdef __cplusplus
extern "C" {
#endif

	/* Pointer getters*/

	ERRLIB_API LPVOID __stdcall ErrLib_StrBuf_GetPointer() {
		return TlsGetValue(ErrLib_tlsiStrBuf);
		//return ErrLib_StrBuf;
	}

	ERRLIB_API LPVOID __stdcall ErrLib_ExArgs_GetPointer() {
		return TlsGetValue(ErrLib_tlsiExArgs);
		//return ErrLib_ExArgs;
	}

	LPVOID ErrLib_LastExceptionCode_GetPointer() {
		return TlsGetValue(ErrLib_tlsiLastExceptionCode);
		//return &ErrLib_LastExceptionCode;
	}

	//Get error message for current exception in ERRLIB_CATCH/ERRLIB_CATCH_ALL block
	ERRLIB_API LPWSTR __stdcall ErrLib_Except_GetMessage() {
		return (LPWSTR)TlsGetValue(ErrLib_tlsiLastExceptionMessage);
		//return ErrLib_LastExceptionMessage;
	}

	//Get stack trace for current exception in ERRLIB_CATCH/ERRLIB_CATCH_ALL block
	ERRLIB_API LPWSTR __stdcall ErrLib_Except_GetStackTrace() {
		return (LPWSTR)TlsGetValue(ErrLib_tlsiLastExceptionStack);
		//return ErrLib_LastExceptionStack;
	}

	//Gets filename from full path
	ERRLIB_API WCHAR* __stdcall ErrLib_FileNameFromPathW(WCHAR* path) {

		int i; WCHAR* p;

		size_t len = wcslen(path);
		if (len <= 3)return path;

		i = len - 2;
		p = path;

		while (TRUE) {
			if (path[i] == '\\' || path[i] == '/') { p = &(path[i + 1]); break; }
			i--;
			if (i < 0)break;
		}
		return p;
	}

	// *** Functions to print error messages *** 

	//Gets Win32 error message for specified function name and error code
	ERRLIB_API void __stdcall ErrLib_ErrorMes(LPTSTR lpszFunction, DWORD dw, WCHAR* buf)
	{
		// Retrieve the system error message for the last-error code
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		// Print the error message 
		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
		swprintf(buf, L"%s failed with error %u. %s",
			lpszFunction, dw, lpMsgBuf);

		LocalFree(lpMsgBuf);
		LocalFree(lpDisplayBuf);
	}

	ERRLIB_API void __stdcall ErrLib_PrintError(LPTSTR lpszFunction, DWORD dw) {
		WCHAR buf[1024] = { 0 };
		ErrLib_ErrorMes(lpszFunction, dw, buf);
		wprintf(L"%s\n", buf);
	}

#ifdef __cplusplus
	ERRLIB_API void __stdcall ErrLib_HResultToString(HRESULT hr, LPTSTR lpszFunction, WCHAR* buf)
	{
		_com_error err(hr);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wide = converter.from_bytes(err.ErrorMessage());
		const WCHAR* errMsg = wide.c_str();

		// Print the error message      
		swprintf(buf, L"%s failed with HRESULT 0x%x. %s",
			lpszFunction, (DWORD)hr, errMsg);
	}
#endif

	//Sets current exception callback. Specify NULL to call default callback
	ERRLIB_API void __stdcall ErrLib_SetExceptionCallback(ERRLIB_EXCEPTION_CALLBACK pCallback) {
		ErrLib_pCurrentExceptionCallback = pCallback;
	}

	//Sets current log file path. The default is [MyDocuments]\[ExeFileName].log
	ERRLIB_API void __stdcall ErrLib_SetLogFilePath(LPCWSTR path) {
		if (path == NULL) return;
		StringCchCopyW(ErrLib_LogFilePath, MAX_PATH, path);
	}

	//Sets value for the specified configuration parameter
	ERRLIB_API BOOL __stdcall ErrLib_SetParameter(UINT param, UINT_PTR value) {
		switch (param)
		{
		case ERRLIB_OUTPUT_LOGFILE: ErrLib_fOutputLogFile = (BOOL)value; return TRUE;
		case ERRLIB_OUTPUT_STDERR: ErrLib_fOutputStderr = (BOOL)value; return TRUE;
		case ERRLIB_OUTPUT_MBOX: ErrLib_fOutputMbox = (BOOL)value; return TRUE;
		case ERRLIB_OUTPUT_EVENT_LOG: ErrLib_fOutputEventLog = (BOOL)value; return TRUE;

		default:return FALSE;
		}
	}

	ERRLIB_API BOOL __stdcall ErrLib_InitThread() {

		LPVOID lpvData = NULL;
		BOOL res;

		lpvData = LocalAlloc(LPTR, sizeof(ULONG_PTR) * 2);
		res = TlsSetValue(ErrLib_tlsiExArgs, lpvData);
		if (res == FALSE) fwprintf(stderr, L"TlsSetValue failed\n");

		lpvData = LocalAlloc(LPTR, sizeof(DWORD));
		res = TlsSetValue(ErrLib_tlsiLastExceptionCode, lpvData);
		if (res == FALSE) fwprintf(stderr, L"TlsSetValue failed\n");

		lpvData = LocalAlloc(LPTR, sizeof(WCHAR) * ErrLib_MessageLen);
		res = TlsSetValue(ErrLib_tlsiLastExceptionMessage, lpvData);
		if (res == FALSE) fwprintf(stderr, L"TlsSetValue failed\n");

		lpvData = LocalAlloc(LPTR, sizeof(WCHAR) * ErrLib_StackLen);
		res = TlsSetValue(ErrLib_tlsiLastExceptionStack, lpvData);
		if (res == FALSE) fwprintf(stderr, L"TlsSetValue failed\n");

		lpvData = LocalAlloc(LPTR, sizeof(WCHAR) * ErrLib_MessageLen);
		res = TlsSetValue(ErrLib_tlsiStrBuf, lpvData);
		if (res == FALSE) fwprintf(stderr, L"TlsSetValue failed\n");

		return res;
	}

	ERRLIB_API void __stdcall ErrLib_FreeThread() {

		LPVOID lpvData = NULL;

		lpvData = TlsGetValue(ErrLib_tlsiExArgs);
		LocalFree(lpvData);

		lpvData = TlsGetValue(ErrLib_tlsiLastExceptionCode);
		LocalFree(lpvData);

		lpvData = TlsGetValue(ErrLib_tlsiLastExceptionMessage);
		LocalFree(lpvData);

		lpvData = TlsGetValue(ErrLib_tlsiLastExceptionStack);
		LocalFree(lpvData);

		lpvData = TlsGetValue(ErrLib_tlsiStrBuf);
		LocalFree(lpvData);
	}

	ERRLIB_API BOOL __stdcall ErrLib_InitTLS() {

		BOOL retval = TRUE;

		ErrLib_tlsiExArgs = TlsAlloc();
		if (ErrLib_tlsiExArgs == TLS_OUT_OF_INDEXES) {
			fwprintf(stderr, L"TlsAlloc failed\n");
			retval = FALSE;
		}

		ErrLib_tlsiLastExceptionCode = TlsAlloc();
		if (ErrLib_tlsiLastExceptionCode == TLS_OUT_OF_INDEXES) {
			fwprintf(stderr, L"TlsAlloc failed\n");
			retval = FALSE;
		}

		ErrLib_tlsiLastExceptionMessage = TlsAlloc();
		if (ErrLib_tlsiLastExceptionMessage == TLS_OUT_OF_INDEXES) {
			fwprintf(stderr, L"TlsAlloc failed\n");
			retval = FALSE;
		}

		ErrLib_tlsiLastExceptionStack = TlsAlloc();
		if (ErrLib_tlsiLastExceptionStack == TLS_OUT_OF_INDEXES) {
			fwprintf(stderr, L"TlsAlloc failed\n");
			retval = FALSE;
		}

		ErrLib_tlsiStrBuf = TlsAlloc();
		if (ErrLib_tlsiStrBuf == TLS_OUT_OF_INDEXES) {
			fwprintf(stderr, L"TlsAlloc failed\n");
			retval = FALSE;
		}

		return retval;
	}

	//Initializes the library. Must be called before any other functionality is used.
	ERRLIB_API BOOL __stdcall ErrLib_Initialize() {
		BOOL res = FALSE;
		HRESULT hr = S_OK;
		PWSTR documents_path = NULL;
		WCHAR szDirectory[MAX_PATH] = L"";
		WCHAR szFileName[MAX_PATH] = L"";
		WCHAR logpath[MAX_PATH] = L"";
		LPCWSTR ShortName = NULL;
		LPVOID lpvData = NULL;

		//set global unhandled exception filter which will be called without debugger's precense
		ErrLib_pPreviousExceptionFilter = SetUnhandledExceptionFilter(&ErrLib_MyUnhandledExceptionFilter);

		//initialize thread safety objects		
		InitializeCriticalSection(&ErrLib_DbgHlpSync);
		InitializeCriticalSection(&ErrLib_LogSync);

		//initialize debugging symbols
		res = SymInitialize(GetCurrentProcess(), NULL, TRUE); //load symbols
		if (res == FALSE) fwprintf(stderr, L"SymInitialize failed with error 0x%x\n", GetLastError());

		//configure default log file path
		hr = SHGetKnownFolderPath(ERRLIB_REFERENCE FOLDERID_Documents, 0, (HANDLE)NULL, &documents_path); //get My Documents path	
		if (SUCCEEDED(hr)) {
			StringCchCopyW(szDirectory, MAX_PATH, documents_path);
			StringCchCatW(szDirectory, MAX_PATH, L"\\");
			CoTaskMemFree(documents_path);
		}

		ShortName = szFileName;
		GetModuleFileNameW(NULL, szFileName, MAX_PATH);

		ShortName = ErrLib_FileNameFromPathW(szFileName); //get current EXE file short name
		StringCchPrintfW(logpath, MAX_PATH, L"%s%s.log", szDirectory, ShortName);
		ErrLib_SetLogFilePath(logpath);

		return res;
	}


	//Creates registry data for Windows Event Log. Requires elevated priveleges. 
	ERRLIB_API BOOL __stdcall ErrLib_RegisterEventSource() {
		WCHAR szFileName[ErrLib_MessageLen] = L"";
		LPCWSTR ShortName = NULL;
		WCHAR buf[ErrLib_MessageLen] = L"";
		LONG lRes;
		DWORD dwValue;
		HKEY hKey;
		HMODULE hModule = NULL;
		BOOL retval = TRUE;

		//get current EXE file short name
		if (0 == GetModuleFileNameW(NULL, szFileName, ErrLib_MessageLen)) {
			fwprintf(stderr, L"ErrLib_RegisterEventSource: GetModuleFileName failed with code %u\n", GetLastError());
			return FALSE;
		}
		ShortName = ErrLib_FileNameFromPathW(szFileName);

		//set registry values
		StringCchPrintfW(buf, ErrLib_MessageLen, L"SYSTEM\\CurrentControlSet\\services\\eventlog\\Application\\%s", ShortName);
		lRes = RegCreateKeyW(HKEY_LOCAL_MACHINE, buf, &hKey);
		if (lRes != ERROR_SUCCESS) {
			return FALSE;
		}

		dwValue = 1;
		lRes = RegSetValueExW(hKey, L"CategoryCount", 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD));
		if (lRes != ERROR_SUCCESS) {
			retval = FALSE;
		}

		dwValue = 3;
		lRes = RegSetValueExW(hKey, L"TypesSupported", 0, REG_DWORD, (BYTE*)&dwValue, sizeof(DWORD));
		if (lRes != ERROR_SUCCESS) {
			retval = FALSE;
		}

		StringCchCopyW(buf, ErrLib_MessageLen, L"");
		if (FALSE == GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)ErrLib_RegisterEventSource, &hModule)) {
			retval = FALSE;
		}

		if (hModule != NULL) GetModuleFileNameW(hModule, buf, ErrLib_MessageLen);

		lRes = RegSetValueExW(hKey, L"CategoryMessageFile", 0, REG_SZ, (BYTE*)buf, wcslen(buf) * 2 + 1);
		if (lRes != ERROR_SUCCESS) {
			retval = FALSE;
		}

		lRes = RegSetValueExW(hKey, L"EventMessageFile", 0, REG_SZ, (BYTE*)buf, wcslen(buf) * 2 + 1);
		if (lRes != ERROR_SUCCESS) {
			retval = FALSE;
		}

		lRes = RegSetValueExW(hKey, L"ParameterMessageFile", 0, REG_SZ, (BYTE*)buf, wcslen(buf) * 2 + 1);
		if (lRes != ERROR_SUCCESS) {
			retval = FALSE;
		}

		RegCloseKey(hKey);
		return retval;
	}

	//Deletes Windows Event Log registry data. Requires elevated priveleges. 
	ERRLIB_API BOOL __stdcall ErrLib_UnregisterEventSource() {
		WCHAR szFileName[ErrLib_MessageLen] = L"";
		LPCWSTR ShortName = NULL;
		WCHAR buf[ErrLib_MessageLen] = L"";
		LONG lRes;

		if (0 == GetModuleFileNameW(NULL, szFileName, ErrLib_MessageLen)) {
			fwprintf(stderr, L"ErrLib_UnregisterEventSource: GetModuleFileName failed with code %u\n", GetLastError());
			return FALSE;
		}
		ShortName = ErrLib_FileNameFromPathW(szFileName);

		if (wcslen(ShortName) == 0) return FALSE;

		StringCchPrintfW(buf, ErrLib_MessageLen, L"SYSTEM\\CurrentControlSet\\services\\eventlog\\Application\\%s", ShortName);
		lRes = RegDeleteKeyW(HKEY_LOCAL_MACHINE, buf);

		if (lRes != ERROR_SUCCESS) {
			return FALSE;
		}

		return TRUE;
	}

	//Prints stack trace based on context record
	ERRLIB_API void __stdcall ErrLib_PrintStack(CONTEXT* ctx, WCHAR* dest, size_t cch)
	{
		BOOL    result = FALSE;
		HANDLE  process = NULL;
		HANDLE  thread = NULL;
		HMODULE hModule = NULL;

		STACKFRAME64        stack;
		ULONG               frame = 0;
		DWORD64             displacement = 0;

		DWORD disp = 0;
		IMAGEHLP_LINEW64* line = NULL;

		char buffer[sizeof(SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(WCHAR)] = { 0 };
		WCHAR name[ErrLib_MaxNameLen] = { 0 };
		WCHAR module[ErrLib_MaxNameLen] = { 0 };
		WCHAR* module_short = NULL;
		WCHAR strbuf[MAX_SYM_NAME * 3] = { 0 };

		PSYMBOL_INFOW pSymbol = (PSYMBOL_INFOW)buffer;

		StringCchCopyW(dest, cch, L"");
		memset(&stack, 0, sizeof(STACKFRAME64));

		process = GetCurrentProcess();
		thread = GetCurrentThread();
		displacement = 0;
#if !defined(_M_AMD64)
		stack.AddrPC.Offset = (*ctx).Eip;
		stack.AddrPC.Mode = AddrModeFlat;
		stack.AddrStack.Offset = (*ctx).Esp;
		stack.AddrStack.Mode = AddrModeFlat;
		stack.AddrFrame.Offset = (*ctx).Ebp;
		stack.AddrFrame.Mode = AddrModeFlat;
#endif

		EnterCriticalSection(&ErrLib_DbgHlpSync);
		__try {

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
					ctx,
					NULL,
					SymFunctionTableAccess64,
					SymGetModuleBase64,
					NULL
				);

				if (!result) break;

				//get symbol name for address
				ZeroMemory(buffer, sizeof(buffer));
				pSymbol->SizeOfStruct = sizeof(SYMBOL_INFOW);
				pSymbol->MaxNameLen = MAX_SYM_NAME;
				result = SymFromAddrW(process, (ULONG64)stack.AddrPC.Offset, &displacement, pSymbol);
				if (result == FALSE) { //name not available, output address instead
					StringCchPrintfW(pSymbol->Name, MAX_SYM_NAME, L"0x%llx", (DWORD64)stack.AddrPC.Offset);
				}

				line = (IMAGEHLP_LINEW64*)malloc(sizeof(IMAGEHLP_LINEW64));
				ZeroMemory(line, sizeof(IMAGEHLP_LINEW64));
				line->SizeOfStruct = sizeof(IMAGEHLP_LINEW64);

				//get module name
				hModule = NULL;
				wcscpy(module, L"");
				module_short = &(module[0]);

				GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
					(LPCTSTR)(stack.AddrPC.Offset), &hModule);

				if (hModule != NULL) {
					GetModuleFileNameW(hModule, module, ErrLib_MaxNameLen);
					module_short = ErrLib_FileNameFromPathW(module);
				}

				//try to get line
				if (SymGetLineFromAddrW64(process, stack.AddrPC.Offset, &disp, line))
				{
					StringCchPrintfW(strbuf, sizeof(strbuf) / sizeof(WCHAR),
						L"  in %s!%s + 0x%02llx (%s; line: %lu;)\n", module_short, pSymbol->Name,
						displacement,
						line->FileName, line->LineNumber);
					StringCchCatW(dest, cch, strbuf);
				}
				else
				{
					//failed to get line, output address instead
					StringCchPrintfW(strbuf, sizeof(strbuf) / sizeof(WCHAR),
						L"  in %s!%s (%s; address: 0x%llx)\n", module_short, pSymbol->Name, module,
						(DWORD64)(stack.AddrPC.Offset - pSymbol->ModBase));
					StringCchCatW(dest, cch, strbuf);
				}

				free(line);
				line = NULL;
				if (frame > 9999)break;
			}//end for

		}
		__finally {
			LeaveCriticalSection(&ErrLib_DbgHlpSync);
		}
	}

	//Prints error message for specified exception
	ERRLIB_API void __stdcall ErrLib_GetExceptionMessage(struct _EXCEPTION_POINTERS* ExceptionInfo, LPWSTR dest, size_t cch) {
		WCHAR buf[1024] = { 0 };

		// https://msdn.microsoft.com/en-us/library/cc704588.aspx?f=255&MSPPError=-2147217396
		//https://msdn.microsoft.com/ru-ru/library/windows/desktop/aa363082%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396

		switch (ExceptionInfo->ExceptionRecord->ExceptionCode)
		{
		case STATUS_CONTROL_C_EXIT: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"STATUS_CONTROL_C_EXIT"); break;

		case STATUS_ACCESS_VIOLATION:
			switch (ExceptionInfo->ExceptionRecord->ExceptionInformation[0])
			{
			case 0:
				StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
					L"The instruction referenced memory at 0x%08llx. The memory could not be read",
					(DWORD64)ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
				break;
			case 1:
				StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
					L"The instruction referenced memory at 0x%08llx. The memory could not be written",
					(DWORD64)ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
				break;
			default:
				StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
					L"The instruction referenced memory address for which it does not have the appropriate access"
				);
				break;
			}
			break;

		case STATUS_IN_PAGE_ERROR:
			switch (ExceptionInfo->ExceptionRecord->ExceptionInformation[0])
			{
			case 0:
				StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
					L"The instruction tried to read memory at 0x%08llx, but the system was unable to load page because of error 0x%08llx",
					(DWORD64)ExceptionInfo->ExceptionRecord->ExceptionInformation[1], (DWORD64)ExceptionInfo->ExceptionRecord->ExceptionInformation[2]);
				break;
			case 1:
				StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
					L"The instruction tried to write memory at 0x%08llx, but the system was unable to load page because of error 0x%08llx",
					(DWORD64)ExceptionInfo->ExceptionRecord->ExceptionInformation[1], (DWORD64)ExceptionInfo->ExceptionRecord->ExceptionInformation[2]);
				break;
			default:
				StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
					L"The instruction tried to access a page that was not present, and the system was unable to load the page"
				);
				break;
			}
			break;

		case 0xe06d7363: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"C++ exception was not handled in user code"); break;
		case STATUS_ARRAY_BOUNDS_EXCEEDED: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Array bounds exceeded"); break;
		case STATUS_NO_MEMORY:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
				L"Not enough virtual memory or paging file quota is available to complete the specified operation");
			break;
		case STATUS_STACK_OVERFLOW:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"The thread used up all of its available stack space");
			break;
		case STATUS_STACK_BUFFER_OVERRUN:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"The system detected an overrun of a stack-based buffer in this application");
			break;
		case STATUS_ILLEGAL_INSTRUCTION:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"An attempt was made to execute an illegal instruction");
			break;
		case STATUS_INVALID_CRUNTIME_PARAMETER:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"An invalid parameter was passed to a C runtime function.");
			break;

		case STATUS_FLOAT_DENORMAL_OPERAND: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Floating-point denormal operand"); break;
		case STATUS_FLOAT_DIVIDE_BY_ZERO: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Floating-point division by zero"); break;
		case STATUS_FLOAT_INEXACT_RESULT: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Floating-point inexact result"); break;
		case STATUS_FLOAT_INVALID_OPERATION: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Floating-point invalid operation"); break;
		case STATUS_FLOAT_OVERFLOW: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Floating-point overflow"); break;
		case STATUS_FLOAT_STACK_CHECK: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L" Floating-point stack check"); break;
		case STATUS_FLOAT_UNDERFLOW: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Floating-point underflow"); break;
		case STATUS_FLOAT_MULTIPLE_FAULTS: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Multiple floating-point faults"); break;
		case STATUS_FLOAT_MULTIPLE_TRAPS: StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Multiple floating-point traps"); break;

		case STATUS_INTEGER_DIVIDE_BY_ZERO:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Integer division by zero"); break;
		case STATUS_INTEGER_OVERFLOW:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR), L"Integer operation resulted in overflow"); break;

		case ERRLIB_WIN32_EXCEPTION:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
				L"%s", (LPCWSTR)ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
			break;

		case ERRLIB_APP_EXCEPTION:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
				L"%s", (LPCWSTR)ExceptionInfo->ExceptionRecord->ExceptionInformation[0]);
			break;

#ifdef __cplusplus
		case ERRLIB_COM_EXCEPTION:
			StringCchPrintfW(buf, sizeof(buf) / sizeof(WCHAR),
				L"%s", (LPCWSTR)ExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
			break;
#endif
		}

		StringCchPrintfW(dest, cch, L"%s", buf);
	}

	//Outputs exception information into configured log targets
	ERRLIB_API void __stdcall ErrLib_LogExceptionInfo(DWORD dwExcCode, LPCWSTR lpwsMessage, LPCWSTR lpwsStackTrace, BOOL visible) {

		SYSTEMTIME st = { 0 };
		FILE* fp = NULL;
		WCHAR buf[ErrLib_StackLen] = { 0 };
		WCHAR szFileName[MAX_PATH] = L"";
		LPCWSTR ShortName = NULL;
		HANDLE hEventLog = NULL;
		LPWSTR pInsertStrings[1] = { NULL };

		EnterCriticalSection(&ErrLib_LogSync);

		__try {

			//put information into the log file
			if (ErrLib_fOutputLogFile != FALSE) {
				fp = _wfopen(ErrLib_LogFilePath, L"at+, ccs=UTF-8");
				if (fp != NULL) {

					GetLocalTime(&st);
					fwprintf(fp, L"%4d.%02d.%02d %2d:%02d - Exception 0x%x: ",
						(int)st.wYear, (int)st.wMonth, (int)st.wDay, (int)st.wHour, (int)st.wMinute, dwExcCode);
					fputws(lpwsMessage, fp);
					fputwc(L'\n', fp);

					fputws(lpwsStackTrace, fp);
					fputwc(L'\n', fp);
					fclose(fp);
				}
			}

			//put information into the Windows Event Log
			if (ErrLib_fOutputEventLog != FALSE) {
				GetModuleFileNameW(NULL, szFileName, MAX_PATH);
				ShortName = ErrLib_FileNameFromPathW(szFileName);

				hEventLog = RegisterEventSourceW(NULL, ShortName);
				if (NULL == hEventLog)
				{
					fwprintf(stderr, L"RegisterEventSource failed with 0x%x.\n", GetLastError());
				}
				else {
					// This event uses insert strings.
					StringCchPrintfW(buf, ErrLib_StackLen, L"Exception 0x%x: %s\n%s", dwExcCode, lpwsMessage, lpwsStackTrace);
					pInsertStrings[0] = buf;
					if (!ReportEventW(hEventLog, EVENTLOG_ERROR_TYPE, ERRLIB_CATEGORY, MSG_ERROR, NULL, 1, 0, (LPCWSTR*)pInsertStrings, NULL))
					{
						fwprintf(stderr, L"ReportEvent failed with 0x%x.\n", GetLastError());
					}
				}
			}

			//put information into stderr stream
			if (visible != FALSE && ErrLib_fOutputStderr != FALSE) {
				fwprintf(stderr, L"Exception 0x%x: ", dwExcCode);
				fputws(lpwsMessage, stderr);
				fputwc(L'\n', stderr);
				fputws(lpwsStackTrace, stderr);
			}

			//display information in message box
			if (visible != FALSE && ErrLib_fOutputMbox != FALSE) {
				StringCchPrintfW(buf, ErrLib_StackLen, L"Exception 0x%x: %s", dwExcCode, lpwsMessage);
				MessageBoxW(NULL, buf, NULL, MB_SYSTEMMODAL | MB_OK | MB_ICONERROR);
			}


		}
		__finally {
			LeaveCriticalSection(&ErrLib_LogSync);
		}
	}

	//Outputs arbitrary string into configured log targets (type can be: MSG_ERROR, MSG_WARNING or MSG_INFORMATION)
	ERRLIB_API void __stdcall ErrLib_LogMessage(LPCWSTR lpwsMessage, BOOL visible, DWORD type, BOOL bIncludeStack) {

		SYSTEMTIME st = { 0 };
		FILE* fp = NULL;
		WCHAR lpwsStackTrace[ErrLib_StackLen] = { 0 };
		WCHAR buf[ErrLib_StackLen] = { 0 };
		WCHAR szFileName[MAX_PATH] = L"";
		LPCWSTR ShortName = NULL;
		LPCWSTR caption = NULL;
		HANDLE hEventLog = NULL;
		LPWSTR pInsertStrings[1] = { NULL };
		CONTEXT ctx = { 0 };
		DWORD eventlog_type;
		DWORD mbox_type;

		if (bIncludeStack != FALSE) {
			RtlCaptureContext(&ctx);
			ErrLib_PrintStack(&ctx, lpwsStackTrace, ErrLib_StackLen);
		}

		EnterCriticalSection(&ErrLib_LogSync);

		__try {

			//put information into the log file
			if (ErrLib_fOutputLogFile != FALSE) {
				fp = _wfopen(ErrLib_LogFilePath, L"at+, ccs=UTF-8");
				if (fp != NULL) {

					GetLocalTime(&st);
					fwprintf(fp, L"%4d.%02d.%02d %2d:%02d: ",
						(int)st.wYear, (int)st.wMonth, (int)st.wDay, (int)st.wHour, (int)st.wMinute);
					fputws(lpwsMessage, fp);
					fputwc(L'\n', fp);

					fputws(lpwsStackTrace, fp);
					fputwc(L'\n', fp);
					fclose(fp);
				}
			}

			//put information into the event log
			if (ErrLib_fOutputEventLog != FALSE) {

				switch (type) {
				case MSG_ERROR: eventlog_type = EVENTLOG_ERROR_TYPE; break;
				case MSG_WARNING: eventlog_type = EVENTLOG_WARNING_TYPE; break;
				case MSG_INFORMATION: eventlog_type = EVENTLOG_INFORMATION_TYPE; break;
				default: eventlog_type = EVENTLOG_INFORMATION_TYPE; break;
				}

				GetModuleFileNameW(NULL, szFileName, MAX_PATH);
				ShortName = ErrLib_FileNameFromPathW(szFileName);

				hEventLog = RegisterEventSourceW(NULL, ShortName);
				if (NULL == hEventLog)
				{
					fwprintf(stderr, L"RegisterEventSource failed with 0x%x.\n", GetLastError());
				}
				else {
					// This event uses insert strings.
					StringCchPrintfW(buf, ErrLib_StackLen, L"%s\n%s", lpwsMessage, lpwsStackTrace);
					pInsertStrings[0] = buf;
					if (!ReportEventW(hEventLog, eventlog_type, ERRLIB_CATEGORY, type, NULL, 1, 0, (LPCWSTR*)pInsertStrings, NULL))
					{
						fwprintf(stderr, L"ReportEvent failed with 0x%x.\n", GetLastError());
					}
				}
			}

			//put information into stderr stream
			if (visible != FALSE && ErrLib_fOutputStderr != FALSE) {

				fputws(lpwsMessage, stderr);
				fputwc(L'\n', stderr);
				fputws(lpwsStackTrace, stderr);
			}

			//display information as message box
			if (visible != FALSE && ErrLib_fOutputMbox != FALSE) {

				switch (type) {
				case MSG_ERROR: mbox_type = MB_ICONERROR; break;
				case MSG_WARNING: mbox_type = MB_ICONWARNING; caption = L"Warning"; break;
				case MSG_INFORMATION: mbox_type = MB_ICONINFORMATION; caption = L"Information"; break;
				default: mbox_type = 0; break;
				}

				MessageBoxW(NULL, lpwsMessage, caption, MB_SYSTEMMODAL | MB_OK | mbox_type);
			}

		}
		__finally {
			LeaveCriticalSection(&ErrLib_LogSync);
		}
	}

	// *** Getters ***

	//Get exception code for current exception in ERRLIB_CATCH/ERRLIB_CATCH_ALL block
	ERRLIB_API DWORD __stdcall ErrLib_Except_GetCode() {
		return *((DWORD*)ErrLib_LastExceptionCode_GetPointer());
	}

	// *** Catch functions ***

	ERRLIB_API LONG __stdcall ErrLib_CatchCode(struct _EXCEPTION_POINTERS* ex, DWORD FilteredCode) {


		if (ex->ExceptionRecord->ExceptionCode == FilteredCode) {
			*((DWORD*)ErrLib_LastExceptionCode_GetPointer()) = ex->ExceptionRecord->ExceptionCode;
			ErrLib_GetExceptionMessage(ex, ErrLib_Except_GetMessage(), ErrLib_MessageLen);
			ErrLib_PrintStack(ex->ContextRecord, ErrLib_Except_GetStackTrace(), ErrLib_StackLen);
			return EXCEPTION_EXECUTE_HANDLER;
		}
		else return EXCEPTION_CONTINUE_SEARCH;
	}

	ERRLIB_API LONG __stdcall ErrLib_CatchAll(struct _EXCEPTION_POINTERS* ex) {

		*((DWORD*)ErrLib_LastExceptionCode_GetPointer()) = ex->ExceptionRecord->ExceptionCode;
		ErrLib_GetExceptionMessage(ex, ErrLib_Except_GetMessage(), ErrLib_MessageLen);
		ErrLib_PrintStack(ex->ContextRecord, ErrLib_Except_GetStackTrace(), ErrLib_StackLen);
		return EXCEPTION_EXECUTE_HANDLER;
	}

#ifdef __cplusplus
} //extern "C" 
#endif

//called on unhandled exception
LONG WINAPI ErrLib_MyUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo) {

	WCHAR mes[ErrLib_MessageLen] = { 0 };
	WCHAR buf[ErrLib_StackLen] = { 0 };
	LONG result = EXCEPTION_CONTINUE_SEARCH;

	if (ExceptionInfo->ExceptionRecord->ExceptionCode == STATUS_CONTROL_C_EXIT) return EXCEPTION_CONTINUE_SEARCH;

	ErrLib_GetExceptionMessage(ExceptionInfo, mes, sizeof(mes) / sizeof(WCHAR));
	ErrLib_PrintStack(ExceptionInfo->ContextRecord, buf, sizeof(buf) / sizeof(WCHAR));

	//call user callback, if one exists, otherwise call default callback
	if (ErrLib_pCurrentExceptionCallback != NULL)
	{
		result = ErrLib_pCurrentExceptionCallback(ExceptionInfo, mes, buf);
	}
	else
	{
		result = ErrLib_DefaultExceptionCallback(ExceptionInfo, mes, buf);
	}


	//call previous exception filter, if there's one
	if (ErrLib_pPreviousExceptionFilter != NULL) {
		result = ErrLib_pPreviousExceptionFilter(ExceptionInfo);
	}

	return result;
	/*system("PAUSE");
	exit(1);	*/
}

LONG WINAPI ErrLib_DefaultExceptionCallback(struct _EXCEPTION_POINTERS* ex, LPCWSTR lpwsMessage, LPCWSTR lpwsStackTrace) {

	ErrLib_LogExceptionInfo(ex->ExceptionRecord->ExceptionCode, lpwsMessage, lpwsStackTrace, TRUE);
	return EXCEPTION_CONTINUE_SEARCH; //crash program
}
