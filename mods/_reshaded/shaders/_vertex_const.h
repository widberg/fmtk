#ifndef	_VS_CONST_H
#define	_VS_CONST_H

//----------------------------------------
// Vertex-only constants
//----------------------------------------

//	***********************************************************
#define		C_END_CONSTANT					253

//	***********************************************************
//	*** CONSTANT MAPPING
//	***********************************************************
#define	C_VP_CONTEXT				1
#define	VIEWPORT_HALF_SIZE			c[C_VP_CONTEXT].xy
#define	VIEWPORT_HALF_SIZEX			c[C_VP_CONTEXT].x
#define	VIEWPORT_HALF_SIZEY			c[C_VP_CONTEXT].y
#define	INV_RDR_SIZE				c[C_VP_CONTEXT].zw
#define	VIEWPORT_RDR_CENTER			c[C_VP_CONTEXT+1].xy
#define	INV_DIST_FADE				c[C_VP_CONTEXT+1].z
//#define	c[C_VP_CONTEXT+1].w
#define	EYE_WORLD					c[C_VP_CONTEXT+2]
#define	CAM_WUP						c[C_VP_CONTEXT+3]
#define	CAM_WSIDE					c[C_VP_CONTEXT+4]
#define	C_VP_CONTEXT_END			C_VP_CONTEXT+5

#define	C_SCATTERING_CONTEXT			C_VP_CONTEXT_END
#define	C_SCATTERCONSTANTS1			c[C_SCATTERING_CONTEXT]
#define	C_SCATTERCONSTANTS2			c[C_SCATTERING_CONTEXT+1]
#define	C_TERM_MULTIPLIERS			c[C_SCATTERING_CONTEXT+2]
#define	C_BETA_1					c[C_SCATTERING_CONTEXT+3]
#define	C_BETA_DASH_1				c[C_SCATTERING_CONTEXT+4]
#define	C_BETA_2					c[C_SCATTERING_CONTEXT+5]
#define	C_BETA_DASH_2				c[C_SCATTERING_CONTEXT+6]
#define	C_BETA_1_PLUS_2				c[C_SCATTERING_CONTEXT+7]
#define	C_ONE_OVER_BETA_1_PLUS_2	c[C_SCATTERING_CONTEXT+8]	
#define	C_HG						c[C_SCATTERING_CONTEXT+9]
#define	C_SCATTERCONSTANTS3			c[C_SCATTERING_CONTEXT+10]
#define	C_SCATTERING_CONTEXT_END	C_SCATTERING_CONTEXT+11

#define	C_GROUND_CONTEXT			C_SCATTERING_CONTEXT_END
#define	C_GROUND_FADE_MIN			c[C_GROUND_CONTEXT].x
#define	C_GROUND_FADE_OVERMAXMIN	c[C_GROUND_CONTEXT].y
#define	C_GROUND_INVTEXSCALE		c[C_GROUND_CONTEXT].z
#define	C_GROUND_WINDX				c[C_GROUND_CONTEXT+1]
#define	C_GROUND_WINDZ				c[C_GROUND_CONTEXT+2]
#define	C_GROUND_CONTEXT_END		C_GROUND_CONTEXT+3

#define	C_VIEW_CONTEXT				C_GROUND_CONTEXT_END
#define VIEW_MAT					c[C_VIEW_CONTEXT]
//#define VIEW_MATRIX             transpose(float4x4(c[C_VIEW_CONTEXT],c[C_VIEW_CONTEXT+1],c[C_VIEW_CONTEXT+2],c[C_VIEW_CONTEXT+3]))
#define VIEW_MATRIX             GetMatrix( C_VIEW_CONTEXT )
#define	VIEW_TRANS					c[C_VIEW_CONTEXT+4]
#define	C_VIEW_CONTEXT_END			C_VIEW_CONTEXT+5

