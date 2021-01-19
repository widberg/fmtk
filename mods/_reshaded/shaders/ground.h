#ifndef __GROUND_HH__
#define __GROUND_HH__

#include "__setup.h"

	struct VS_GroundOutput {
		float4	position	: POSITION;
		float4	color		: COLOR0;
		float4	uv			: TEXCOORD0;	// .xy = diffuse uv	// .zw = bump map uv
		float4	eyevec		: TEXCOORD1;	// .xyz = eyevec	// .w = road decal fade / transition
		float4  variables	: TEXCOORD2;
		float4	sundir		: TEXCOORD3;	// .xyz	= sunvec	// .w = asphalt yes/no flag (0/1)
		float3	inscatter	: TEXCOORD4;
		float3	extinction	: TEXCOORD5;		
	#ifdef	bOmni
		float3	omni		: TEXCOORD6;
	#endif
	#ifdef	bShadowMap
		float3	shadowtcproj : TEXCOORD7;
	#endif
	};

	struct VS_GroundGBuffer_Output {
		float4		position	: POSITION;
		float4		variables	: TEXCOORD0;
		float3x3	iTBNmatrix	: TEXCOORD1; // TEXCOORD2, TEXCOORD3 ... inverse TBN light space for headlights
		float4		position2	: TEXCOORD7;
	};

//------------------------------

	void	SetSpecular(inout VS_GroundOutput o, float v)			{o.variables.z=v;}
	float	GetSpecular(in VS_GroundOutput i)						{return	i.variables.z;}
	void	SetOcclusion(inout VS_GroundOutput o, float v)			{o.variables.w=v;}
	float	GetOcclusion(in VS_GroundOutput i)						{return	i.variables.w;}
	void	SetBump1Lerp(inout VS_GroundOutput i, float v)			{i.variables.x=v;}
	float	GetBump1Lerp(in VS_GroundOutput i)						{return	i.variables.x;}
	void	SetBump2Lerp(inout VS_GroundOutput i, float v)			{i.variables.y=v;}
	float	GetBump2Lerp(in VS_GroundOutput i)						{return	i.variables.y;}

#endif //__GROUND_HH__