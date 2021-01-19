#ifndef	_SCAT_PART_H
#define	_SCAT_PART_H

	struct VS_ScatPart	{
		float4	position		: POSITION;
		float2	uv				: TEXCOORD0;
		float3	sunlight		: TEXCOORD1;
		float4	skylight		: TEXCOORD2;
		float3	inscatter		: TEXCOORD3;

		#ifdef	bShadowMap
		float4	shadowtcproj0 	: TEXCOORD4;	// .w = tcproj.z
		float3	shadowtcproj1 	: TEXCOORD5;
		float3	shadowtcproj2 	: TEXCOORD6;
		float3	shadowtcproj3 	: TEXCOORD7;
		#endif
	};

#endif