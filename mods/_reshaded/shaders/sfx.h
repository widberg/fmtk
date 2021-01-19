#ifndef	__SFX_H__
#define	__SFX_H__

#include "_vertex_const.h"
#include "hlsl_math.h"
#include "hlsl_shadow.h"
#include "__setup.h"

// vertex in
struct VS_Sfx
{
	float4	positionRotation			: POSITION;
	float4	uvscale						: TEXCOORD0;
	float4	rgba						: TEXCOORD1;
};

// vertex in
struct VS_SfxStatic
{
	float4	positionRotation			: POSITION;
	float4	uvscale						: TEXCOORD0;
	float4	rgba						: TEXCOORD1;
	float4	AlphaParameters				: TEXCOORD2;
	float4	ScaleParameters				: TEXCOORD3;
	float4	Life						: TEXCOORD4;
	float4	Speed						: TEXCOORD5;
	float4	Acceleration				: TEXCOORD6;
};

// screen splashes
struct VS_SfxWaterOut
{
	float4	position					: POSITION0;
	float2	uv							: TEXCOORD0;
	float4	diffuse						: TEXCOORD1;
	float2  uvscreen					: TEXCOORD2;
	float3	screenvector				: TEXCOORD3;
};

// wheel dust
struct VS_SfxSoftOut
{
	float4	position	: POSITION0;
	float2	uv			: TEXCOORD0;
	float4	sunlight	: TEXCOORD1;
	float4	skylight	: COLOR0;//TEXCOORD2;
	float3  inscatter	: TEXCOORD2;
	float4  uvscreen	: TEXCOORD3;

	#ifdef	bShadowMap
	float4  shadowtcproj0 : TEXCOORD4;
	float3  shadowtcproj1 : TEXCOORD5;
	float3  shadowtcproj2 : TEXCOORD6;
	float3  shadowtcproj3 : TEXCOORD7;
	#endif
};

// sparks
struct VS_SfxSoftNoLightingOut
{
	float4	position	: POSITION0;
	float2	uv			: TEXCOORD0;
	float4	diffuse		: TEXCOORD1;
	float4  uvscreen	: TEXCOORD2;
};

// lightning
struct VS_LightningOut
{
	float4	position	: POSITION0;
	float4	diffuse		: TEXCOORD2;
	float3	uvFog		: TEXCOORD0;
	float3	extinction	: COLOR1;
	float3	inscatter	: COLOR2;
};

// flinging mud & screen border dirt
struct VS_SfxOutDirt
{
	float4	position		: POSITION0;
	float4	diffuse			: TEXCOORD2;
	float2	uv				: TEXCOORD0;
};

// HUD lines & direction arrow
struct VS_SfxOutHUD
{
	float4	position		: POSITION0;
	float4	diffuse			: TEXCOORD2;
	float2	uv				: TEXCOORD0;
};

// ???
struct VS_SfxOut
{
	float4	position		: POSITION0;
	float4	diffuse			: TEXCOORD2;
	float2	uv				: TEXCOORD0;
};

// rain & snow vertex-to-pixel
struct VS_SfxOutRainSnow
{
	float4	position		: POSITION0;
	float4	diffuse			: TEXCOORD2;
	float2	uv				: TEXCOORD0;
	bool	rain			: TEXCOORD1;	// flag if dealing with rain or not, since can't overload ph_* func's
};

// clouds (sfx.vhl) & blowing leaves (wind.vhl)
struct VS_SfxScatteringOut {
	float4	position	: POSITION0;
	float2	uv			: TEXCOORD0;
	float4	sunlight	: TEXCOORD1;
	float3  inscatter	: TEXCOORD2;
};

#endif