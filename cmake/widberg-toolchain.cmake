set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)
set(CMAKE_SYSTEM_PROCESSOR X86)

set(triple i386-pc-win32-msvc)

file(TO_CMAKE_PATH "${LLVM_WIDBERG}" LLVM_WIDBERG)
set(CMAKE_C_COMPILER "${LLVM_WIDBERG}/bin/clang-cl.exe")
set(CMAKE_C_COMPILER_TARGET "${triple}")
set(CMAKE_CXX_COMPILER "${LLVM_WIDBERG}/bin/clang-cl.exe")
set(CMAKE_CXX_COMPILER_TARGET "${triple}")
set(CMAKE_ASM_COMPILER "${LLVM_WIDBERG}/bin/clang-cl.exe")
set(CMAKE_ASM_COMPILER_TARGET "${triple}")

set(CMAKE_LINKER "${LLVM_WIDBERG}/bin/lld-link.exe")
set(CMAKE_AR "${LLVM_WIDBERG}/bin/llvm-lib.exe")
set(CMAKE_RC "${LLVM_WIDBERG}/bin/llvm-rc.exe")

# Keep LuaJIT from building host tools for 64-bit Windows
set(CMAKE_CROSSCOMPILING FALSE)
