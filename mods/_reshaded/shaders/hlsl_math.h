#ifndef	__HLSL_MATH_H__
#define	__HLSL_MATH_H__

#include "_vertex_const.h"
#include "hlsl_const.h"

//--------------------------------------------------

#ifdef bVLight

	void VertexLighting( float4 position, float3 normal, inout VS_Output o)
	{
				o.light.w	= MTL_EMISSIVE.w * GOURAUD_DFLT.w;

	#ifndef	bNoDLight
		float	lightdot	= dot( DLIGHT_DIR.xyz, normal.xyz );
				lightdot	= saturate( lightdot );
//				lightdot	= max( C_NULL, lightdot );
				o.light.rgb	= DLIGHT_COLOR.rgb * lightdot + DLIGHT_AMBIENT.rgb;
				o.light.rgb = ( o.light.xyz * MTL_DIFFUSE.rgb + MTL_EMISSIVE.rgb ) * GOURAUD_DFLT.rgb;
	#else
				o.light.rgb = C_NULL;
	#endif

				o.light.rgb	= min( C_UNIT, o.light.rgb );

//o.light.r *= 10;
	}


//-----------------------------------------------------------------------------
#else
//-----------------------------------------------------------------------------

	#ifdef	bNormal
	void OutTexSpace(out S_TexSpace o, float3x3 mLight )
	{
		o.axe	= mLight;
//		o.axe	= transpose(mLight);
	}
	#else
	void OutTexSpace(out S_TexSpace o, float3 normal )
	{
		#ifndef	bNormalLocal
		o.normal	= normalize(normal);
		#endif
	}
	#endif

#endif //bVLight

#endif //__HLSL_MATH_H__