#ifndef __TORNADO_H__
#define __TORNADO_H__

#include "__setup.h"

// billowy dust around tornado base
struct VS_TornadoSoftSmokeOut {
	float4	position	: POSITION0;
	float4	diffuse		: COLOR0;
	float2	uv			: TEXCOORD0;
	float3	sunlight	: TEXCOORD1;
	float3  inscatter	: TEXCOORD2;
	float4  uvscreen	: TEXCOORD3;
};

// ???? .. unknown, so disabled
struct VS_TornadoSmokeOut {
	float4	position	: POSITION0;
	float4	diffuse		: COLOR0;
	float2	uv			: TEXCOORD0;
	float4	sundir		: TEXCOORD1;
	float3	skydir		: TEXCOORD2;
	float4	sunlight	: TEXCOORD3;
	float4	skylight	: TEXCOORD4;
	float3	extinction	: TEXCOORD5;
	float3  inscatter	: TEXCOORD6;
	float3  normal		: TEXCOORD7;
};

// rotating cloud at top of tornado
struct VS_Tornado_Cloud_Output
{
	float4	position	: POSITION;
	float2	uv			: TEXCOORD0;
	float3	inscatter	: TEXCOORD1;
	float3	extinction	: TEXCOORD2;
	float4	eyevec		: TEXCOORD3;
};

// tornado funnel
struct VS_Tornado_Funnel_Output
{
	float4	position	: POSITION;
	float4	eyevec		: TEXCOORD0;

	float3x3 TBNmatrix	: TEXCOORD1;//, TEXCOORD2, TEXCOORD3

	float4	uv			: TEXCOORD4;
	float3	inscatter	: TEXCOORD5;
	float3	extinction	: TEXCOORD6;
	float	alpha		: TEXCOORD7;
};

#endif