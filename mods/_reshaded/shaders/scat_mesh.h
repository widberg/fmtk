#ifndef	__SCAT_MESH_H__
#define	__SCAT_MESH_H__

#include "hlsl_math.h"
#include "_vertex_const.h"

//------------------------------------
//	Scat_mesh & Scat_skin share some things.
//	Chucking them here, then #include'ing them
//	in those respective files.
//------------------------------------

struct VS_ScatOutput {
	float4	position		: POSITION;	  // Vertex pos (clip space)
	float3	eyevec			: TEXCOORD0;  // Unnormalized eye vector (object space)

	S_TexSpace	TexSpace;	// TEXCOORD1, TEXCOORD2, TEXCOORD3

	float4	omni			: TEXCOORD4;  // .w = broken
	float4	inscatter		: TEXCOORD5;  // .w = diffuse UV.x
	float4	extinction		: TEXCOORD6;  // .w = diffuse UV.y
	float4	brokenUV		: TEXCOORD7;

		#ifdef	bShadowMap
	float3  shadowtcproj	: TEXCOORD8;
		#endif

		#ifdef	bDirt
	float2	dirtiness		: TEXCOORD9;	// .x = (dirtiness now) // .y = (dirtiness we'll transition to)
		#endif
};


// headlights light buffer
	struct VS_GBufferOutput {
		float4	position	: POSITION;		// Vertex pos
		float2	tDiffuse	: TEXCOORD0;	// Diffuse map
		S_TexSpace TexSpace : TEXCOORD1;	// TEXCOORD2, TEXCOORD2
		float4	position2	: TEXCOORD4;
	};

//-------------------------------------
// setters and getters for unusually packed stuff
//-------------------------------------

float2	GetDiffuseUV(VS_ScatOutput i)	
{
	float2	uv;
			uv.x	= i.inscatter.w;
			uv.y	= i.extinction.w;
	return	uv;
}
void	SetDiffuseUV(inout VS_ScatOutput o, float2 uv)	
{
	o.inscatter.w	= uv.x;
	o.extinction.w	= uv.y;
}

float	GetBroken(VS_ScatOutput i)					{return i.omni.w;}
void	SetBroken(inout VS_ScatOutput o, float v)	{o.omni.w = v;}

//-------------------------------------

float4 SetBrokenUV ( float2 uv )
{
	float4	halfuv;
			halfuv.xy	= uv.xy;
			halfuv.x	*= 0.5;
			halfuv.zw	= halfuv.xy;
			halfuv.z	+= 0.5;
	return	halfuv;
}

//-------------------------------------

#ifdef	bDirt
void SetDirtiness ( inout VS_ScatOutput o )
{
		// dirtiness is tracked inversely in C_USER0.x
		// so, as it goes up, dirtiness goes down.
		// so, have to 1 - C_USER0.x to see how much dirtiness goes up
		// also have to 1 - C_USER0.x - transition to get an amount of change over time
		// but, since dirt negates specular, also need to track C_USER0.x as-is to negate specular in pixel shader

	float	dirtTransition	= 0.15;
			o.dirtiness.x	= 1 - C_USER0.x;	// base dirtiness amount for transition
			o.dirtiness.y	= o.dirtiness.x + dirtTransition;	// base dirtiness amount for transition
}
#endif


#endif //__SCAT_MESH_H__