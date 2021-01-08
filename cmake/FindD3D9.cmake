set(D3D9_SEARCH_PATHS
	"$ENV{DX9SDK_DIR}/Include"
	"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
)
find_path(DIRECTX_INCLUDE_DIR d3dx9shader.h
	HINTS ${D3D9_SEARCH_PATHS}
	REQUIRED
)

set(D3D9_LIBRARY_PATHS
	"$ENV{DX9SDK_DIR}/Lib/x86/"
	"C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"
)
find_library(D3D9_LIB d3dx9
	HINTS ${D3D9_LIBRARY_PATHS}
	NO_DEFAULT_PATH
	REQUIRED
)

add_library(d3d9 STATIC IMPORTED GLOBAL)
set_target_properties(d3d9 PROPERTIES IMPORTED_LOCATION "${D3D9_LIB}")
target_include_directories(d3d9 INTERFACE "${DIRECTX_INCLUDE_DIR}")

SET(D3D9_FOUND "${D3D9_LIB}" CACHE INTERNAL "")
