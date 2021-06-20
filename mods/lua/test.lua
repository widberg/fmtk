socket = require('socket')

fmtk.print(fmtk.fs.get_current_file_path())
fmtk.print(socket._VERSION)
v = fmtk.get_fmtklua_version()
fmtk.print(string.format("v%d.%d.%d.%d", v.major, v.minor, v.patch, v.tweak))
