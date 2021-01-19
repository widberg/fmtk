-- FMTK_CURRENT_DIR only defined on startup
RAT_CURRENT_DIR = FMTK_CURRENT_DIR

fmtk.hook(FMTK_EVENT_COMMAND_INIT, function()
	fmtk.alias('DATAS\\PILOT.DPC', RAT_CURRENT_DIR .. '\\PILOT.DPC')
end)
