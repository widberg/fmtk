socket = require('socket')

fmtk.debug(fmtk.fs.get_current_file_path())
fmtk.debug(socket._VERSION)
v = fmtk.get_fmtklua_version()
fmtk.debug(string.format("v%d.%d.%d.%d", v.major, v.minor, v.patch, v.tweak))
