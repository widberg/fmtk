#ifndef	_HLSL_CONST_H
#define	_HLSL_CONST_H

#include "__setup.h"

//---------------------------------------------

	struct DLight {
		float4		Dir			: register(c5);	// .xyz = position in sky // .w seems to be unused, so float3'ing this
		float4		Color		: register(c6);
		float4		Ambient		: register(c7);	// .xyz = ambient occlusion color // .w = DLightAmbientFactor = how much ambient occlusion is around trees due to canopy blocking more light near base
	};

//-----------------------------------------------------------------------------

	DLight		gDLight;
	float 		gGlobalWetness;	// flags when rain is falling ( 0 = no rain // >0 = raining )
	float4		gDSkyColor;
	float4		gDSunColor;
	float3		gDSkyAmbient;
	float3		gDSkyDir;
	float4		gBlueShift;		// color shifts at night	// .rgb = color // .a tracks day/night shift
	float4		gViewportScaleOffset2;
	float3x3	gViewRotMatrix;
	float4x4	gInvProjMatrix;

//-----------------------------------------------------------------------------

// gBlueShift.a tracks day/night shift, but in boolean fashion
// so we can setup a flag to denote when it's night or day.

#define	IS_NIGHT	(bool) step ( gBlueShift.a, 0.75 )

// now that we can flag when it's night & day, we can use it to
// create routines to darken / lighten things at night (eg: inscatter).
// NIGHT_MULTIPLIER is in __setup.h where user can edit it to darken
// or lighten the nights as they see fit.


// if night multiplier is 1, then we're not doing or gaining anything
// by processing it, so just return the value, otherwise do the processing
/*
#if	( NIGHT_MULTIPLIER == 1 )
	#define NIGHT_DARKEN(value)				value
	#define NIGHT_LIGHTEN(value)			value

	#define NIGHT_DARKEN2(value, night)		value
	#define NIGHT_LIGHTEN2(value, night)	value
#else
*/
/*
	#define NIGHT_DARKEN(value)			IS_NIGHT ? ( value * NIGHT_MULTIPLIER ) : value
	#define NIGHT_LIGHTEN(value)		IS_NIGHT ? ( value / NIGHT_MULTIPLIER ) : value

	#define NIGHT_DARKEN2(value, night)	   night ? ( value * NIGHT_MULTIPLIER ) : value
	#define NIGHT_LIGHTEN2(value, night)   night ? ( value / NIGHT_MULTIPLIER ) : value
*/
/*
	#define NIGHT_DARKEN(value)				lerp( value, value * NIGHT_MULTIPLIER, IS_NIGHT )
	#define NIGHT_LIGHTEN(value)			lerp( value, value / NIGHT_MULTIPLIER, IS_NIGHT )

	#define NIGHT_DARKEN2(value, night)		lerp( value, value * NIGHT_MULTIPLIER, night )
	#define NIGHT_LIGHTEN2(value, night)	lerp( value, value / NIGHT_MULTIPLIER, night )
*/


//-----------------------------------------------

//-----------------------------------------------



	#define NIGHT_DARKEN(value)				Select( value, value * NIGHT_MULTIPLIER, IS_NIGHT )
	#define NIGHT_LIGHTEN(value)			Select( value, value / NIGHT_MULTIPLIER, IS_NIGHT )

	#define NIGHT_DARKEN2(value, night)		Select( value, value * NIGHT_MULTIPLIER, night )
	#define NIGHT_LIGHTEN2(value, night)	Select( value, value / NIGHT_MULTIPLIER, night )



//#endif

//-----------------------------------------------------------------------------

//#ifdef	bOmni
	sampler1D	sProjSpotMap0;	//Omni Proj Map (ambient occlusion under cars)
//#endif

	sampler2D	sDiffuse : register(s0);
	sampler2D	sSpecular;
	sampler2D	sNormal;
	sampler2D	sNormalLocal;
	sampler2D	sZBuffer;
	sampler2D	sRenderTarget;
	samplerCUBE	sGlobalCube;

//-----------------------------------------------------------------------------

	struct S_TexSpace {
#ifdef	bNormal
		float3x3	axe		: TEXCOORD1;	// TEXCOORD2, TEXCOORD3
#else
	#ifndef	bNormalLocal
		float3	normal		: TEXCOORD1;	//Vertex Normal or Texture is Local NormalMap
	#endif
#endif
	};


#ifdef	bVLight	// Vertex lighting (ps1.2)

	struct VS_Output {
		float4	position	: POSITION;
		float2	diffuse		: TEXCOORD0;
		float4  light		: COLOR0;
	};

//-----------------------------------------------------------------------------
#else			// Pixel Lighting (ps2.0)

	struct VS_Output {
		float4	position	: POSITION;		//Vertex pos
		float2	diffuse		: TEXCOORD0;
		S_TexSpace			TexSpace;
	};
#endif



//-----------------------------------------------------------------------------

	struct PS_GBuffer {
		float4 Normal : COLOR0;
		float4 Depth : COLOR1;
	};

//-----------------------------------------------------------------------------


#endif