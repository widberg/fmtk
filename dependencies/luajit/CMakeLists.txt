cmake_minimum_required(VERSION 3.15)

project(luajit VERSION 2.0.5 LANGUAGES C ASM)

set(LUAJIT_DIR ${CMAKE_CURRENT_LIST_DIR}/LuaJIT/src CACHE PATH "Location of luajit sources")

set(LJLIB_C
	${LUAJIT_DIR}/lib_base.c
	${LUAJIT_DIR}/lib_math.c
	${LUAJIT_DIR}/lib_bit.c
	${LUAJIT_DIR}/lib_string.c
	${LUAJIT_DIR}/lib_table.c
	${LUAJIT_DIR}/lib_io.c
	${LUAJIT_DIR}/lib_os.c
	${LUAJIT_DIR}/lib_debug.c
	${LUAJIT_DIR}/lib_jit.c
	${LUAJIT_DIR}/lib_ffi.c
	${LUAJIT_DIR}/lib_package.c
)

add_executable(minilua ${LUAJIT_DIR}/host/minilua.c)

# buildvm_arch_generator
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/LuaJIT/src/host/)
add_custom_target(buildvm_arch_generator
	COMMAND minilua ${LUAJIT_DIR}/../dynasm/dynasm.lua -D WIN -D FPU -D HFABI -o ${CMAKE_CURRENT_BINARY_DIR}/LuaJIT/src/host/buildvm_arch.h ${LUAJIT_DIR}/vm_x86.dasc
	DEPENDS minilua
	BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/LuaJIT/src/host/buildvm_arch.h
	WORKING_DIRECTORY ${LUAJIT_DIR}/../dynasm
)

# buildvm
add_executable(buildvm
	${LUAJIT_DIR}/host/buildvm.c
	${LUAJIT_DIR}/host/buildvm_asm.c
	${LUAJIT_DIR}/host/buildvm_fold.c
	${LUAJIT_DIR}/host/buildvm_lib.c
	${LUAJIT_DIR}/host/buildvm_peobj.c
)
target_include_directories(buildvm PRIVATE ${LUAJIT_DIR} ${CMAKE_CURRENT_BINARY_DIR}/LuaJIT/src/host)
add_dependencies(buildvm buildvm_arch_generator)

macro(add_buildvm_target _target _mode)
	add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_target}
		COMMAND buildvm ARGS -m ${_mode} -o ${CMAKE_CURRENT_BINARY_DIR}/${_target} ${ARGN}
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		DEPENDS buildvm ${ARGN}
	)
endmacro()

add_buildvm_target(lj_vm.obj peobj)
add_buildvm_target(lj_ffdef.h   ffdef   ${LJLIB_C})
add_buildvm_target(lj_bcdef.h  bcdef  ${LJLIB_C})
add_buildvm_target(lj_folddef.h folddef ${LUAJIT_DIR}/lj_opt_fold.c)
add_buildvm_target(lj_recdef.h  recdef  ${LJLIB_C})
add_buildvm_target(lj_libdef.h  libdef  ${LJLIB_C})
add_buildvm_target(jit/vmdef.lua  libvm  ${LJLIB_C})

