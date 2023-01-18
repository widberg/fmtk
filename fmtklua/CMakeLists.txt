add_library(fmtklua SHARED
	fmtklua.cpp
)
target_link_libraries(fmtklua fmtksdk sol2 lua51)
