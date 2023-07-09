/*
 * backward.hpp
 * Copyright 2013 Google Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef H_6B9572DA_A64B_49E6_B234_051480991C89
#define H_6B9572DA_A64B_49E6_B234_051480991C89

#ifndef __cplusplus
#error "It's not going to compile without a C++ compiler..."
#endif

#if defined(BACKWARD_CXX11)
#elif defined(BACKWARD_CXX98)
#else
#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1800)
#define BACKWARD_CXX11
#define BACKWARD_ATLEAST_CXX11
#define BACKWARD_ATLEAST_CXX98
#if __cplusplus >= 201703L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201703L)
#define BACKWARD_ATLEAST_CXX17
#endif
#else
#define BACKWARD_CXX98
#define BACKWARD_ATLEAST_CXX98
#endif
#endif

// You can define one of the following (or leave it to the auto-detection):
//
// #define BACKWARD_SYSTEM_LINUX
//	- specialization for linux
//
// #define BACKWARD_SYSTEM_DARWIN
//	- specialization for Mac OS X 10.5 and later.
//
// #define BACKWARD_SYSTEM_WINDOWS
//  - specialization for Windows (Clang 9 and MSVC2017)
//
// #define BACKWARD_SYSTEM_UNKNOWN
//	- placebo implementation, does nothing.
//
#if defined(BACKWARD_SYSTEM_LINUX)
#elif defined(BACKWARD_SYSTEM_DARWIN)
#elif defined(BACKWARD_SYSTEM_UNKNOWN)
#elif defined(BACKWARD_SYSTEM_WINDOWS)
#else
#if defined(__linux) || defined(__linux__)
#define BACKWARD_SYSTEM_LINUX
#elif defined(__APPLE__)
#define BACKWARD_SYSTEM_DARWIN
#elif defined(_WIN32)
#define BACKWARD_SYSTEM_WINDOWS
#else
#define BACKWARD_SYSTEM_UNKNOWN
#endif
#endif

#define NOINLINE __attribute__((noinline))

#include "logging.hpp"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <new>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

#if defined(BACKWARD_SYSTEM_LINUX)

// On linux, backtrace can back-trace or "walk" the stack using the following
// libraries:
//
// #define BACKWARD_HAS_UNWIND 1
//  - unwind comes from libgcc, but I saw an equivalent inside clang itself.
//  - with unwind, the stacktrace is as accurate as it can possibly be, since
//  this is used by the C++ runtime in gcc/clang for stack unwinding on
//  exception.
//  - normally libgcc is already linked to your program by default.
//
// #define BACKWARD_HAS_LIBUNWIND 1
//  - libunwind provides, in some cases, a more accurate stacktrace as it knows
//  to decode signal handler frames and lets us edit the context registers when
//  unwinding, allowing stack traces over bad function references.
//
// #define BACKWARD_HAS_BACKTRACE == 1
//  - backtrace seems to be a little bit more portable than libunwind, but on
//  linux, it uses unwind anyway, but abstract away a tiny information that is
//  sadly really important in order to get perfectly accurate stack traces.
//  - backtrace is part of the (e)glib library.
//
// The default is:
// #define BACKWARD_HAS_UNWIND == 1
//
// Note that only one of the define should be set to 1 at a time.
//
#if BACKWARD_HAS_UNWIND == 1
#elif BACKWARD_HAS_LIBUNWIND == 1
#elif BACKWARD_HAS_BACKTRACE == 1
#else
#undef BACKWARD_HAS_UNWIND
#define BACKWARD_HAS_UNWIND 1
#undef BACKWARD_HAS_LIBUNWIND
#define BACKWARD_HAS_LIBUNWIND 0
#undef BACKWARD_HAS_BACKTRACE
#define BACKWARD_HAS_BACKTRACE 0
#endif

// On linux, backward can extract detailed information about a stack trace
// using one of the following libraries:
//
// #define BACKWARD_HAS_DW 1
//  - libdw gives you the most juicy details out of your stack traces:
//    - object filename
//    - function name
//    - source filename
//    - line and column numbers
//    - source code snippet (assuming the file is accessible)
//    - variable names (if not optimized out)
//    - variable values (not supported by backward-cpp)
//  - You need to link with the lib "dw":
//    - apt-get install libdw-dev
//    - g++/clang++ -ldw ...
//
// #define BACKWARD_HAS_BFD 1
//  - With libbfd, you get a fair amount of details:
//    - object filename
//    - function name
//    - source filename
//    - line numbers
//    - source code snippet (assuming the file is accessible)
//  - You need to link with the lib "bfd":
//    - apt-get install binutils-dev
//    - g++/clang++ -lbfd ...
//
// #define BACKWARD_HAS_DWARF 1
//  - libdwarf gives you the most juicy details out of your stack traces:
//    - object filename
//    - function name
//    - source filename
//    - line and column numbers
//    - source code snippet (assuming the file is accessible)
//    - variable names (if not optimized out)
//    - variable values (not supported by backward-cpp)
//  - You need to link with the lib "dwarf":
//    - apt-get install libdwarf-dev
//    - g++/clang++ -ldwarf ...
//
// #define BACKWARD_HAS_BACKTRACE_SYMBOL 1
//  - backtrace provides minimal details for a stack trace:
//    - object filename
//    - function name
//  - backtrace is part of the (e)glib library.
//
// The default is:
// #define BACKWARD_HAS_BACKTRACE_SYMBOL == 1
//
// Note that only one of the define should be set to 1 at a time.
//
#if BACKWARD_HAS_DW == 1
#elif BACKWARD_HAS_BFD == 1
#elif BACKWARD_HAS_DWARF == 1
#elif BACKWARD_HAS_BACKTRACE_SYMBOL == 1
#else
#undef BACKWARD_HAS_DW
#define BACKWARD_HAS_DW 0
#undef BACKWARD_HAS_BFD
#define BACKWARD_HAS_BFD 0
#undef BACKWARD_HAS_DWARF
#define BACKWARD_HAS_DWARF 0
#undef BACKWARD_HAS_BACKTRACE_SYMBOL
#define BACKWARD_HAS_BACKTRACE_SYMBOL 1
#endif

#include <cxxabi.h>
#include <fcntl.h>
#ifdef __ANDROID__
//		Old Android API levels define _Unwind_Ptr in both link.h and
// unwind.h 		Rename the one in link.h as we are not going to be using
// it
#define _Unwind_Ptr _Unwind_Ptr_Custom
#include <link.h>
#undef _Unwind_Ptr
#else
#include <link.h>
#endif
#if defined(__ppc__) || defined(__powerpc) || defined(__powerpc__) ||          \
    defined(__POWERPC__)
// Linux kernel header required for the struct pt_regs definition
// to access the NIP (Next Instruction Pointer) register value
#include <asm/ptrace.h>
#endif
#include <signal.h>
#include <sys/stat.h>
#include <syscall.h>
#include <unistd.h>
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#include <dlfcn.h>
#undef _GNU_SOURCE
#else
#include <dlfcn.h>
#endif

#if BACKWARD_HAS_BFD == 1
//              NOTE: defining PACKAGE{,_VERSION} is required before including
//                    bfd.h on some platforms, see also:
//                    https://sourceware.org/bugzilla/show_bug.cgi?id=14243
#ifndef PACKAGE
#define PACKAGE
#endif
#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION
#endif
#include <bfd.h>
#endif

#if BACKWARD_HAS_DW == 1
#include <dwarf.h>
#include <elfutils/libdw.h>
#include <elfutils/libdwfl.h>
#endif

#if BACKWARD_HAS_DWARF == 1
#include <algorithm>
#include <dwarf.h>
#include <libdwarf.h>
#include <libelf.h>
#include <map>
#endif

#if (BACKWARD_HAS_BACKTRACE == 1) || (BACKWARD_HAS_BACKTRACE_SYMBOL == 1)
// then we shall rely on backtrace
#include <execinfo.h>
#endif

#endif // defined(BACKWARD_SYSTEM_LINUX)

#if defined(BACKWARD_SYSTEM_DARWIN)
// On Darwin, backtrace can back-trace or "walk" the stack using the following
// libraries:
//
// #define BACKWARD_HAS_UNWIND 1
//  - unwind comes from libgcc, but I saw an equivalent inside clang itself.
//  - with unwind, the stacktrace is as accurate as it can possibly be, since
//  this is used by the C++ runtime in gcc/clang for stack unwinding on
//  exception.
//  - normally libgcc is already linked to your program by default.
//
// #define BACKWARD_HAS_LIBUNWIND 1
//  - libunwind comes from clang, which implements an API compatible version.
//  - libunwind provides, in some cases, a more accurate stacktrace as it knows
//  to decode signal handler frames and lets us edit the context registers when
//  unwinding, allowing stack traces over bad function references.
//
// #define BACKWARD_HAS_BACKTRACE == 1
//  - backtrace is available by default, though it does not produce as much
//  information as another library might.
//
// The default is:
// #define BACKWARD_HAS_UNWIND == 1
//
// Note that only one of the define should be set to 1 at a time.
//
#if BACKWARD_HAS_UNWIND == 1
#elif BACKWARD_HAS_BACKTRACE == 1
#elif BACKWARD_HAS_LIBUNWIND == 1
#else
#undef BACKWARD_HAS_UNWIND
#define BACKWARD_HAS_UNWIND 1
#undef BACKWARD_HAS_BACKTRACE
#define BACKWARD_HAS_BACKTRACE 0
#undef BACKWARD_HAS_LIBUNWIND
#define BACKWARD_HAS_LIBUNWIND 0
#endif

// On Darwin, backward can extract detailed information about a stack trace
// using one of the following libraries:
//
// #define BACKWARD_HAS_BACKTRACE_SYMBOL 1
//  - backtrace provides minimal details for a stack trace:
//    - object filename
//    - function name
//
// The default is:
// #define BACKWARD_HAS_BACKTRACE_SYMBOL == 1
//
#if BACKWARD_HAS_BACKTRACE_SYMBOL == 1
#else
#undef BACKWARD_HAS_BACKTRACE_SYMBOL
#define BACKWARD_HAS_BACKTRACE_SYMBOL 1
#endif

#include <cxxabi.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>

#if (BACKWARD_HAS_BACKTRACE == 1) || (BACKWARD_HAS_BACKTRACE_SYMBOL == 1)
#include <execinfo.h>
#endif
#endif // defined(BACKWARD_SYSTEM_DARWIN)

#if defined(BACKWARD_SYSTEM_WINDOWS)

#include <condition_variable>
#include <mutex>
#include <thread>

#include <basetsd.h>

#ifdef _WIN64
typedef SSIZE_T ssize_t;
#else
typedef int ssize_t;
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <winnt.h>

#include <psapi.h>
#include <signal.h>

#ifndef __clang__
#undef NOINLINE
#define NOINLINE __declspec(noinline)
#endif

#ifdef _MSC_VER
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")
#endif

// Comment / packing is from stackoverflow:
// https://stackoverflow.com/questions/6205981/windows-c-stack-trace-from-a-running-app/28276227#28276227
// Some versions of imagehlp.dll lack the proper packing directives themselves
// so we need to do it.
#pragma pack(push, before_imagehlp, 8)
#include <imagehlp.h>
#pragma pack(pop, before_imagehlp)

// TODO maybe these should be undefined somewhere else?
#undef BACKWARD_HAS_UNWIND
#undef BACKWARD_HAS_BACKTRACE
#if BACKWARD_HAS_PDB_SYMBOL == 1
#else
#undef BACKWARD_HAS_PDB_SYMBOL
#define BACKWARD_HAS_PDB_SYMBOL 1
#endif

#endif

#if BACKWARD_HAS_UNWIND == 1

#include <unwind.h>
// while gcc's unwind.h defines something like that:
//  extern _Unwind_Ptr _Unwind_GetIP (struct _Unwind_Context *);
//  extern _Unwind_Ptr _Unwind_GetIPInfo (struct _Unwind_Context *, int *);
//
// clang's unwind.h defines something like this:
//  uintptr_t _Unwind_GetIP(struct _Unwind_Context* __context);
//
// Even if the _Unwind_GetIPInfo can be linked to, it is not declared, worse we
// cannot just redeclare it because clang's unwind.h doesn't define _Unwind_Ptr
// anyway.
//
// Luckily we can play on the fact that the guard macros have a different name:
#ifdef __CLANG_UNWIND_H
// In fact, this function still comes from libgcc (on my different linux boxes,
// clang links against libgcc).
#include <inttypes.h>
extern "C" uintptr_t _Unwind_GetIPInfo(_Unwind_Context *, int *);
#endif

#endif // BACKWARD_HAS_UNWIND == 1

#if BACKWARD_HAS_LIBUNWIND == 1
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#endif // BACKWARD_HAS_LIBUNWIND == 1

#ifdef BACKWARD_ATLEAST_CXX11
#include <unordered_map>
#include <utility> // for std::swap
namespace backward {
namespace details {
template <typename K, typename V> struct hashtable {
  typedef std::unordered_map<K, V> type;
};
using std::move;
} // namespace details
} // namespace backward
#else // NOT BACKWARD_ATLEAST_CXX11
#define nullptr NULL
#define override
#include <map>
namespace backward {
namespace details {
template <typename K, typename V> struct hashtable {
  typedef std::map<K, V> type;
};
template <typename T> const T &move(const T &v) { return v; }
template <typename T> T &move(T &v) { return v; }
} // namespace details
} // namespace backward
#endif // BACKWARD_ATLEAST_CXX11

namespace backward {
namespace details {
#if defined(BACKWARD_SYSTEM_WINDOWS)
const char kBackwardPathDelimiter[] = ";";
#else
const char kBackwardPathDelimiter[] = ":";
#endif
} // namespace details
} // namespace backward

namespace backward {

namespace system_tag {
struct linux_tag; // seems that I cannot call that "linux" because the name
// is already defined... so I am adding _tag everywhere.
struct darwin_tag;
struct windows_tag;
struct unknown_tag;

#if defined(BACKWARD_SYSTEM_LINUX)
typedef linux_tag current_tag;
#elif defined(BACKWARD_SYSTEM_DARWIN)
typedef darwin_tag current_tag;
#elif defined(BACKWARD_SYSTEM_WINDOWS)
typedef windows_tag current_tag;
#elif defined(BACKWARD_SYSTEM_UNKNOWN)
typedef unknown_tag current_tag;
#else
#error "May I please get my system defines?"
#endif
} // namespace system_tag

namespace trace_resolver_tag {
#if defined(BACKWARD_SYSTEM_LINUX)
struct libdw;
struct libbfd;
struct libdwarf;
struct backtrace_symbol;

#if BACKWARD_HAS_DW == 1
typedef libdw current;
#elif BACKWARD_HAS_BFD == 1
typedef libbfd current;
#elif BACKWARD_HAS_DWARF == 1
typedef libdwarf current;
#elif BACKWARD_HAS_BACKTRACE_SYMBOL == 1
typedef backtrace_symbol current;
#else
#error "You shall not pass, until you know what you want."
#endif
#elif defined(BACKWARD_SYSTEM_DARWIN)
struct backtrace_symbol;

#if BACKWARD_HAS_BACKTRACE_SYMBOL == 1
typedef backtrace_symbol current;
#else
#error "You shall not pass, until you know what you want."
#endif
#elif defined(BACKWARD_SYSTEM_WINDOWS)
struct pdb_symbol;
#if BACKWARD_HAS_PDB_SYMBOL == 1
typedef pdb_symbol current;
#else
#error "You shall not pass, until you know what you want."
#endif
#endif
} // namespace trace_resolver_tag

namespace details {

template <typename T> struct rm_ptr {
  typedef T type;
};

template <typename T> struct rm_ptr<T *> {
  typedef T type;
};

template <typename T> struct rm_ptr<const T *> {
  typedef const T type;
};

template <typename R, typename T, R (*F)(T)> struct deleter {
  template <typename U> void operator()(U &ptr) const { (*F)(ptr); }
};

template <typename T> struct default_delete {
  void operator()(T &ptr) const { delete ptr; }
};

template <typename T, typename Deleter = deleter<void, void *, &::free>>
class handle {
  struct dummy;
  T _val;
  bool _empty;

#ifdef BACKWARD_ATLEAST_CXX11
  handle(const handle &) = delete;
  handle &operator=(const handle &) = delete;
#endif

public:
  ~handle() {
    if (!_empty) {
      Deleter()(_val);
    }
  }

  explicit handle() : _val(), _empty(true) {}
  explicit handle(T val) : _val(val), _empty(false) {
    if (!_val)
      _empty = true;
  }

#ifdef BACKWARD_ATLEAST_CXX11
  handle(handle &&from) : _empty(true) { swap(from); }
  handle &operator=(handle &&from) {
    swap(from);
    return *this;
  }
#else
  explicit handle(const handle &from) : _empty(true) {
    // some sort of poor man's move semantic.
    swap(const_cast<handle &>(from));
  }
  handle &operator=(const handle &from) {
    // some sort of poor man's move semantic.
    swap(const_cast<handle &>(from));
    return *this;
  }
#endif

  void reset(T new_val) {
    handle tmp(new_val);
    swap(tmp);
  }

  void update(T new_val) {
    _val = new_val;
    _empty = !static_cast<bool>(new_val);
  }

  operator const dummy *() const {
    if (_empty) {
      return nullptr;
    }
    return reinterpret_cast<const dummy *>(_val);
  }
  T get() { return _val; }
  T release() {
    _empty = true;
    return _val;
  }
  void swap(handle &b) {
    using std::swap;
    swap(b._val, _val);     // can throw, we are safe here.
    swap(b._empty, _empty); // should not throw: if you cannot swap two
    // bools without throwing... It's a lost cause anyway!
  }

  T &operator->() { return _val; }
  const T &operator->() const { return _val; }

  typedef typename rm_ptr<T>::type &ref_t;
  typedef const typename rm_ptr<T>::type &const_ref_t;
  ref_t operator*() { return *_val; }
  const_ref_t operator*() const { return *_val; }
  ref_t operator[](size_t idx) { return _val[idx]; }

  // Watch out, we've got a badass over here
  T *operator&() {
    _empty = false;
    return &_val;
  }
};

// Default demangler implementation (do nothing).
template <typename TAG> struct demangler_impl {
  static std::string demangle(const char *funcname) { return funcname; }
};

struct demangler : public demangler_impl<system_tag::current_tag> {};

// Split a string on the platform's PATH delimiter.  Example: if delimiter
// is ":" then:
//   ""              --> []
//   ":"             --> ["",""]
//   "::"            --> ["","",""]
//   "/a/b/c"        --> ["/a/b/c"]
//   "/a/b/c:/d/e/f" --> ["/a/b/c","/d/e/f"]
//   etc.
inline std::vector<std::string> split_source_prefixes(const std::string &s) {
  std::vector<std::string> out;
  size_t last = 0;
  size_t next = 0;
  size_t delimiter_size = sizeof(kBackwardPathDelimiter) - 1;
  while ((next = s.find(kBackwardPathDelimiter, last)) != std::string::npos) {
    out.push_back(s.substr(last, next - last));
    last = next + delimiter_size;
  }
  if (last <= s.length()) {
    out.push_back(s.substr(last));
  }
  return out;
}

} // namespace details

/*************** A TRACE ***************/

