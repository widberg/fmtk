add_library(fmtksdk INTERFACE
	include/fmtksdk/fmtksdk.hpp
	include/fmtksdk/placeholder.hpp
)

target_link_libraries(fmtksdk INTERFACE common)

target_include_directories(fmtksdk INTERFACE
	include
)
