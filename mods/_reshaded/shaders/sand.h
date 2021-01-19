#ifndef	__SAND_H__
#define	__SAND_H__

struct VS_SandStorm_Output
{
	float4	position		: POSITION;
	float3	eyevec			: TEXCOORD0;

	float3x3	TBNmatrix	: TEXCOORD1;//, TEXCOORD2, TEXCOORD3

	float2 uvDiffuse		: TEXCOORD4;
	float4 uvNormals		: TEXCOORD5;	// .xy = uv normal1	// .zw = uv normal2
	float3 inscatter		: TEXCOORD6;
	float3 extinction		: TEXCOORD7;
	float4 diffuse			: TEXCOORD8;
};

#endif