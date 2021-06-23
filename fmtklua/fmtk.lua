--- FMTK package
fmtk = {}

-- @field INITIALIZE This hook is called before the game's WinMain entry point. At this point the ScriptManager is unavailable so calls to command functions will fail. This will always be the first hook to run.
-- @field TICK This hook is called once per execution of the game's CoreMainLoop function.
-- @field SCRIPT_MANAGER_INITIALIZE This hook is called once the ScriptManager is initialized. This is the first legal place to call the register command function. None of the FUEL commands have been registered yet so RunCommand with them will fail.
-- @field SHUTDOWN This hook is called before the FMTK dll is unloaded. This will always be the last hook to run.
EVENT = {}

--- register a hook
function fmtk.hook(event, id, callback) end

--- unregister a hook
function fmtk.unhook(event, id) end

--- get the version of FMTK SDK that was used to build FMTKLua
-- @return table version
function fmtk.get_fmtklua_version() end

--- run a command string
-- @return bool success
function fmtk.run_command(cmd) end

--- register command
function fmtk.register_command(name, callback) end

--- unregister a command
function fmtk.unregister_command(name) end

--- FMTK logging package
fmtk.log = {}

--- log a trace message
function fmtk.log.trace(source, msg) end

--- log a debug message
function fmtk.log.debug(source, msg) end

--- log an info message
function fmtk.log.info(source, msg) end

--- log a warining
function fmtk.log.warn(source, msg) end

--- log an error
function fmtk.log.error(source, msg) end

--- log a critical error
function fmtk.log.critical(source, msg) end

--- FMTK file system package
fmtk.fs = {}

--- alias a file path
function fmtk.fs.alias(oldPath, newPath) end

--- unalias a file path
function fmtk.fs.unalias(oldPath) end

-- get the mods directory path
-- @return string absolute mods directory path
function fmtk.fs.get_mods_directory_path() end

--- get the current lua file path
-- @return string absolute current lua file path
function get_current_file_path() end