#define	C_PROJ_CONTEXT				C_VIEW_CONTEXT_END
//#define PROJ_MAT					c[C_PROJ_CONTEXT]
//#define PROJ_MATRIX             transpose(float4x4(c[C_PROJ_CONTEXT],c[C_PROJ_CONTEXT+1],c[C_PROJ_CONTEXT+2],c[C_PROJ_CONTEXT+3]))
#define PROJ_MATRIX             GetMatrix( C_PROJ_CONTEXT )
#define	C_PROJ_CONTEXT_END			C_PROJ_CONTEXT+4

#define	C_WORLD_CONTEXT				C_PROJ_CONTEXT_END
//#define WORLD_MAT					c[C_WORLD_CONTEXT]
//#define WORLD_MATRIX             transpose(float4x4(c[C_WORLD_CONTEXT],c[C_WORLD_CONTEXT+1],c[C_WORLD_CONTEXT+2],c[C_WORLD_CONTEXT+3]))
#define WORLD_MATRIX             GetMatrix( C_WORLD_CONTEXT )
#define	EYE_LOCAL					c[C_WORLD_CONTEXT+4]
#define	C_WORLD_CONTEXT_END			C_WORLD_CONTEXT+5

#define	C_SHADOWMAP_CONTEXT			C_WORLD_CONTEXT_END
#define	SHADOWMAP_MAT			GetMatrix( C_SHADOWMAP_CONTEXT )
#define	SHADOWMAP_VIEWPOS			c[C_SHADOWMAP_CONTEXT+4]
#define	SHADOWMAP_VERTEX_BIAS		c[C_SHADOWMAP_CONTEXT+5].x
#define	SHADOWMAP_VERTEX_ADDBIAS	c[C_SHADOWMAP_CONTEXT+5].y
#define	SHADOWMAP_TEXTURE_SIZE		c[C_SHADOWMAP_CONTEXT+5].z
#define	SHADOWMAP_DONTUSE			c[C_SHADOWMAP_CONTEXT+5].w	
#define	C_SHADOWMAP_CONTEXT_END		C_SHADOWMAP_CONTEXT+6

#define	C_SLICE_CONTEXT				C_SHADOWMAP_CONTEXT_END
#define	SLICE_MAT0				GetMatrix4x3(C_SLICE_CONTEXT)
#define	SLICE_MAT1				GetMatrix4x3(C_SLICE_CONTEXT+4)
#define	SLICE_MAT2				GetMatrix4x3(C_SLICE_CONTEXT+8)
#define	SLICE_MAT3				GetMatrix4x3(C_SLICE_CONTEXT+12)
#define C_SLICE_CONTEXT_END			(C_SLICE_CONTEXT+16)
#define C_SLICE_CONTEXT_SIZE		(C_SLICE_CONTEXT_SIZE-C_SLICE_CONTEXT)

// -- MATERIAL --
#define	C_MTL_CONTEXT				C_SLICE_CONTEXT_END
#define MTL_MAT						c[C_MTL_CONTEXT]
#define MTL_DIFFUSE					c[C_MTL_CONTEXT+2]
#define MTL_ADDSOUS_NULL			c[C_MTL_CONTEXT+2].w	//==0.f if Additif or Soustratif
#define MTL_EMISSIVE				c[C_MTL_CONTEXT+3]
#define	MTL_OPACITY					c[C_MTL_CONTEXT+3].w
#define	MTL_SPECULAR				c[C_MTL_CONTEXT+4]
#define	MTL_SPECULAR_POW			c[C_MTL_CONTEXT+4].w
#define	MTL_PARAMS					c[C_MTL_CONTEXT+5]
#define	MTL_FALLOFF_PERP			c[C_MTL_CONTEXT+6].x
#define	MTL_FALLOFF_PARA			c[C_MTL_CONTEXT+6].y
#define	C_MTL_CONTEXT_END			C_MTL_CONTEXT+7

