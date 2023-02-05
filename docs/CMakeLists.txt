find_package(Sphinx REQUIRED)

add_custom_target(fmtkdocs ALL
	COMMAND ${SPHINX_EXECUTABLE} -M html source build
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
