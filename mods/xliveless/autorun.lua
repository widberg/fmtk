-- FMTK_CURRENT_DIR only defined on startup
XLIVELESS_CURRENT_DIR = FMTK_CURRENT_DIR

fmtk.hook(FMTK_EVENT_ENTRY, function()
	fmtk.alias('xlive.dll', XLIVELESS_CURRENT_DIR .. '\\xlive.dll')
	fmtk.alias('xlive.ini', XLIVELESS_CURRENT_DIR .. '\\xlive.ini')
end)