// -- DIR LIGHT &FOG --
#define	C_LIGHT_CONTEXT				C_MTL_CONTEXT_END
#define	GOURAUD_DFLT				c[C_LIGHT_CONTEXT]
#define	DLIGHT_DIR					c[C_LIGHT_CONTEXT+1]
#define	DLIGHT_RADSPECULAR			c[C_LIGHT_CONTEXT+1].w
#define	DLIGHT_AMBIENT				c[C_LIGHT_CONTEXT+2]
#define	DLIGHT_AMBIENT_FACTOR		c[C_LIGHT_CONTEXT+2].w
#define	DLIGHT_COLOR				c[C_LIGHT_CONTEXT+3]
#define	DSKY_DIR					c[C_LIGHT_CONTEXT+4].xyz
#define	DSKY_COLOR					c[C_LIGHT_CONTEXT+5]
#define	DLIGHT_INVINTENSITY			c[C_LIGHT_CONTEXT+5].w		//Pour Moduler Emissive en Scattering
#define	DSKY_AMBIENT				c[C_LIGHT_CONTEXT+6]
#define	C_LIGHT_CONTEXT_END			C_LIGHT_CONTEXT+7

#define	C_FOG_CONTEXT				C_LIGHT_CONTEXT_END
#define FOG_PLANE					c[C_FOG_CONTEXT]
#define	FOG_COLOR					c[C_FOG_CONTEXT+1].xyz
#define	FOG_ON						c[C_FOG_CONTEXT+1].w
#define	C_FOG_CONTEXT_END			(C_FOG_CONTEXT+2)

// -- OMNIS --
#define	C_OMNI_SIZE					12

#define	C_OMNI_CONTEXT				C_FOG_CONTEXT+2
#define	OMNI_0_POS					c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+0]
#define	OMNI_0_STARTSQR				c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+0].w
#define	OMNI_0_DIR					c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+1]
#define	OMNI_0_RADSPECULAR			c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+1].w			//On Off in case of Radiosity Map
#define	OMNI_0_COLOR				c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+2]
#define	OMNI_0_RADDIFFUSE			c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+2].w			//On Off in case of Radiosity Map
#define	OMNI_0_ATT					c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+3].x
#define	OMNI_0_CUTOFF0				c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+3].y
#define	OMNI_0_CUTOFF1				c[C_OMNI_CONTEXT+0*C_OMNI_SIZE+3].z
#define	C_OMNI_0_PROJMATTEX			C_OMNI_CONTEXT+0*C_OMNI_SIZE+4			//Matrice Projection Texture
#define	C_OMNI_0_WORLDTOLOCAL		GetMatrix4x3( C_OMNI_CONTEXT+0*C_OMNI_SIZE+8 )			//World 2 Local

#define	OMNI_1_POS					c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+0]
#define	OMNI_1_STARTSQR				c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+0].w
#define	OMNI_1_DIR					c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+1]
#define	OMNI_1_RADSPECULAR			c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+1].w			//On Off in case of Radiosity Map
#define	OMNI_1_COLOR				c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+2]
#define	OMNI_1_RADDIFFUSE			c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+2].w			//On Off in case of Radiosity Map
#define	OMNI_1_ATT					c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+3].x
#define	OMNI_1_CUTOFF0				c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+3].y
#define	OMNI_1_CUTOFF1				c[C_OMNI_CONTEXT+1*C_OMNI_SIZE+3].z
#define	C_OMNI_1_PROJMATTEX			C_OMNI_CONTEXT+1*C_OMNI_SIZE+4
#define	C_OMNI_1_WORLDTOLOCAL		C_OMNI_CONTEXT+1*C_OMNI_SIZE+8			//World 2 Local

#define	OMNI_2_POS					c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+0]
#define	OMNI_2_STARTSQR				c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+0].w
#define	OMNI_2_DIR					c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+1]
#define	OMNI_2_RADSPECULAR			c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+1].w		//On Off in case of Radiosity Map
#define	OMNI_2_COLOR				c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+2]
#define	OMNI_2_RADDIFFUSE			c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+2].w		//On Off in case of Radiosity Map
#define	OMNI_2_ATT					c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+3].x
#define	OMNI_2_CUTOFF0				c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+3].y
#define	OMNI_2_CUTOFF1				c[C_OMNI_CONTEXT+2*C_OMNI_SIZE+3].z
#define	C_OMNI_2_PROJMATTEX			C_OMNI_CONTEXT+2*C_OMNI_SIZE+4
#define	C_OMNI_2_WORLDTOLOCAL		C_OMNI_CONTEXT+2*C_OMNI_SIZE+8

