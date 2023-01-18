local socket = require('socket')

fmtk.hook(fmtk.event.INITIALIZE, "test_mod_init_hook", function()
	fmtk.log.debug("TEST MOD", fmtk.fs.get_current_file_path())
	fmtk.log.debug("TEST MOD", socket._VERSION)
	v = fmtk.get_fmtklua_version()
	fmtk.log.debug("TEST MOD", string.format("v%d.%d.%d.%d", v.major, v.minor, v.patch, v.tweak))
end)

fmtk.hook(fmtk.event.SCRIPT_MANAGER_INITIALIZE, "test_mod_smi_hook", function()
	fmtk.log.info("TEST MOD", "Adding our command")
	fmtk.register_command("FmtkTestMod", function(args)
		fmtk.log.info("TEST MOD", "Our command was called")
	end)
end)

local bbb = false;
fmtk.hook(fmtk.event.TICK, "test_mod_tick", function()
	if not bbb then
		fmtk.run_command("DisableMOvie")
		bbb = true
	end
end)
