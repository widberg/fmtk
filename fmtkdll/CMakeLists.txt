set(fmtkdll_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/fmtkdll.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/fmtkdll.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/instrument.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/instrument.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/sehexception.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sehexception.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/coremainloop.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/d3dxcompileshaderfromfilea.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/registercommand.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/validatememory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/createfilew.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/outputdebugstringa.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/runcommand.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/winmain.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/createwindowexw.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/outputdebugstringw.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/scriptmanagerinit.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/getplayerposition.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/readfile.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/idirectsoundbufferplay.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/gadgets/createdialogparama.hpp
)

add_sinker_target(fmtkdll_sinker
	INPUT
		${CMAKE_CURRENT_SOURCE_DIR}/fmtk.skr
		${fmtkdll_SOURCES}
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fmtk.skr
	DEFINITIONS ${CMAKE_CURRENT_BINARY_DIR}/include/fmtk.def
)

add_library(fmtkdll SHARED
	${fmtkdll_SOURCES}
)
add_dependencies(fmtkdll fmtkdll_sinker)
target_include_directories(fmtkdll PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(fmtkdll common fmtksdk detours spdlog d3d9 dbghelp tomlplusplus::tomlplusplus sinker)
target_link_options(fmtkdll PRIVATE /export:DetourFinishHelperProcess,@1,NONAME)
target_compile_options(fmtkdll PRIVATE /EHa)
target_compile_definitions(fmtkdll PRIVATE FMTK_INTERNAL)
