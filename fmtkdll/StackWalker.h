//Project: ErrLib
//Author: MSDN.WhiteKnight (https://github.com/MSDN-WhiteKnight)

#ifndef ErrLib_H_INCLUDED
#define ErrLib_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>

#include <windows.h>
#include <strsafe.h>
#include <process.h>
#include <DbgHelp.h>
#include <Shlobj.h>


#ifdef __cplusplus
#include <exception>
#include <comdef.h>
#define ERRLIB_REFERENCE
#else
#define ERRLIB_REFERENCE &
#endif

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "Dbghelp.lib")

#define ErrLib_MaxNameLen 300
#define ErrLib_MessageLen 1024
#define ErrLib_StackLen 10000

#define ERRLIB_API

// *** The following are the message definitions. ***

// MessageId: MSG_ERROR
#define MSG_ERROR                        ((DWORD)0xC0020100L)

// MessageId: MSG_WARNING
#define MSG_WARNING                      ((DWORD)0x80020101L)

// MessageId: MSG_INFORMATION
#define MSG_INFORMATION                  ((DWORD)0x40020102L)

// *** Configuration flags *** 

//Specifies the logging functions should write information into log file
#define ERRLIB_OUTPUT_LOGFILE 1

//Specifies the logging functions should write information into stderr stream
#define ERRLIB_OUTPUT_STDERR 2

//Specifies the logging functions should display information as message box
#define ERRLIB_OUTPUT_MBOX 3

//Specifies the logging functions should write information into Windows Event Log
#define ERRLIB_OUTPUT_EVENT_LOG 4

// *** Typedefs *** 

//Function pointer type used as unhandled exception callback
typedef LONG(WINAPI* ERRLIB_EXCEPTION_CALLBACK) (struct _EXCEPTION_POINTERS*, LPCWSTR, LPCWSTR);

// *** Custom exception codes for SEH *** 

//Win32 Exception
#define ERRLIB_WIN32_EXCEPTION  0xC0400000 

//Com Exception
#define ERRLIB_COM_EXCEPTION  0xC0400001

//Application Exception
#define ERRLIB_APP_EXCEPTION  0xC0400002 

// *** exported function declarations *** 

#ifdef __cplusplus
extern "C" {
#endif

	//Gets Win32 error message for specified function name and error code
	ERRLIB_API void __stdcall ErrLib_ErrorMes(LPTSTR lpszFunction, DWORD dw, WCHAR* buf);

	//Gets filename from full path
	ERRLIB_API WCHAR* __stdcall ErrLib_FileNameFromPathW(WCHAR* path);

	//Sets current exception callback. Specify NULL to call default callback
	ERRLIB_API void __stdcall ErrLib_SetExceptionCallback(ERRLIB_EXCEPTION_CALLBACK pCallback);

	//Sets current log file path. The default is [MyDocuments]\[ExeFileName].log
	ERRLIB_API void __stdcall ErrLib_SetLogFilePath(LPCWSTR path);

	//Sets value for the specified configuration parameter
	ERRLIB_API BOOL __stdcall ErrLib_SetParameter(UINT param, UINT_PTR value);

	//Initializes the library. Must be called before any other functionality is used.
	ERRLIB_API BOOL __stdcall ErrLib_Initialize();

	ERRLIB_API BOOL __stdcall ErrLib_InitTLS();
	ERRLIB_API BOOL __stdcall ErrLib_InitThread();
	ERRLIB_API void __stdcall ErrLib_FreeThread();

	//Creates registry data for Windows Event Log. Requires elevated priveleges. 
	ERRLIB_API BOOL __stdcall ErrLib_RegisterEventSource();

	//Deletes Windows Event Log registry data. Requires elevated priveleges. 
	ERRLIB_API BOOL __stdcall ErrLib_UnregisterEventSource();

	//Prints stack trace based on context record
	ERRLIB_API void __stdcall ErrLib_PrintStack(CONTEXT* ctx, WCHAR* dest, size_t cch);

	//Prints error message for specified exception
	ERRLIB_API void __stdcall ErrLib_GetExceptionMessage(struct _EXCEPTION_POINTERS* ExceptionInfo, LPWSTR dest, size_t cch);

	//Outputs exception information into configured log targets
	ERRLIB_API void __stdcall ErrLib_LogExceptionInfo(DWORD dwExcCode, LPCWSTR lpwsMessage, LPCWSTR lpwsStackTrace, BOOL visible);

	//Outputs arbitrary string into configured log targets (type can be: MSG_ERROR, MSG_WARNING or MSG_INFORMATION)
	ERRLIB_API void __stdcall ErrLib_LogMessage(LPCWSTR lpwsMessage, BOOL visible, DWORD type, BOOL bIncludeStack);

	//Get exception code for current exception in ERRLIB_CATCH/ERRLIB_CATCH_ALL block
	ERRLIB_API DWORD __stdcall ErrLib_Except_GetCode();

	//Get error message for current exception in ERRLIB_CATCH/ERRLIB_CATCH_ALL block
	ERRLIB_API LPWSTR __stdcall ErrLib_Except_GetMessage();

	//Get stack trace for current exception in ERRLIB_CATCH/ERRLIB_CATCH_ALL block
	ERRLIB_API LPWSTR __stdcall ErrLib_Except_GetStackTrace();

	ERRLIB_API LONG __stdcall ErrLib_CatchCode(struct _EXCEPTION_POINTERS* ex, DWORD FilteredCode);
	ERRLIB_API LONG __stdcall ErrLib_CatchAll(struct _EXCEPTION_POINTERS* ex);

	ERRLIB_API LPVOID __stdcall ErrLib_StrBuf_GetPointer();
	ERRLIB_API LPVOID __stdcall ErrLib_ExArgs_GetPointer();

#ifdef __cplusplus
	ERRLIB_API void __stdcall ErrLib_HResultToString(HRESULT hr, LPTSTR lpszFunction, WCHAR* buf);
}//extern "C"
#endif


