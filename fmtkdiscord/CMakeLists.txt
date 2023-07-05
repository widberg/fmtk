add_library(fmtkdiscord SHARED
    fmtkdiscord.cpp
)
target_link_libraries(fmtkdiscord fmtksdk discord_game_sdk)