struct Trace {
  void *addr;
  size_t idx;

  Trace() : addr(nullptr), idx(0) {}

  explicit Trace(void *_addr, size_t _idx) : addr(_addr), idx(_idx) {}
};

struct ResolvedTrace : public Trace {

  struct SourceLoc {
    std::string function;
    std::string filename;
    unsigned line;
    unsigned col;

    SourceLoc() : line(0), col(0) {}

    bool operator==(const SourceLoc &b) const {
      return function == b.function && filename == b.filename &&
             line == b.line && col == b.col;
    }

    bool operator!=(const SourceLoc &b) const { return !(*this == b); }
  };

  // In which binary object this trace is located.
  std::string object_filename;

  // The function in the object that contain the trace. This is not the same
  // as source.function which can be an function inlined in object_function.
  std::string object_function;

  // The source location of this trace. It is possible for filename to be
  // empty and for line/col to be invalid (value 0) if this information
  // couldn't be deduced, for example if there is no debug information in the
  // binary object.
  SourceLoc source;

  std::string module_name;

  // An optionals list of "inliners". All the successive sources location
  // from where the source location of the trace (the attribute right above)
  // is inlined. It is especially useful when you compiled with optimization.
  typedef std::vector<SourceLoc> source_locs_t;
  source_locs_t inliners;

