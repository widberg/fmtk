socket = require('socket')

fmtk.log.debug("TEST MOD", fmtk.fs.get_current_file_path())
fmtk.log.debug("TEST MOD", socket._VERSION)
v = fmtk.get_fmtklua_version()
fmtk.log.debug(string.format("TEST MOD", "v%d.%d.%d.%d", v.major, v.minor, v.patch, v.tweak))
