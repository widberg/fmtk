add_library(fmtkdll SHARED
	fmtkdll.cpp fmtkdll.hpp
	instrument.cpp instrument.hpp
	sehexception.cpp sehexception.hpp

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
)
target_link_libraries(fmtkdll common fmtksdk detours spdlog d3d9 dbghelp tomlplusplus::tomlplusplus usercall_hpp)
add_dependencies(fmtkdll hookcash)
target_link_options(fmtkdll PRIVATE /export:DetourFinishHelperProcess,@1,NONAME)
target_compile_options(fmtkdll PRIVATE /EHa)
target_compile_definitions(fmtkdll PRIVATE FMTK_INTERNAL)

set_target_properties(fmtkdll PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