  ResolvedTrace() : Trace() {}
  ResolvedTrace(const Trace &mini_trace) : Trace(mini_trace) {}
};

/*************** STACK TRACE ***************/

// default implemention.
template <typename TAG> class StackTraceImpl {
public:
  size_t size() const { return 0; }
  Trace operator[](size_t) const { return Trace(); }
  size_t load_here(size_t = 0) { return 0; }
  size_t load_from(void *, size_t = 0, void * = nullptr, void * = nullptr) {
    return 0;
  }
  size_t thread_id() const { return 0; }
  std::string exception_description() const { return ""; }
  void skip_n_firsts(size_t) {}
  void *const *begin() const { return nullptr; }
};

class StackTraceImplBase {
public:
  StackTraceImplBase()
      : _thread_id(0), _exception_description(""), _skip(0), _context(nullptr),
        _error_addr(nullptr) {}

  size_t thread_id() const { return _thread_id; }

  void set_exception_description(std::string const &exception_description) {
    _exception_description = exception_description;
  }
  std::string const &exception_description() const {
    return _exception_description;
  }

  void skip_n_firsts(size_t n) { _skip = n; }

protected:
  void load_thread_info() {}

  void set_context(void *context) { _context = context; }
  void *context() const { return _context; }

  void set_error_addr(void *error_addr) { _error_addr = error_addr; }
  void *error_addr() const { return _error_addr; }

