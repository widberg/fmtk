#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <cassert>

#define FMTK_ASSERT(condition, message) assert((message, condition))
#define FMTK_STATIC_ASSERT(condition, message) static_assert(condition, message)

#if defined(_DEBUG) || defined(DEBUG) || !defined(NDEBUG)
#define FMTK_DEBUG 1
#else
#define FMTK_DEBUG 0
#endif

#define FMTK_BREAKPOINT() __debugbreak()

#define FMTK_BREAKPOINT_IF(condition) do { if (condition) { FMTK_BREAKPOINT(); } } while(0)

#endif // !DEBUG_HPP
