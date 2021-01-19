#ifndef	__WARP_H__
#define	__WARP_H__

#include "hlsl_math.h"
#include "_vertex_const.h"

struct VS_SkyOutput {
	float4	position	: POSITION;
	float4  inscatter	: TEXCOORD0;
};

struct VS_SkysunOutput {
	float4	position	: POSITION;
	float2	uv			: TEXCOORD0;
	float3	inscatter	: TEXCOORD1;
	float3	mooncolor	: TEXCOORD2;
	float3	suncolor	: TEXCOORD3;
};

struct VS_SkyGBuffer_Output {
	float4	position	: POSITION;
	float4	position2 	: TEXCOORD0;
};

#endif