  size_t skip_n_firsts() const { return _skip; }

private:
  size_t _thread_id;
  std::string _exception_description;
  size_t _skip;
  void *_context;
  void *_error_addr;
};

class StackTraceImplHolder : public StackTraceImplBase {
public:
  size_t size() const {
    return (_stacktrace.size() >= skip_n_firsts())
               ? _stacktrace.size() - skip_n_firsts()
               : 0;
  }
  Trace operator[](size_t idx) const {
    if (idx >= size()) {
      return Trace();
    }
    return Trace(_stacktrace[idx + skip_n_firsts()], idx);
  }
  void *const *begin() const {
    if (size()) {
      return &_stacktrace[skip_n_firsts()];
    }
    return nullptr;
  }

protected:
  std::vector<void *> _stacktrace;
};

template <>
class StackTraceImpl<system_tag::current_tag> : public StackTraceImplHolder {
public:
  // We have to load the machine type from the image info
  // So we first initialize the resolver, and it tells us this info
  void set_machine_type(DWORD machine_type) { machine_type_ = machine_type; }
  void set_context(CONTEXT *ctx) { ctx_ = ctx; }
  void set_thread_handle(HANDLE handle) { thd_ = handle; }

  NOINLINE
  size_t load_here(size_t depth = 32, void *context = nullptr,
                   void *error_addr = nullptr) {
    set_context(static_cast<CONTEXT *>(context));
    set_error_addr(error_addr);
    CONTEXT localCtx; // used when no context is provided

    if (depth == 0) {
      return 0;
    }

    if (!ctx_) {
      ctx_ = &localCtx;
      RtlCaptureContext(ctx_);
    }

    if (!thd_) {
      thd_ = GetCurrentThread();
    }

    HANDLE process = GetCurrentProcess();

    STACKFRAME64 s;
    memset(&s, 0, sizeof(STACKFRAME64));

    // TODO: 32 bit context capture
    s.AddrStack.Mode = AddrModeFlat;
    s.AddrFrame.Mode = AddrModeFlat;
    s.AddrPC.Mode = AddrModeFlat;
#ifdef _M_X64
    s.AddrPC.Offset = ctx_->Rip;
    s.AddrStack.Offset = ctx_->Rsp;
    s.AddrFrame.Offset = ctx_->Rbp;
#else
    s.AddrPC.Offset = ctx_->Eip;
    s.AddrStack.Offset = ctx_->Esp;
    s.AddrFrame.Offset = ctx_->Ebp;
#endif

    if (!machine_type_) {
#ifdef _M_X64
      machine_type_ = IMAGE_FILE_MACHINE_AMD64;
#else
      machine_type_ = IMAGE_FILE_MACHINE_I386;
#endif
    }

    for (;;) {
      // NOTE: this only works if PDBs are already loaded!
      SetLastError(0);
      if (!StackWalk64(machine_type_, process, thd_, &s, ctx_, NULL,
                       SymFunctionTableAccess64, SymGetModuleBase64, NULL))
        break;

      if (s.AddrReturn.Offset == 0)
        break;

      _stacktrace.push_back(reinterpret_cast<void *>(s.AddrPC.Offset));

      if (size() >= depth)
        break;
    }

    return size();
  }

  size_t load_from(void *addr, size_t depth = 32, void *context = nullptr,
                   void *error_addr = nullptr) {
    load_here(depth + 8, context, error_addr);

    for (size_t i = 0; i < _stacktrace.size(); ++i) {
      if (_stacktrace[i] == addr) {
        skip_n_firsts(i);
        break;
      }
    }

    _stacktrace.resize(std::min(_stacktrace.size(), skip_n_firsts() + depth));
    return size();
  }

private:
  DWORD machine_type_ = 0;
  HANDLE thd_ = 0;
  CONTEXT *ctx_ = nullptr;
};

class StackTrace : public StackTraceImpl<system_tag::current_tag> {};

/*************** TRACE RESOLVER ***************/

class TraceResolverImplBase {
public:
  virtual ~TraceResolverImplBase() {}

  virtual void load_addresses(void *const *addresses, int address_count) {
    (void)addresses;
    (void)address_count;
  }

  template <class ST> void load_stacktrace(ST &st) {
    load_addresses(st.begin(), static_cast<int>(st.size()));
  }

  virtual ResolvedTrace resolve(ResolvedTrace t) { return t; }

protected:
  std::string demangle(const char *funcname) {
    return _demangler.demangle(funcname);
  }

private:
  details::demangler _demangler;
};

template <typename TAG> class TraceResolverImpl;

#ifdef BACKWARD_SYSTEM_UNKNOWN

template <>
class TraceResolverImpl<system_tag::unknown_tag>
    : public TraceResolverImplBase {};

#endif

#ifdef BACKWARD_SYSTEM_WINDOWS

// Load all symbol info
// Based on:
// https://stackoverflow.com/questions/6205981/windows-c-stack-trace-from-a-running-app/28276227#28276227

struct module_data {
  std::string image_name;
  std::string module_name;
  void *base_address;
  DWORD load_size;
};

class get_mod_info {
  HANDLE process;
  static const int buffer_length = 4096;

public:
  get_mod_info(HANDLE h) : process(h) {}

  module_data operator()(HMODULE module) {
    module_data ret;
    char temp[buffer_length];
    MODULEINFO mi;

    GetModuleInformation(process, module, &mi, sizeof(mi));
    ret.base_address = mi.lpBaseOfDll;
    ret.load_size = mi.SizeOfImage;

    GetModuleFileNameExA(process, module, temp, sizeof(temp));
    ret.image_name = temp;
    GetModuleBaseNameA(process, module, temp, sizeof(temp));
    ret.module_name = temp;
    std::vector<char> img(ret.image_name.begin(), ret.image_name.end());
    std::vector<char> mod(ret.module_name.begin(), ret.module_name.end());
    SymLoadModule64(process, 0, &img[0], &mod[0], (DWORD64)ret.base_address,
                    ret.load_size);
    return ret;
  }
};

