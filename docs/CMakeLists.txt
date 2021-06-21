add_custom_target(fmtkdocs ALL
	COMMAND sphinx-build -M html source build
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
file(GENERATE OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile INPUT Doxyfile.in)
configure_file(source/version.txt.in ${CMAKE_CURRENT_SOURCE_DIR}/source/version.txt @ONLY)
