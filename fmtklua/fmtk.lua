--- FMTK module
--- @module fmtk
fmtk = {}

--- EVENT enum
--- @table EVENT
EVENT = {
	INITIALIZE, --- This hook is called before the game's WinMain entry point. At this point the ScriptManager is unavailable so calls to command functions will fail. This will always be the first hook to run.
	TICK, --- This hook is called once per execution of the game's CoreMainLoop function.
	SCRIPT_MANAGER_INITIALIZE, --- This hook is called once the ScriptManager is initialized. This is the first legal place to call the register command function. None of the FUEL commands have been registered yet so RunCommand with them will fail.
	SHUTDOWN, --- This hook is called before the FMTK dll is unloaded. This will always be the last hook to run.
}

--- register a hook
--- @function hook
function fmtk.hook(event, id, callback) end

--- unregister a hook
--- @function unhook
function fmtk.unhook(event, id) end

--- get the version of FMTK SDK that was used to build FMTKLua
--- @return table version
--- @function get_fmtklua_version
function fmtk.get_fmtklua_version() end

--- run a command string
--- @return bool success
--- @function run_command
function fmtk.run_command(cmd) end

--- register command
--- @function register_command
function fmtk.register_command(name, callback) end

--- unregister a command
--- @function unregister_command
function fmtk.unregister_command(name) end

--- FMTK logging module
--- @table log
fmtk.log = {}

--- log a trace message
--- @function trace
function fmtk.log.trace(source, msg) end

--- log a debug message
--- @function debug
function fmtk.log.debug(source, msg) end

--- log an info message
--- @function info
function fmtk.log.info(source, msg) end

--- log a warining
--- @function warn
--- @function warn
function fmtk.log.warn(source, msg) end

--- log an error
--- @function error
function fmtk.log.error(source, msg) end

--- log a critical error
--- @function critical
function fmtk.log.critical(source, msg) end

--- FMTK file system module
--- @table fs
fmtk.fs = {}

--- alias a file path
--- @function alias
function fmtk.fs.alias(oldPath, newPath) end

--- unalias a file path
--- @function unalias
function fmtk.fs.unalias(oldPath) end

--- get the mods directory path
--- @function get_mods_directory_path
--- @return string absolute mods directory path
function fmtk.fs.get_mods_directory_path() end

--- get the current lua file path
--- @function get_current_file_path
--- @return string absolute current lua file path
function get_current_file_path() end