template <>
class TraceResolverImpl<system_tag::windows_tag>
    : public TraceResolverImplBase {
public:
  TraceResolverImpl() {

    HANDLE process = GetCurrentProcess();

    std::vector<module_data> modules;
    DWORD cbNeeded;
    std::vector<HMODULE> module_handles(1);
    SymInitialize(process, "mods/fmtk/pdbs", false);
    DWORD symOptions = SymGetOptions();
    symOptions |= SYMOPT_LOAD_LINES | SYMOPT_UNDNAME;
    SymSetOptions(symOptions);
    EnumProcessModules(
        process, &module_handles[0],
        static_cast<DWORD>(module_handles.size() * sizeof(HMODULE)), &cbNeeded);
    module_handles.resize(cbNeeded / sizeof(HMODULE));
    EnumProcessModules(
        process, &module_handles[0],
        static_cast<DWORD>(module_handles.size() * sizeof(HMODULE)), &cbNeeded);
    std::transform(module_handles.begin(), module_handles.end(),
                   std::back_inserter(modules), get_mod_info(process));
    void *base = modules[0].base_address;
    IMAGE_NT_HEADERS *h = ImageNtHeader(base);
    image_type = h->FileHeader.Machine;
  }

  static const int max_sym_len = 255;
  struct symbol_t {
    SYMBOL_INFO sym;
    char buffer[max_sym_len];
  } sym;

  DWORD64 displacement;

  ResolvedTrace resolve(ResolvedTrace t) override {
    HANDLE process = GetCurrentProcess();

    memset(&sym, 0, sizeof(sym));
    sym.sym.SizeOfStruct = sizeof(SYMBOL_INFO);
    sym.sym.MaxNameLen = max_sym_len;

    if (SymFromAddr(process, (ULONG64)t.addr, &displacement, &sym.sym)) {
      char name[256];
      if (UnDecorateSymbolName(sym.sym.Name, (PSTR)name, 256,
                               UNDNAME_COMPLETE)) {
        t.object_function = name;
        t.object_filename = "";
        t.source.function = name;
      } else {
        t.object_function = sym.sym.Name;
        t.object_filename = "";
        t.source.function = sym.sym.Name;
      }
    } else {
      t.object_function = "";
      t.object_filename = "";
      t.source.function = "";
    }

    DWORD offset = 0;
    IMAGEHLP_LINE line;
    if (SymGetLineFromAddr(process, (ULONG64)t.addr, &offset, &line)) {
      t.object_filename = line.FileName;
      t.source.filename = line.FileName;
      t.source.line = line.LineNumber;
      t.source.col = offset;
    }

    HMODULE hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                          GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                      (LPCTSTR)t.addr, &hModule);
    if (hModule != NULL) {
      char module[MAX_PATH + 1];
      GetModuleFileNameA(hModule, module, MAX_PATH + 1);
      t.module_name = module;
    }

    return t;
  }

  DWORD machine_type() const { return image_type; }

private:
  DWORD image_type;
};

#endif

class TraceResolver : public TraceResolverImpl<system_tag::current_tag> {};

/*************** CODE SNIPPET ***************/

class SourceFile {
public:
  typedef std::vector<std::pair<unsigned, std::string>> lines_t;

  SourceFile() {}
  SourceFile(const std::string &path) {
    // 1. If FMTK_SOURCE_PREFIXES is set then assume it contains
    //    a colon-separated list of path prefixes.  Try prepending each
    //    to the given path until a valid file is found.
    const std::vector<std::string> &prefixes = get_paths_from_env_variable();
    for (size_t i = 0; i < prefixes.size(); ++i) {
      // Double slashes (//) should not be a problem.
      std::string new_path = prefixes[i] + '/' + path;
      _file.reset(new std::ifstream(new_path.c_str()));
      if (is_open())
        break;
    }
    // 2. If no valid file found then fallback to opening the path as-is.
    if (!_file || !is_open()) {
      _file.reset(new std::ifstream(path.c_str()));
    }
  }
  bool is_open() const { return _file->is_open(); }

  lines_t &get_lines(unsigned line_start, unsigned line_count, lines_t &lines) {
    using namespace std;
    // This function make uses of the dumbest algo ever:
    //	1) seek(0)
    //	2) read lines one by one and discard until line_start
    //	3) read line one by one until line_start + line_count
    //
    // If you are getting snippets many time from the same file, it is
    // somewhat a waste of CPU, feel free to benchmark and propose a
    // better solution ;)

    _file->clear();
    _file->seekg(0);
    string line;
    unsigned line_idx;

    for (line_idx = 1; line_idx < line_start; ++line_idx) {
      std::getline(*_file, line);
      if (!*_file) {
        return lines;
      }
    }

    // think of it like a lambda in C++98 ;)
    // but look, I will reuse it two times!
    // What a good boy am I.
    struct isspace {
      bool operator()(char c) { return std::isspace(c); }
    };

    bool started = false;
    for (; line_idx < line_start + line_count; ++line_idx) {
      getline(*_file, line);
      if (!*_file) {
        return lines;
      }
      if (!started) {
        if (std::find_if(line.begin(), line.end(), not_isspace()) == line.end())
          continue;
        started = true;
      }
      lines.push_back(make_pair(line_idx, line));
    }

    lines.erase(
        std::find_if(lines.rbegin(), lines.rend(), not_isempty()).base(),
        lines.end());
    return lines;
  }

  lines_t get_lines(unsigned line_start, unsigned line_count) {
    lines_t lines;
    return get_lines(line_start, line_count, lines);
  }

  // there is no find_if_not in C++98, lets do something crappy to
  // workaround.
  struct not_isspace {
    bool operator()(char c) { return !std::isspace(c); }
  };
  // and define this one here because C++98 is not happy with local defined
  // struct passed to template functions, fuuuu.
  struct not_isempty {
    bool operator()(const lines_t::value_type &p) {
      return !(std::find_if(p.second.begin(), p.second.end(), not_isspace()) ==
               p.second.end());
    }
  };

  void swap(SourceFile &b) { _file.swap(b._file); }

#ifdef BACKWARD_ATLEAST_CXX11
  SourceFile(SourceFile &&from) : _file(nullptr) { swap(from); }
  SourceFile &operator=(SourceFile &&from) {
    swap(from);
    return *this;
  }
#else
  explicit SourceFile(const SourceFile &from) {
    // some sort of poor man's move semantic.
    swap(const_cast<SourceFile &>(from));
  }
  SourceFile &operator=(const SourceFile &from) {
    // some sort of poor man's move semantic.
    swap(const_cast<SourceFile &>(from));
    return *this;
  }
#endif