#define	OMNI_3_POS					c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+0]
#define	OMNI_3_STARTSQR				c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+0].w
#define	OMNI_3_DIR					c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+1]
#define	OMNI_3_RADSPECULAR			c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+1].w		//On Off in case of Radiosity Map
#define	OMNI_3_COLOR				c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+2]
#define	OMNI_3_RADDIFFUSE			c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+2].w		//On Off in case of Radiosity Map
#define	OMNI_3_ATT					c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+3].x
#define	OMNI_3_CUTOFF0				c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+3].y
#define	OMNI_3_CUTOFF1				c[C_OMNI_CONTEXT+3*C_OMNI_SIZE+3].z
#define	C_OMNI_3_PROJMATTEX			C_OMNI_CONTEXT+3*C_OMNI_SIZE+4
#define	C_OMNI_3_WORLDTOLOCAL		C_OMNI_CONTEXT+3*C_OMNI_SIZE+8

#define	OMNI_4_POS					c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+0]
#define	OMNI_4_STARTSQR				c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+0].w
#define	OMNI_4_DIR					c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+1]
#define	OMNI_4_RADSPECULAR			c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+1].w		//On Off in case of Radiosity Map
#define	OMNI_4_COLOR				c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+2]
#define	OMNI_4_RADDIFFUSE			c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+2].w		//On Off in case of Radiosity Map
#define	OMNI_4_ATT					c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+3].x
#define	OMNI_4_CUTOFF0				c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+3].y
#define	OMNI_4_CUTOFF1				c[C_OMNI_CONTEXT+4*C_OMNI_SIZE+3].z
#define	C_OMNI_4_PROJMATTEX			C_OMNI_CONTEXT+4*C_OMNI_SIZE+4
#define	C_OMNI_4_WORLDTOLOCAL		C_OMNI_CONTEXT+4*C_OMNI_SIZE+8

#define	C_OMNI_CONTEXT_END			C_OMNI_4_WORLDTOLOCAL+4

// -- VECTOR --
#define	C_DEFAULT					C_OMNI_CONTEXT+C_OMNI_SIZE+C_OMNI_SIZE+C_OMNI_SIZE
#define C_QUARTER					c[C_DEFAULT].x
#define C_DTIME						c[C_DEFAULT].y
#define C_ATIME						c[C_DEFAULT].z
#define	C_3QUARTER					c[C_DEFAULT].w
#define	C_NULL						c[C_DEFAULT+1].x
#define C_HALF						c[C_DEFAULT+1].y
#define C_UNIT						c[C_DEFAULT+1].z
#define C_TWO						c[C_DEFAULT+1].w
#define	C_INV16						c[C_DEFAULT+2].x
#define C_INV256					c[C_DEFAULT+2].y
#define C_INV4096					c[C_DEFAULT+2].z
#define C_INV16384					c[C_DEFAULT+2].w
#define	C_RIGHT						c[C_DEFAULT+1].zxxx			// 1 0 0 0
#define	C_UP						c[C_DEFAULT+1].xzxx			// 0 1 0 0
#define	C_FRONT						c[C_DEFAULT+1].xxzx			// 0 0 1 0	
#define	C_SINCOS1					c[C_DEFAULT+3]				// ( Pi, 0.5f, 2*Pi, 1/(2*Pi) )
#define	C_SINCOS2					c[C_DEFAULT+4]				// (1.f/(7!*128), 1.f/(6!*64), 1.f/(4!*16), 1.f/(5!*32) )
#define	C_SINCOS3					c[C_DEFAULT+5]				// (1.f/(3!*8), 1.f/(2!*8), 1.f, 0.5f )
#define	C_DEFAULT_END				C_DEFAULT+6

