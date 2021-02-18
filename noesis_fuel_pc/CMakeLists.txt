add_library(noesis_fuel_pc SHARED
	noesis_fuel_pc.cpp
	noesisplugin.cpp
	pluginsupport.cpp
	pluginshare.h pluginbasetypes.h pluginclasses.h
)

set_target_properties(noesis_fuel_pc PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
