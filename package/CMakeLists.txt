add_custom_target(package
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/mods" "$<TARGET_FILE_DIR:fmtk>/mods"
	COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtklua>" "$<TARGET_FILE_DIR:fmtk>/mods/native/fmtklua.dll"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/README.md" "$<TARGET_FILE_DIR:fmtk>/README.md"
	COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/package/uninstall_fmtk.bat" "$<TARGET_FILE_DIR:fmtk>/uninstall_fmtk.bat"
	COMMAND
		${CMAKE_COMMAND} -E tar "cf" "${CMAKE_BINARY_DIR}/fmtk.zip" --format=zip
		"$<TARGET_FILE:fmtk>"
		"$<TARGET_FILE:fmtkdll>"
		"$<TARGET_FILE:secudll>"
		"$<TARGET_FILE:lua51>"
		"$<TARGET_FILE_DIR:fmtk>/mods"
		"$<TARGET_FILE_DIR:fmtk>/README.md"
		"$<TARGET_FILE_DIR:fmtk>/uninstall_fmtk.bat"
	WORKING_DIRECTORY "$<TARGET_FILE_DIR:fmtk>"
	DEPENDS fmtk fmtkdll secudll fmtklua
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

add_dependencies(package package_fmtksdk)