#define C_USER_CONTEXT				C_DEFAULT_END
#define C_USER0						c[C_USER_CONTEXT]
#define C_USER1						c[C_USER_CONTEXT+1]
#define C_USER2						c[C_USER_CONTEXT+2]
#define C_USER3						c[C_USER_CONTEXT+3]
#define C_USER4						c[C_USER_CONTEXT+4]		
#define C_USER5						c[C_USER_CONTEXT+5]
#define C_USER6						c[C_USER_CONTEXT+6]
#define C_USER7						c[C_USER_CONTEXT+7]
#define C_USER_CONTEXT_END			C_USER_CONTEXT+8


//	***********************************************************
// --  C_SHARED_ARRAY0  de Constante qui se supperposent
#define	C_SHARED_ARRAY0				C_USER_CONTEXT_END
//	***********************************************************

// -- VISIBILITY QUERY --
#define	C_VIZQUERY_CONTEXT			C_SHARED_ARRAY0
#define	VIZQUERY_XYZ				c[C_VIZQUERY_CONTEXT]
#define	VIZQUERY_SIZE				c[C_VIZQUERY_CONTEXT].w
#define	VIZQUERY_RESULT				c[C_VIZQUERY_CONTEXT+1].x
#define	C_VIZQUERY_CONTEXT_END		C_VIZQUERY_CONTEXT+2

// -- WATER --
#define	C_WATER_CONTEXT				C_SHARED_ARRAY0
#define C_WATER0					c[C_WATER_CONTEXT]
#define C_WATER1					c[C_WATER_CONTEXT+1]
#define C_WATER2					c[C_WATER_CONTEXT+2]
#define C_WATER3					c[C_WATER_CONTEXT+3]
#define C_WATER4					c[C_WATER_CONTEXT+4]		
#define C_WATER5					c[C_WATER_CONTEXT+5]
#define C_WATER6					c[C_WATER_CONTEXT+6]
#define C_WATER7					c[C_WATER_CONTEXT+7]
#define C_WATER_CONTEXT_END			C_WATER_CONTEXT+8

// -- MORPH --
#define	C_MORPH_CONTEXT				C_SHARED_ARRAY0
#define	C_MORPH_RATIO_0				c[C_MORPH_CONTEXT].xxx
#define	C_MORPH_RATIO_1				c[C_MORPH_CONTEXT].yyy
#define	C_MORPH_RATIO_2				c[C_MORPH_CONTEXT].zzz
#define	C_MORPH_RATIO_3				c[C_MORPH_CONTEXT].www
#define	C_MORPH_CONTEXT_END			C_MORPH_CONTEXT+1

// -- LONGUEUR D EXTRUSION DU VOLUME STENCIL
#define	C_VOLEXT_CONTEXT			C_MORPH_CONTEXT
#define	C_VOLEXT					c[C_VOLEXT_CONTEXT]
#define C_VOLEXT_LIGHTPOS			c[C_VOLEXT_CONTEXT+1].xyz
#define C_VOLEXT_LIGHTATT0			c[C_VOLEXT_CONTEXT+1].w
#define C_VOLEXT_LIGHTCOL			c[C_VOLEXT_CONTEXT+2].xyz
#define C_VOLEXT_LIGHTATT1			c[C_VOLEXT_CONTEXT+2].w
#define	C_VOLEXT_CONTEXT_END		  C_VOLEXT_CONTEXT+3

//	***********************************************************
// --  C_SHARED_ARRAY1 apres  C_SHARED_ARRAY0
#define	C_SHARED_ARRAY1				C_VOLEXT_CONTEXT_END
//	***********************************************************