  // Allow adding to paths gotten from FMTK_SOURCE_PREFIXES after
  // loading the library; this can be useful when the library is loaded when the
  // locations are unknown Warning: Because this edits the static paths
  // variable, it is *not* intrinsiclly thread safe
  static void add_paths_to_env_variable_impl(const std::string &to_add) {
    get_mutable_paths_from_env_variable().push_back(to_add);
  }

private:
  details::handle<std::ifstream *, details::default_delete<std::ifstream *>>
      _file;

  static std::vector<std::string> get_paths_from_env_variable_impl() {
    std::vector<std::string> paths;
    const char *prefixes_str = std::getenv("FMTK_SOURCE_PREFIXES");
    if (prefixes_str && prefixes_str[0]) {
      paths = details::split_source_prefixes(prefixes_str);
    }
    return paths;
  }

  static std::vector<std::string> &get_mutable_paths_from_env_variable() {
    static volatile std::vector<std::string> paths =
        get_paths_from_env_variable_impl();
    return const_cast<std::vector<std::string> &>(paths);
  }

  static const std::vector<std::string> &get_paths_from_env_variable() {
    return get_mutable_paths_from_env_variable();
  }

#ifdef BACKWARD_ATLEAST_CXX11
  SourceFile(const SourceFile &) = delete;
  SourceFile &operator=(const SourceFile &) = delete;
#endif
};

class SnippetFactory {
public:
  typedef SourceFile::lines_t lines_t;

  lines_t get_snippet(const std::string &filename, unsigned line_start,
                      unsigned context_size) {

    SourceFile &src_file = get_src_file(filename);
    unsigned start = line_start - context_size / 2;
    return src_file.get_lines(start, context_size);
  }

  lines_t get_combined_snippet(const std::string &filename_a, unsigned line_a,
                               const std::string &filename_b, unsigned line_b,
                               unsigned context_size) {
    SourceFile &src_file_a = get_src_file(filename_a);
    SourceFile &src_file_b = get_src_file(filename_b);

    lines_t lines =
        src_file_a.get_lines(line_a - context_size / 4, context_size / 2);
    src_file_b.get_lines(line_b - context_size / 4, context_size / 2, lines);
    return lines;
  }

  lines_t get_coalesced_snippet(const std::string &filename, unsigned line_a,
                                unsigned line_b, unsigned context_size) {
    SourceFile &src_file = get_src_file(filename);

    using std::max;
    using std::min;
    unsigned a = min(line_a, line_b);
    unsigned b = max(line_a, line_b);

    if ((b - a) < (context_size / 3)) {
      return src_file.get_lines((a + b - context_size + 1) / 2, context_size);
    }

    lines_t lines = src_file.get_lines(a - context_size / 4, context_size / 2);
    src_file.get_lines(b - context_size / 4, context_size / 2, lines);
    return lines;
  }

private:
  typedef details::hashtable<std::string, SourceFile>::type src_files_t;
  src_files_t _src_files;

  SourceFile &get_src_file(const std::string &filename) {
    src_files_t::iterator it = _src_files.find(filename);
    if (it != _src_files.end()) {
      return it->second;
    }
    SourceFile &new_src_file = _src_files[filename];
    new_src_file = SourceFile(filename);
    return new_src_file;
  }
};

/*************** PRINTER ***************/

namespace ColorMode {
enum type { automatic, never, always };
}

class cfile_streambuf : public std::streambuf {
public:
  cfile_streambuf(FILE *_sink) : sink(_sink) {}
  int_type underflow() override { return traits_type::eof(); }
  int_type overflow(int_type ch) override {
    if (traits_type::not_eof(ch) && fputc(ch, sink) != EOF) {
      return ch;
    }
    return traits_type::eof();
  }

  std::streamsize xsputn(const char_type *s, std::streamsize count) override {
    return static_cast<std::streamsize>(
        fwrite(s, sizeof *s, static_cast<size_t>(count), sink));
  }

#ifdef BACKWARD_ATLEAST_CXX11
public:
  cfile_streambuf(const cfile_streambuf &) = delete;
  cfile_streambuf &operator=(const cfile_streambuf &) = delete;
#else
private:
  cfile_streambuf(const cfile_streambuf &);
  cfile_streambuf &operator=(const cfile_streambuf &);
#endif

private:
  FILE *sink;
  std::vector<char> buffer;
};

namespace Color {
enum type { yellow = 0, purple = 0, reset = 0 };
} // namespace Color

class Colorize {
public:
  Colorize(std::ostream &) {}
  void activate(ColorMode::type) {}
  void activate(ColorMode::type, FILE *) {}
  void set_color(Color::type) {}
};

class Printer {
public:
  bool snippet;
  ColorMode::type color_mode;
  bool address;
  bool object;
  int inliner_context_size;
  int trace_context_size;
  bool reverse;

  Printer()
      : snippet(true), color_mode(ColorMode::automatic), address(false),
        object(false), inliner_context_size(5), trace_context_size(7),
        reverse(false) {}

  template <typename ST> FILE *print(ST &st, FILE *fp = stderr) {
    cfile_streambuf obuf(fp);
    std::ostream os(&obuf);
    Colorize colorize(os);
    colorize.activate(color_mode, fp);
    print_stacktrace(st, os, colorize);
    return fp;
  }

  template <typename ST> std::ostream &print(ST &st, std::ostream &os) {
    Colorize colorize(os);
    colorize.activate(color_mode);
    print_stacktrace(st, os, colorize);
    return os;
  }

  template <typename IT>
  FILE *print(IT begin, IT end, FILE *fp = stderr, size_t thread_id = 0,
              std::string const &exception_description = "") {
    cfile_streambuf obuf(fp);
    std::ostream os(&obuf);
    Colorize colorize(os);
    colorize.activate(color_mode, fp);
    print_stacktrace(begin, end, os, thread_id, exception_description,
                     colorize);
    return fp;
  }

  template <typename IT>
  std::ostream &print(IT begin, IT end, std::ostream &os, size_t thread_id = 0,
                      std::string const &exception_description = "") {
    Colorize colorize(os);
    colorize.activate(color_mode);
    print_stacktrace(begin, end, os, thread_id, exception_description,
                     colorize);
    return os;
  }

  TraceResolver const &resolver() const { return _resolver; }

private:
  TraceResolver _resolver;
  SnippetFactory _snippets;

  template <typename ST>
  void print_stacktrace(ST &st, std::ostream &os, Colorize &colorize) {
    print_header(os, st.thread_id(), st.exception_description());
    _resolver.load_stacktrace(st);
    if (reverse) {
      for (size_t trace_idx = st.size(); trace_idx > 0; --trace_idx) {
        print_trace(os, _resolver.resolve(st[trace_idx - 1]), colorize);
      }
    } else {
      for (size_t trace_idx = 0; trace_idx < st.size(); ++trace_idx) {
        print_trace(os, _resolver.resolve(st[trace_idx]), colorize);
      }
    }
  }

  template <typename IT>
  void print_stacktrace(IT begin, IT end, std::ostream &os, size_t thread_id,
                        std::string const &exception_description,
                        Colorize &colorize) {
    print_header(os, thread_id, exception_description);
    for (; begin != end; ++begin) {
      print_trace(os, *begin, colorize);
    }
  }

