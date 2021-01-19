#ifndef	_GROUND_ROTSHAPE_H
#define	_GROUND_ROTSHAPE_H

#include "__setup.h"

// AO spot under vehicle
#if ( GRASS_AMB_OCC == 1 )
	#define	bOmni	// can cause plants / trees to flicker when seen from a distance, so created a user flag out of it
#endif

#include "hlsl_math.h"

	struct VS_GroundRotShapeOutput {
		float4	position	: POSITION;
		float4	vcolor		: COLOR0;
		float3	sunlight	: TEXCOORD0;
		float3	skylight	: TEXCOORD1;
		float2	uv			: TEXCOORD2;
		float3	inscatter	: TEXCOORD3;
		float3	dots		: TEXCOORD4;	// .x = sky	// .y = sun	// .z = specular
		float3	spcref		: TEXCOORD5;
		float	occlusion	: TEXCOORD6;

		#ifdef	bShadowMap
		float3	shadowtcproj : TEXCOORD7;
		#endif

		#ifdef	bOmni
		float3	omni		: TEXCOORD8;
		#endif
	};

	struct VS_GroundRotShapeGBuffer_Output {
		float4	position	: POSITION;
		float4  uvax		: TEXCOORD0;	// u,v,alpha,undefined
		float3	normal		: TEXCOORD1;
		float4	position2	: TEXCOORD2;
	};

#endif
