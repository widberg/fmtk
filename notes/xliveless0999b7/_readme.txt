XLiveLess - small replacement of the xlive.dll with no online support 

Notes:
  * run LaunchGTAIV.exe instead GTAIV.exe to prevent "drunk" camera
  * Plugin saveToGameFolder not included in this beta version
  
Main features:
  * complete replacement of the xlive.dll (original dll isn't required)
  * all online functions are disabled (to prevent cheating in multiplayer)
  * removed all anti-debugger checks
  * all game variables are made unprotected and can be accessed and changed by the trainers, control centers and other tools
  * in-built asi-loader (no need to replace original files)
  * asi-loader searches for .asi files in the root game folder and .asi and .dll in the 'plugins' folder

Additional features for GTA IV version 1.0.1.0-1.0.7.0, 1.0.4.2, EfCL 1.1.1.0-1.1.2.0: 
  * removed savefile check (you can load save file made by any offline XLive user)
  * removed files.txt check (integrated replacement of FileCheckFix and MagicPatch)
  * restored access to the system debugger in the critical error menu (removed loading of WER.dll)
  * enabled write access to memory in code and .rdata segments (no need to call VirtualProtect from a .asi)
  * disabled RGSC (since update 6, RGSC moved to the paul.dll)
  * moved 'savegames' folder (see below).

Additional features for Red Faction: Guerrilla:
  * removed savefile check

Installation:
  * unpack the file xlive.dll into the game folder (don't forget to remove the asi loader if there is one)
  * to turn on debug logging, unpack the file xlive_d.dll into the game folder and rename it to xlive.dll

Additional notes on savefiles:
  * savefiles are now located in the folder:
    (for Windows Vista) %USERPROFILE%\Documents\Rockstar Games\GTA IV\savegames\
    (for Windows XP) %USERPROFILE%\My Documents\Rockstar Games\GTA IV\savegames\
  	NOTE: don't forget to copy savegames to the new location

Plugin support:
  * Added plugin API (xliveless.h and xlive.lib)

Special thanks to Rick and Seemann.