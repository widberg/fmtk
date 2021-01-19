-- FMTK_CURRENT_DIR only defined on startup
DELOREAN_CURRENT_DIR = FMTK_CURRENT_DIR
TT_DELOREAN_NAME_ID = 16109
TT_DELOREAN_MESH_NAME_ID = 16110

fmtk.hook(FMTK_EVENT_LOAD, function()
	-- fmtk.run_command('BSource ' .. DELOREAN_CURRENT_DIR .. '\\DeloreanDef.tsc ' .. TT_DELOREAN_NAME_ID .. ' ' .. TT_DELOREAN_MESH_NAME_ID)
end)
