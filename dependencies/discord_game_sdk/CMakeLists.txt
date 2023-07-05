cmake_minimum_required(VERSION 3.15)

project(discord_game_sdk VERSION 3.2.1 LANGUAGES CXX)

set(DISCORD_GAME_SDK_DIR ${CMAKE_CURRENT_LIST_DIR}/discord_game_sdk CACHE PATH "Location of discord_game_sdk sources")

add_library(discord_game_sdk_imported SHARED IMPORTED)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set_target_properties(discord_game_sdk_imported PROPERTIES
		IMPORTED_LOCATION ${DISCORD_GAME_SDK_DIR}/lib/x86_64/discord_game_sdk.dll
		IMPORTED_IMPLIB ${DISCORD_GAME_SDK_DIR}/lib/x86_64/discord_game_sdk.dll.lib
	)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	set_target_properties(discord_game_sdk_imported PROPERTIES
		IMPORTED_LOCATION ${DISCORD_GAME_SDK_DIR}/lib/x86/discord_game_sdk.dll
		IMPORTED_IMPLIB ${DISCORD_GAME_SDK_DIR}/lib/x86/discord_game_sdk.dll.lib
	)
endif()


set(DISCORD_GAME_SDK_CXX
	${DISCORD_GAME_SDK_DIR}/cpp/achievement_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/activity_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/application_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/core.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/image_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/lobby_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/network_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/overlay_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/relationship_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/storage_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/store_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/types.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/user_manager.cpp
	${DISCORD_GAME_SDK_DIR}/cpp/voice_manager.cpp
)

add_library(discord_game_sdk STATIC ${DISCORD_GAME_SDK_CXX})
target_include_directories(discord_game_sdk PUBLIC ${DISCORD_GAME_SDK_DIR}/cpp)
target_link_libraries(discord_game_sdk discord_game_sdk_imported)
