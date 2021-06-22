add_custom_target(fmtkdocs ALL
	COMMAND sphinx-build -M html source build
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
