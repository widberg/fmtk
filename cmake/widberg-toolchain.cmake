set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR X86)

set(triple i386-pc-win32-msvc)

file(TO_CMAKE_PATH "${LLVM_WIDBERG}" LLVM_WIDBERG)
set(CMAKE_C_COMPILER "${LLVM_WIDBERG}/clang-cl.exe")
set(CMAKE_C_COMPILER_TARGET "${triple}")
set(CMAKE_CXX_COMPILER "${LLVM_WIDBERG}/clang-cl.exe")
set(CMAKE_CXX_COMPILER_TARGET "${triple}")
set(CMAKE_ASM_COMPILER "${LLVM_WIDBERG}/clang-cl.exe")
set(CMAKE_ASM_COMPILER_TARGET "${triple}")

set(CMAKE_LINKER "${LLVM_WIDBERG}/lld-link.exe")
set(CMAKE_AR "${LLVM_WIDBERG}/llvm-lib.exe")
set(CMAKE_RC "${LLVM_WIDBERG}/llvm-rc.exe")
