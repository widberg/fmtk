add_library(secudll SHARED
	secudll.cpp
)
target_link_libraries(secudll common detours tomlplusplus::tomlplusplus dbghelp)
target_link_options(secudll PRIVATE /export:DetourFinishHelperProcess,@1,NONAME)
