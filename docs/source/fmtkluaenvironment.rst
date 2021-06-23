FMTK Lua Environment
====================

When the game loads it will recursively check each subdirectory of the `<mods>/lua` directory for files named `autorun.lua`. If found it will run the file. Directories starting with the character `_` will not be searched and neither will their subdirectories, this provides a way to disable mods without deleting them. You can rename individual mods to begin with the character `_` or move them to a directory named something like `_disabled`.

The Lua runtime is LuaJIT 2.1 which is build on Lua 5.1 with some forward compatibility features. `<mods>/lua/shared` has been added to the front of both the `path` and `cpath` Lua package environment variables. `<mods>/lua` has been added to the `path` Lua package environment variable after `shared` and before everything else.

By default the following standard Lua packages are available:

* base
* bit32
* coroutine
* debug
* ffi
* io
* jit
* math
* os
* package
* string
* table
* utf8

By default the following 3rd-party Lua packages are available:

* sockets
