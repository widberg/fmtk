#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
	#define DETOUR_DEBUG 1
	#define DETOUR_TRACE(x) DETOUR_TRACE_INTERNAL x
	#define DETOUR_TRACE_INTERNAL(fmt, ...) printf("[T] [DETOURS] " fmt, __VA_ARGS__)
	#define DETOUR_BREAK()  __debugbreak()
	#define _CRT_STDIO_ARBITRARY_WIDE_SPECIFIERS
	#include <stdio.h>
	#include <limits.h>
#endif