#define	C_TARGET_MORPH_CONTEXT		C_SHARED_ARRAY1
#define	C_TARGET_MORPH0				c[C_TARGET_MORPH_CONTEXT]
#define	C_TARGET_MORPH1				c[C_TARGET_MORPH_CONTEXT+1]
#define	C_TARGET_MORPH2				c[C_TARGET_MORPH_CONTEXT+2]
#define	C_TARGET_MORPH3				c[C_TARGET_MORPH_CONTEXT+3]
#define	C_TARGET_MORPH4				c[C_TARGET_MORPH_CONTEXT+4]
#define	C_TARGET_MORPH5				c[C_TARGET_MORPH_CONTEXT+5]
#define	C_TARGET_MORPH6				c[C_TARGET_MORPH_CONTEXT+6]
#define	C_TARGET_MORPH7				c[C_TARGET_MORPH_CONTEXT+7]
#define	C_TARGET_MORPH8				c[C_TARGET_MORPH_CONTEXT+8]
#define	C_TARGET_MORPH_CONTEXT_END	  C_TARGET_MORPH_CONTEXT+9

// Particles Heat
#define	C_PARTICLE_CONTEXT			C_SHARED_ARRAY1
#define	PARTICLE_CAM_X				c[C_PARTICLE_CONTEXT].xyz
#define	PARTICLE_COS				c[C_PARTICLE_CONTEXT].w
#define	PARTICLE_CAM_Y				c[C_PARTICLE_CONTEXT+1].xyz
#define	PARTICLE_SIN				c[C_PARTICLE_CONTEXT+1].w
#define	PARTICLE_POS				c[C_PARTICLE_CONTEXT+2].xyz
#define	PARTICLE_COEFF				c[C_PARTICLE_CONTEXT+2].w
#define	PARTICLE_COLOR				c[C_PARTICLE_CONTEXT+3].xyz
#define	PARTICLE_SCALE_XY			c[C_PARTICLE_CONTEXT+4].xy
#define C_PARTICLE_CONTEXT_END		C_PARTICLE_CONTEXT+5

// Flare Constant
#define	C_FLARE_CONTEXT				C_SHARED_ARRAY1
#define	FLARE_VECXY					c[C_FLARE_CONTEXT].xy
#define	FLARE_ALPHA_MUL				c[C_FLARE_CONTEXT].z
#define	FLARE_SIZE_MUL				c[C_FLARE_CONTEXT].w
#define	C_FLARE_CONTEXT_END			C_FLARE_CONTEXT+1

// RotShape Constant
#define	C_RSHAPE_CONTEXT			C_SHARED_ARRAY1
#define	RSHAPE_PIVOT				c[C_RSHAPE_CONTEXT]
#define	RSHAPE_UPLEFT				c[C_RSHAPE_CONTEXT+1].xyz
#define	RSHAPE_SCALE				c[C_RSHAPE_CONTEXT+1].w
#define	RSHAPE_DWRIGHT				c[C_RSHAPE_CONTEXT+2].xyz
#define	C_RSHAPE_CONTEXT_END		C_RSHAPE_CONTEXT+3

// HField
#define	C_HFIELD_CONTEXT			C_SHARED_ARRAY1
#define	HFIELD_ORGX					c[C_HFIELD_CONTEXT].x
#define	HFIELD_ORGZ					c[C_HFIELD_CONTEXT].y
//#define	HFIELD_GSIZE			c[C_HFIELD_CONTEXT].zzzz
#define	HFIELD_TSIZE				c[C_HFIELD_CONTEXT].wwww
#define	C_HFIELD_CONTEXT_END		C_HFIELD_CONTEXT+1

#define	C_LAYER_CONTEXT				C_HFIELD_CONTEXT_END
#define	C_LAYER_START				c[C_LAYER_CONTEXT].x
#define	C_LAYER_GSIZE				c[C_LAYER_CONTEXT].y
#define	C_LAYER_ATTENUATION			c[C_LAYER_CONTEXT].z
#define	C_LAYER_NOISE				c[C_LAYER_CONTEXT].w
#define	C_LAYER_VFOG				c[C_LAYER_CONTEXT+1].x
#define	C_LAYER_CONTEXT_END			C_LAYER_CONTEXT+2

