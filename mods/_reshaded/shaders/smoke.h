#ifndef	_SMOKE_H_
#define	_SMOKE_H_

struct VS_SmokeOut {
	float4	position	: POSITION0;
	float2	uv			: TEXCOORD0;
	float3	sunlight	: TEXCOORD1;
	float4	skylight	: TEXCOORD2;
	float3  inscatter	: TEXCOORD3;

	#ifdef	bShadowMap
	float4  shadowtcproj0 : TEXCOORD4;
	float3  shadowtcproj1 : TEXCOORD5;
	float3  shadowtcproj2 : TEXCOORD6;
	float3  shadowtcproj3 : TEXCOORD7;
	#endif
};

#endif