## LuaJIT Library
set(LJCORE_C
	${CMAKE_CURRENT_BINARY_DIR}/lj_vm.obj
	${CMAKE_CURRENT_BINARY_DIR}/lj_ffdef.h
	${CMAKE_CURRENT_BINARY_DIR}/lj_bcdef.h
	${CMAKE_CURRENT_BINARY_DIR}/lj_folddef.h
	${CMAKE_CURRENT_BINARY_DIR}/lj_recdef.h
	${CMAKE_CURRENT_BINARY_DIR}/lj_libdef.h

	${LUAJIT_DIR}/lib_aux.c
	${LUAJIT_DIR}/lib_base.c
	${LUAJIT_DIR}/lib_bit.c
	${LUAJIT_DIR}/lib_debug.c
	${LUAJIT_DIR}/lib_ffi.c
	${LUAJIT_DIR}/lib_init.c
	${LUAJIT_DIR}/lib_io.c
	${LUAJIT_DIR}/lib_jit.c
	${LUAJIT_DIR}/lib_math.c
	${LUAJIT_DIR}/lib_os.c
	${LUAJIT_DIR}/lib_package.c
	${LUAJIT_DIR}/lib_string.c
	${LUAJIT_DIR}/lib_table.c
	${LUAJIT_DIR}/lj_alloc.c
	${LUAJIT_DIR}/lj_api.c
	${LUAJIT_DIR}/lj_asm.c
	${LUAJIT_DIR}/lj_assert.c
	${LUAJIT_DIR}/lj_bc.c
	${LUAJIT_DIR}/lj_bcread.c
	${LUAJIT_DIR}/lj_bcwrite.c
	${LUAJIT_DIR}/lj_buf.c
	${LUAJIT_DIR}/lj_carith.c
	${LUAJIT_DIR}/lj_ccall.c
	${LUAJIT_DIR}/lj_ccallback.c
	${LUAJIT_DIR}/lj_cconv.c
	${LUAJIT_DIR}/lj_cdata.c
	${LUAJIT_DIR}/lj_char.c
	${LUAJIT_DIR}/lj_clib.c
	${LUAJIT_DIR}/lj_cparse.c
	${LUAJIT_DIR}/lj_crecord.c
	${LUAJIT_DIR}/lj_ctype.c
	${LUAJIT_DIR}/lj_debug.c
	${LUAJIT_DIR}/lj_dispatch.c
	${LUAJIT_DIR}/lj_err.c
	${LUAJIT_DIR}/lj_ffrecord.c
	${LUAJIT_DIR}/lj_func.c
	${LUAJIT_DIR}/lj_gc.c
	${LUAJIT_DIR}/lj_gdbjit.c
	${LUAJIT_DIR}/lj_ir.c
	${LUAJIT_DIR}/lj_lex.c
	${LUAJIT_DIR}/lj_lib.c
	${LUAJIT_DIR}/lj_load.c
	${LUAJIT_DIR}/lj_mcode.c
	${LUAJIT_DIR}/lj_meta.c
	${LUAJIT_DIR}/lj_obj.c
	${LUAJIT_DIR}/lj_opt_dce.c
	${LUAJIT_DIR}/lj_opt_fold.c
	${LUAJIT_DIR}/lj_opt_loop.c
	${LUAJIT_DIR}/lj_opt_mem.c
	${LUAJIT_DIR}/lj_opt_narrow.c
	${LUAJIT_DIR}/lj_opt_sink.c
	${LUAJIT_DIR}/lj_opt_split.c
	${LUAJIT_DIR}/lj_parse.c
	${LUAJIT_DIR}/lj_prng.c
	${LUAJIT_DIR}/lj_profile.c
	${LUAJIT_DIR}/lj_record.c
	${LUAJIT_DIR}/lj_snap.c
	${LUAJIT_DIR}/lj_state.c
	${LUAJIT_DIR}/lj_str.c
	${LUAJIT_DIR}/lj_strfmt.c
	${LUAJIT_DIR}/lj_strfmt_num.c
	${LUAJIT_DIR}/lj_strscan.c
	${LUAJIT_DIR}/lj_tab.c
	${LUAJIT_DIR}/lj_trace.c
	${LUAJIT_DIR}/lj_udata.c
	${LUAJIT_DIR}/lj_vmevent.c
	${LUAJIT_DIR}/lj_vmmath.c
)

add_library(luajit STATIC ${LJCORE_C})
target_include_directories(luajit PUBLIC ${LUAJIT_DIR})
target_include_directories(luajit PRIVATE ${CMAKE_CURRENT_BINARY_DIR} dynasm ${LUAJIT_DIR}/host)