// Blend Matrix Cache	(Care BLEND MATRIX CACHE USE C_PATCH_CONSTANT ARRAY till END)
#define	C_CACHE_MATRIX_0			C_SHARED_ARRAY1
#define	C_CACHE_A_MATRIX_SIZE		6
#define	CACHE_MATRIX_SIZE			(C_END_CONSTANT-(C_CACHE_MATRIX_0))/C_CACHE_A_MATRIX_SIZE

const float4	c[C_END_CONSTANT]	: register(vs, c0);


//----------------------------------------
/*
	night flag is an on / off thing. originally used
	branching...

		value	= night ? true value : false value

	.. but concerned about logic branching performance.
	So, moved on to lerp'ing.. but lerp is a rather robust/
	expensive operation for just a simple logic switch.
	So, ended up making "select" statements below.
	They array the two values, then use the night
	value as an integer indexer to pick the correct one.
	Should be more GPU friendly solution?

	I designed the Select statement to follow same format
	as Lerp, where the "false" value is first, then "true"
	then decision factor...IE:

		value = lerp   ( value if lerp factor = 0, value if lerp factor = 1, lerp factor );

		value = select ( value if slct factor = 0, value if slct factor = 1, slct factor );

	But, in the case of select, we're indexing, and literally
	picking one or the other, not interpolating them.
	So, all I have to do is copy the lerp statements
	and rename them to select statements, and be good to go.

	Can also use this for other similar situations, like
	pixelmorph logic branch.
*/


// overloaded functions.. one for each float type

float	Select( float valueFalse, float valueTrue, int pick )
{
	float2x1	values;
				values[0]	= valueFalse;
				values[1]	= valueTrue;
	return		values[ pick ];			// if night (1), pick night value, else pick day value
}

float2	Select( float2 valueFalse, float2 valueTrue, int pick )
{
	float2x2	values;
				values[0]	= valueFalse;
				values[1]	= valueTrue;
	return		values[ pick ];			// if night (1), pick night value, else pick day value
}

float3	Select( float3 valueFalse, float3 valueTrue, int pick )
{
	float2x3	values;
				values[0]	= valueFalse;
				values[1]	= valueTrue;
	return		values[ pick ];			// if night (1), pick night value, else pick day value
}

float4	Select( float4 valueFalse, float4 valueTrue, int pick )
{
	float2x4	values;
				values[0]	= valueFalse;
				values[1]	= valueTrue;
	return		values[ pick ];			// if night (1), pick night value, else pick day value
}




//----------------------------------------

// using hlsl intrinsic sincos() function
// .x = sin, .y = cos
float2 Sincos ( float angle )
{
	float2	sc;
			sincos ( angle, sc.x, sc.y );
	return	sc;
}

//--------------------------------------------------

float4x4	GetMatrix( int CST)			{return	transpose( float4x4( c[CST], c[CST+1], c[CST+2], c[CST+3] ) );}

float4x3	GetMatrix4x3( int CST)		{return	GetMatrix( CST );}	// recast as float4x3

//--------------------------------------------------

float4	WrldVertex( float4 inPos)		{return mul(inPos, WORLD_MATRIX);}
float4	ViewVertex( float4 inPos)		{return mul(inPos, VIEW_MATRIX);}
float4	ProjVertex( float4 inPos)		{return mul(inPos, PROJ_MATRIX);}

//Coord World
float4 WorldToViewVertex( float4 wPos)	{return ViewVertex(wPos - VIEW_TRANS);}
float4 WorldToProjVertex( float4 wPos)	{return	ProjVertex(WorldToViewVertex(wPos));}


//Coord Local
float4 WorldVertex( float4 lPos)		{return	VIEW_TRANS + WrldVertex(lPos);}

float4 ProjectVertex( float4 lPos)
{
	float4	cPos	= WrldVertex(lPos);
	float4	vPos	= ViewVertex(cPos);
	float4	pPos	= ProjVertex(vPos);
	return	pPos;
}

