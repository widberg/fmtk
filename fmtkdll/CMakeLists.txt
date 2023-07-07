set(fmtkdll_SOURCES
	fmtkdll.cpp
	fmtkdll.hpp
	instrument.cpp
	instrument.hpp
	backward.hpp

	gadgets/coremainloop.hpp
	gadgets/d3dxcompileshaderfromfilea.hpp
	gadgets/registercommand.hpp
	gadgets/validatememory.hpp
	gadgets/createfilew.hpp
	gadgets/outputdebugstringa.hpp
	gadgets/runcommand.hpp
	gadgets/winmain.hpp
	gadgets/createwindowexw.hpp
	gadgets/outputdebugstringw.hpp
	gadgets/scriptmanagerinit.hpp
	gadgets/getplayerposition.hpp
	gadgets/readfile.hpp
	gadgets/idirectsoundbufferplay.hpp
	gadgets/createdialogparama.hpp
	gadgets/regqueryvalueexw.hpp
	gadgets/securom.hpp
	gadgets/input.hpp
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
target_link_libraries(fmtkdll common fmtksdk spdlog d3d9 dbghelp tomlplusplus::tomlplusplus sinker)
target_compile_definitions(fmtkdll PRIVATE FMTK_INTERNAL)