//**** Helper macro functions *****

//Raises custom exception
#define ERRLIB_THROW(mes) {((ULONG_PTR*)ErrLib_ExArgs_GetPointer())[0]=(ULONG_PTR)mes;\
RaiseException(ERRLIB_APP_EXCEPTION,0,1,((ULONG_PTR*)ErrLib_ExArgs_GetPointer()));}

//Raises Win32 exception if variable is equal to the passed value
#define ERRLIB_THROW_IF_EQUAL(var,value,func) if((var)==(value)){DWORD ErrLibLocal_LastError=GetLastError();\
ErrLib_ErrorMes(L#func,ErrLibLocal_LastError,(WCHAR*)ErrLib_StrBuf_GetPointer());((ULONG_PTR*)ErrLib_ExArgs_GetPointer())[0]=(ULONG_PTR)ErrLibLocal_LastError;\
((ULONG_PTR*)ErrLib_ExArgs_GetPointer())[1]=(ULONG_PTR)ErrLib_StrBuf_GetPointer();RaiseException(ERRLIB_WIN32_EXCEPTION,0,2,(ULONG_PTR*)ErrLib_ExArgs_GetPointer());}

//Raises COM exception if passed HRESULT indicates failure
#define ERRLIB_THROW_IF_FAILED(var,func) if(FAILED(var)){ErrLib_HResultToString((DWORD)var,L#func,(WCHAR*)ErrLib_StrBuf_GetPointer());\
((ULONG_PTR*)ErrLib_ExArgs_GetPointer())[0]=(ULONG_PTR)var;((ULONG_PTR*)ErrLib_ExArgs_GetPointer())[1]=(ULONG_PTR)ErrLib_StrBuf_GetPointer();RaiseException(ERRLIB_COM_EXCEPTION,0,2,((ULONG_PTR*)ErrLib_ExArgs_GetPointer()));}

//Calls WINAPI function and raises Win32 exception if it fails
#define ERRLIB_INVOKEAPI(func, ...) if(FALSE == func( ##__VA_ARGS__ )){DWORD ErrLibLocal_LastError=GetLastError();\
ErrLib_ErrorMes((L#func),ErrLibLocal_LastError,(WCHAR*)ErrLib_StrBuf_GetPointer());\
((ULONG_PTR*)ErrLib_ExArgs_GetPointer())[0]=(ULONG_PTR)ErrLibLocal_LastError;((ULONG_PTR*)ErrLib_ExArgs_GetPointer())[1]=(ULONG_PTR)ErrLib_StrBuf_GetPointer();\
RaiseException(ERRLIB_WIN32_EXCEPTION,0,2,((ULONG_PTR*)ErrLib_ExArgs_GetPointer()));}

//SEH Catch block that executes handles for all exceptions
#define ERRLIB_CATCH_ALL __except(ErrLib_CatchAll(GetExceptionInformation()))

//SEH Catch block that executes handles for exceptions with specified code
#define ERRLIB_CATCH(code) __except(ErrLib_CatchCode(GetExceptionInformation(),code))

#endif