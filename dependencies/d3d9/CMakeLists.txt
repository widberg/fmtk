add_library(d3d9 INTERFACE
	Include/d3d9.h
	Include/d3d9types.h
	Include/d3dx9anim.h
	Include/d3dx9effect.h
	Include/d3dx9math.inl
	Include/d3dx9shader.h
	Include/d3dx9tex.h
	Include/d3d9caps.h
	Include/d3dx9.h
	Include/d3dx9core.h
	Include/d3dx9math.h
	Include/d3dx9mesh.h
	Include/d3dx9shape.h
	Include/d3dx9xof.h
)

target_include_directories(d3d9 INTERFACE
	Include
)
