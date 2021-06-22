socket = require('socket')

fmtk.log.debug("TEST MOD", fmtk.fs.get_current_file_path())
fmtk.log.debug("TEST MOD", socket._VERSION)
v = fmtk.get_fmtklua_version()
fmtk.log.debug("TEST MOD", string.format("v%d.%d.%d.%d", v.major, v.minor, v.patch, v.tweak))
