add_custom_target(package_fmtk
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/mods" "${CMAKE_BINARY_DIR}/mods"
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtklua>" "${CMAKE_BINARY_DIR}/mods/native/fmtklua.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:lua51>" "${CMAKE_BINARY_DIR}/mods/path/lua5.1.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtkdll>" "${CMAKE_BINARY_DIR}/mods/fmtk/fmtkdll.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:secudll>" "${CMAKE_BINARY_DIR}/mods/fmtk/secudll.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtk>" "${CMAKE_BINARY_DIR}/mods/fmtk/fmtk.exe"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/README.md" "${CMAKE_BINARY_DIR}/README.md"
	COMMAND
		${CMAKE_COMMAND} -E tar "cf" "${CMAKE_BINARY_DIR}/fmtk.zip" --format=zip
		"${CMAKE_BINARY_DIR}/mods"
		"${CMAKE_BINARY_DIR}/README.md"
	WORKING_DIRECTORY "$<TARGET_FILE_DIR:fmtk>"
	DEPENDS fmtk fmtkdll secudll fmtklua lua51
	BYPRODUCTS "${CMAKE_BINARY_DIR}/fmtk.zip"
)

add_custom_target(package_fmtksdk
	COMMAND
		${CMAKE_COMMAND} -E tar "cf" "${CMAKE_BINARY_DIR}/fmtksdk.zip" --format=zip
		"include"
		"CMakeLists.txt"
	WORKING_DIRECTORY "$<TARGET_PROPERTY:fmtksdk,BINARY_DIR>"
	DEPENDS fmtksdk
	BYPRODUCTS "${CMAKE_BINARY_DIR}/fmtksdk.zip"
)

add_dependencies(package_fmtk package_fmtksdk)