  void print_header(std::ostream &os, size_t thread_id,
                    std::string const &exception_description) {
    if (exception_description.size()) {
      os << "Exception";
      if (thread_id) {
        os << " in thread " << thread_id;
      }
      os << ":\n";
      os << exception_description;
    }

    os << "Stack trace";
    if (thread_id) {
      os << " in thread " << thread_id;
    }
    os << ":\n";
  }

  void print_trace(std::ostream &os, const ResolvedTrace &trace,
                   Colorize &colorize) {
    os << "#" << std::left << std::setw(2) << trace.idx << std::right;
    bool already_indented = true;

    os << " " << trace.module_name;

    if (trace.object_filename.size()) {
      os << " Object \"" << trace.object_filename << "\"";
    }
    os << " at " << trace.addr;
    if (trace.object_function.size()) {
      os << ", in " << trace.object_function;
    }
    os << "\n";
    already_indented = false;

    for (size_t inliner_idx = trace.inliners.size(); inliner_idx > 0;
         --inliner_idx) {
      if (!already_indented) {
        os << "   ";
      }
      const ResolvedTrace::SourceLoc &inliner_loc =
          trace.inliners[inliner_idx - 1];
      print_source_loc(os, " | ", inliner_loc);
      if (snippet) {
        print_snippet(os, "    | ", inliner_loc, colorize, Color::purple,
                      inliner_context_size);
      }
      already_indented = false;
    }

    if (trace.source.filename.size()) {
      if (!already_indented) {
        os << "   ";
      }
      print_source_loc(os, "   ", trace.source, trace.addr);
      if (snippet) {
        print_snippet(os, "      ", trace.source, colorize, Color::yellow,
                      trace_context_size);
      }
    }
  }

  void print_snippet(std::ostream &os, const char *indent,
                     const ResolvedTrace::SourceLoc &source_loc,
                     Colorize &colorize, Color::type color_code,
                     int context_size) {
    using namespace std;
    typedef SnippetFactory::lines_t lines_t;

    lines_t lines = _snippets.get_snippet(source_loc.filename, source_loc.line,
                                          static_cast<unsigned>(context_size));

    for (lines_t::const_iterator it = lines.begin(); it != lines.end(); ++it) {
      if (it->first == source_loc.line) {
        colorize.set_color(color_code);
        os << indent << ">";
      } else {
        os << indent << " ";
      }
      os << std::setw(4) << it->first << ": " << it->second << "\n";
      if (it->first == source_loc.line) {
        colorize.set_color(Color::reset);
      }
    }
  }

  void print_source_loc(std::ostream &os, const char *indent,
                        const ResolvedTrace::SourceLoc &source_loc,
                        void *addr = nullptr) {
    os << indent << "Source \"" << source_loc.filename << "\", line "
       << source_loc.line << ", in " << source_loc.function;

    if (address && addr != nullptr) {
      os << " [" << addr << "]";
    }
    os << "\n";
  }
};

/*************** SIGNALS HANDLING ***************/

class SignalHandling {
public:
  SignalHandling(const std::vector<int> & = std::vector<int>())
      : reporter_thread_([]() {
          /* We handle crashes in a utility thread:
            backward structures and some Windows functions called here
            need stack space, which we do not have when we encounter a
            stack overflow.
            To support reporting stack traces during a stack overflow,
            we create a utility thread at startup, which waits until a
            crash happens or the program exits normally. */

          {
            std::unique_lock<std::mutex> lk(mtx());
            cv().wait(lk, [] { return crashed() != crash_status::running; });
          }
          if (crashed() == crash_status::crashed) {
            handle_stacktrace(skip_recs());
          }
          {
            std::unique_lock<std::mutex> lk(mtx());
            crashed() = crash_status::ending;
          }
          cv().notify_one();
        }) {
    SetUnhandledExceptionFilter(crash_handler);

    signal(SIGABRT, signal_handler);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

    std::set_terminate(&terminator);
#ifndef BACKWARD_ATLEAST_CXX17
    std::set_unexpected(&terminator);
#endif
    _set_purecall_handler(&terminator);
    _set_invalid_parameter_handler(&invalid_parameter_handler);
  }
  bool loaded() const { return true; }

  ~SignalHandling() {
    {
      std::unique_lock<std::mutex> lk(mtx());
      crashed() = crash_status::normal_exit;
    }

    cv().notify_one();

    reporter_thread_.join();
  }

private:
  static CONTEXT *ctx() {
    static CONTEXT data;
    return &data;
  }

  static std::string &exception_description() {
    static std::string exception_description;
    return exception_description;
  }

  enum class crash_status { running, crashed, normal_exit, ending };

  static crash_status &crashed() {
    static crash_status data;
    return data;
  }

  static std::mutex &mtx() {
    static std::mutex data;
    return data;
  }

  static std::condition_variable &cv() {
    static std::condition_variable data;
    return data;
  }

  static HANDLE &thread_handle() {
    static HANDLE handle;
    return handle;
  }

  std::thread reporter_thread_;

  // TODO: how not to hardcode these?
  static const constexpr int signal_skip_recs =
#ifdef __clang__
      // With clang, RtlCaptureContext also captures the stack frame of the
      // current function Below that, there are 3 internal Windows functions
      4
#else
      // With MSVC cl, RtlCaptureContext misses the stack frame of the current
      // function The first entries during StackWalk are the 3 internal Windows
      // functions
      3
#endif
      ;

  static int &skip_recs() {
    static int data;
    return data;
  }

  static inline void terminator() {
    crash_handler(signal_skip_recs);
    abort();
  }

  static inline void signal_handler(int) {
    crash_handler(signal_skip_recs);
    abort();
  }

  static inline void __cdecl invalid_parameter_handler(const wchar_t *,
                                                       const wchar_t *,
                                                       const wchar_t *,
                                                       unsigned int,
                                                       uintptr_t) {
    crash_handler(signal_skip_recs);
    abort();
  }

  NOINLINE static LONG WINAPI crash_handler(EXCEPTION_POINTERS *info) {
    // The exception info supplies a trace from exactly where the issue was,
    // no need to skip records
    crash_handler(0, info->ContextRecord, info->ExceptionRecord);
    return EXCEPTION_CONTINUE_SEARCH;
  }

