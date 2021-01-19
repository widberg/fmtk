#ifndef	__DECAL_H__
#define	__DECAL_H__

#define	bOmni			// force ambient occlusion on tire tracks under cars and trees
#define	bNormal			// force normal maps for bump-map detail

// this covers tire tracks & skid marks

struct VS_Out_Decal
{
	float4	position		: POSITION0;
	float4	diffuse			: COLOR0;
	float4	uvDeepTarmac	: TEXCOORD0;

	float3x3	TBNmatrix	: TEXCOORD1;//, TEXCOORD2, TEXCOORD3

	float3	extinction		: TEXCOORD4;
	float3	inscatter		: TEXCOORD5;
	float3	eyevec			: TEXCOORD6;

#ifdef	bShadowMap
	float3	shadowtcproj	: TEXCOORD7;
#endif
#ifdef	bOmni
	float3	omni			: TEXCOORD8;
#endif
};

#endif