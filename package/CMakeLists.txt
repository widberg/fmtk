add_custom_target(package
COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/mods" "${CMAKE_BINARY_DIR}/mods"
COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:fmtklua>" "${CMAKE_BINARY_DIR}/mods/native/fmtklua.dll"
COMMAND
	${CMAKE_COMMAND} -E tar "cf" "${CMAKE_BINARY_DIR}/fmtk.zip" --format=zip
	"$<TARGET_FILE:fmtk>"
	"$<TARGET_FILE:fmtkdll>"
	"$<TARGET_FILE:secudll>"
	"$<TARGET_FILE:lua51>"
	"${CMAKE_SOURCE_DIR}/README.md"
	"${CMAKE_BINARY_DIR}/mods"
	"${CMAKE_SOURCE_DIR}/package/uninstall_fmtk.bat"
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