  static std::string exception_code_to_string(DWORD code) {
    switch (code) {
    case EXCEPTION_ACCESS_VIOLATION:
      return "Access Violation!";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
      return "Array out of bounds access error!";
    case EXCEPTION_DATATYPE_MISALIGNMENT:
      return "Read / write of misaligned data!";
    case EXCEPTION_FLT_DENORMAL_OPERAND:
      return "Denormal floating point operation!";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
      return "Unhandled floating point division by zero!";
    case EXCEPTION_FLT_INEXACT_RESULT:
      return "Inexact floating point result!";
    case EXCEPTION_FLT_INVALID_OPERATION:
      return "Invalid floating point operation!";
    case EXCEPTION_FLT_OVERFLOW:
      return "Floating point overflow!";
    case EXCEPTION_FLT_STACK_CHECK:
      return "Floating point stack overflow / underflow!";
    case EXCEPTION_FLT_UNDERFLOW:
      return "Floating point underflow!";
    case EXCEPTION_ILLEGAL_INSTRUCTION:
      return "Illegal CPU instruction!\nPossibly newer CPU architecture is "
             "required";
    case EXCEPTION_IN_PAGE_ERROR:
      return "In page error!\nPossibly network connection error when running "
             "the program over network";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
      return "Unhandled integer division by zero!";
    case EXCEPTION_INT_OVERFLOW:
      return "Integer overflow!";
    case EXCEPTION_INVALID_DISPOSITION:
      return "Invalid disposition error!";
    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
      return "Noncontinuable exception occured!";
    case EXCEPTION_PRIV_INSTRUCTION:
      return "Error due to invalid call to priviledged instruction!";
    case EXCEPTION_STACK_OVERFLOW:
      return "Stack overflow error!";
    default:
      return "Unknown exception!";
    }
  }

  static std::string format_exception_record(EXCEPTION_RECORD *er) {
    std::ostringstream oss;
    oss << "Exception code: " << exception_code_to_string(er->ExceptionCode)
        << " (" << std::hex << (void *)er->ExceptionCode << ")\n";
    oss << "Exception flags: " << std::hex << er->ExceptionFlags << "\n";
    oss << "Exception address: " << std::hex << er->ExceptionAddress << "\n";
    oss << "Number of parameters: " << std::dec << er->NumberParameters << "\n";
    for (DWORD i = 0; i < er->NumberParameters; ++i) {
      oss << "Parameter " << std::dec << i << ": " << std::hex
          << er->ExceptionInformation[i] << "\n";
    }
    return oss.str();
  }

  static std::string format_context(CONTEXT *ct) {
#ifdef _M_X64
    std::ostringstream oss;
    oss << "CS: " << std::hex << (void *)ct->SegCs << "\n";
    oss << "DS: " << std::hex << (void *)ct->SegDs << "\n";
    oss << "ES: " << std::hex << (void *)ct->SegEs << "\n";
    oss << "FS: " << std::hex << (void *)ct->SegFs << "\n";
    oss << "GS: " << std::hex << (void *)ct->SegGs << "\n";
    oss << "SS: " << std::hex << (void *)ct->SegSs << "\n";
    oss << "EFLAGS: " << std::hex << (void *)ct->EFlags << "\n";
    oss << "RAX: " << std::hex << (void *)ct->Rax << "\n";
    oss << "RBX: " << std::hex << (void *)ct->Rbx << "\n";
    oss << "RCX: " << std::hex << (void *)ct->Rcx << "\n";
    oss << "RDX: " << std::hex << (void *)ct->Rdx << "\n";
    oss << "RSP: " << std::hex << (void *)ct->Rsp << "\n";
    oss << "RBP: " << std::hex << (void *)ct->Rbp << "\n";
    oss << "RSI: " << std::hex << (void *)ct->Rsi << "\n";
    oss << "RDI: " << std::hex << (void *)ct->Rdi << "\n";
    oss << "R8: " << std::hex << (void *)ct->R8 << "\n";
    oss << "R9: " << std::hex << (void *)ct->R9 << "\n";
    oss << "R10: " << std::hex << (void *)ct->R10 << "\n";
    oss << "R11: " << std::hex << (void *)ct->R11 << "\n";
    oss << "R12: " << std::hex << (void *)ct->R12 << "\n";
    oss << "R13: " << std::hex << (void *)ct->R13 << "\n";
    oss << "R14: " << std::hex << (void *)ct->R14 << "\n";
    oss << "R15: " << std::hex << (void *)ct->R15 << "\n";
    oss << "RIP: " << std::hex << (void *)ct->Rip << "\n";
#else
    std::ostringstream oss;
    oss << "CS: " << std::hex << (void *)ct->SegCs << "\n";
    oss << "DS: " << std::hex << (void *)ct->SegDs << "\n";
    oss << "ES: " << std::hex << (void *)ct->SegEs << "\n";
    oss << "FS: " << std::hex << (void *)ct->SegFs << "\n";
    oss << "GS: " << std::hex << (void *)ct->SegGs << "\n";
    oss << "SS: " << std::hex << (void *)ct->SegSs << "\n";
    oss << "EFLAGS: " << std::hex << (void *)ct->EFlags << "\n";
    oss << "EAX: " << std::hex << (void *)ct->Eax << "\n";
    oss << "EBX: " << std::hex << (void *)ct->Ebx << "\n";
    oss << "ECX: " << std::hex << (void *)ct->Ecx << "\n";
    oss << "EDX: " << std::hex << (void *)ct->Edx << "\n";
    oss << "ESP: " << std::hex << (void *)ct->Esp << "\n";
    oss << "EBP: " << std::hex << (void *)ct->Ebp << "\n";
    oss << "ESI: " << std::hex << (void *)ct->Esi << "\n";
    oss << "EDI: " << std::hex << (void *)ct->Edi << "\n";
    oss << "EIP: " << std::hex << (void *)ct->Eip << "\n";
#endif
    return oss.str();
  }

  NOINLINE static void crash_handler(int skip, CONTEXT *ct = nullptr,
                                     EXCEPTION_RECORD *er = nullptr) {

    LOG(critical, FMTK,
        "Crash handler called; collecting exception information and stack "
        "trace.");

    if (ct == nullptr) {
      RtlCaptureContext(ctx());
    } else {
      memcpy(ctx(), ct, sizeof(CONTEXT));
    }

    if (er != nullptr) {
      exception_description() =
          format_exception_record(er) + format_context(ct);
    }

    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
                    GetCurrentProcess(), &thread_handle(), 0, FALSE,
                    DUPLICATE_SAME_ACCESS);

    skip_recs() = skip;

    {
      std::unique_lock<std::mutex> lk(mtx());
      crashed() = crash_status::crashed;
    }

    cv().notify_one();

    {
      std::unique_lock<std::mutex> lk(mtx());
      cv().wait(lk, [] { return crashed() != crash_status::crashed; });
    }
  }

  static void handle_stacktrace(int skip_frames = 0) {
    // printer creates the TraceResolver, which can supply us a machine type
    // for stack walking. Without this, StackTrace can only guess using some
    // macros.
    // StackTrace also requires that the PDBs are already loaded, which is
    // done in the constructor of TraceResolver
    Printer printer;

    StackTrace st;
    st.set_machine_type(printer.resolver().machine_type());
    st.set_thread_handle(thread_handle());
    st.load_here(32 + skip_frames, ctx());
    st.set_exception_description(exception_description());
    st.skip_n_firsts(skip_frames);

    printer.address = true;
    std::stringstream ss;
    printer.print(st, ss);
    LOG(critical, FMTK, "{}", ss.str());
  }
};

} // namespace backward

#endif /* H_GUARD */
