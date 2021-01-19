#ifndef	_SCAT_ROTSHAPE_H
#define	_SCAT_ROTSHAPE_H

#include "__setup.h"

	struct VS_ScatRotShapeOutput {
		float4	position	: POSITION;
#if ( DISTANCE_SMOKE == 1 )
		float2	uv			: TEXCOORD0;
		float4	sunlight	: TEXCOORD1;
		float3	inscatter	: TEXCOORD2;
#endif
	};

#endif