float4 WorldAndViewAndProjectVertex( float4 lPos, out float4 wPos, out float4 vPos)
{
	float4	cPos	= WrldVertex(lPos);
			wPos	= cPos + VIEW_TRANS;
			vPos	= ViewVertex(cPos);
	float4	pPos	= ProjVertex(vPos);
	return	pPos;
}

// this one doesn't need vPos, so creates dummy one to call func above
float4 WorldAndProjectVertex( float4 lPos, out float4 wPos)
{
	float4	vPos;
	return	WorldAndViewAndProjectVertex(lPos, wPos, vPos);
}

//--------------------------------------------------


float4	TargetMorph(float4 position, float3 delta, out float pixelmorph)
{
	float4	newposition		= position;
	float	ratio			= C_TARGET_MORPH0.x;
			newposition.xyz	+= ratio * delta;
			pixelmorph		=  ratio * dot(delta, 1.0);
//			pixelmorph		= pixelmorph ? C_UNIT : C_NULL;
	bool	pixelmorphbool	= pixelmorph;	// keep this bool or saturate 0 to 1.. if just made int, vehicles will be missing parts
//			pixelmorph		= lerp( C_NULL, C_UNIT, pixelmorphbool );
			pixelmorph		= Select( C_NULL, C_UNIT, pixelmorphbool );
	return	newposition;
}


float GetWorldScale(void)
{
	float3	axeX;
			axeX.x 	= c[C_WORLD_CONTEXT].x;
			axeX.y	= c[C_WORLD_CONTEXT+1].x;
			axeX.z	= c[C_WORLD_CONTEXT+2].x;
//	return sqrt(dot(axeX.xyz,axeX.xyz));
	return 	length( axeX.xyz );
}


// Projections
float ProjectZ(float z)
{
	int	id	= C_PROJ_CONTEXT + 2;
	return z * c[id].z + c[id].w;
}


//Material
float2 MaterialMat(float2 t)
{
	float3	p	= float3(t.xy, C_UNIT);
	float2	v	= mul(p, float2x3( c[C_MTL_CONTEXT].xyz, c[C_MTL_CONTEXT+1].xyz ) );
//			v.x = dot(p,c[C_MTL_CONTEXT+0].xyz);
//			v.y = dot(p,c[C_MTL_CONTEXT+1].xyz);
	return	v;
}


float3 MapTankCoordinate ( float3 Position, float3 TankPosition, float TankScale )
{
	float3	p	= Position - TankPosition / TankScale;	// MAD
	float3	sp	= sign ( p );
			p	+= sp;
			p	= fmod ( p, C_TWO );
			p	-= sp;
			p	= p * TankScale + TankPosition;			// MAD
	return	p;
}


	// Texture Space
	// assumes inNormal is normalized already
	float3x3	LightMat( float4 inTangent, float3 inNormal)
	{
		float3	vNormal			= inNormal;
//				vNormal			= normalize(vNormal);
		float4	vTangent		= inTangent * C_TWO - C_UNIT;
//				vTangent.xyz	= normalize(vTangent.xyz);
		float3	vBinormal		= cross(vNormal, vTangent.xyz) * vTangent.w;	//vTangent.w = handedness
//				vBinormal		= normalize(vBinormal);
		return	float3x3(vTangent.xyz, vBinormal, vNormal);
	}



	float2 GetScaleRotation ( float2 scale, float2 sincos )
	{
		// using dot intrinsic instead
		float2	rotation	= sincos.xy;
				rotation.x	= -rotation.x;
		//		rotation.x	= ( scale.x * sincos.y - scale.y * sincos.x );
				rotation.x	= dot ( scale.xy, rotation.yx );
		//		rotation.y	= ( scale.x * sincos.x + scale.y * sincos.y );
				rotation.y	= dot ( scale.xy, sincos.xy );
		return	rotation;
	}


#endif