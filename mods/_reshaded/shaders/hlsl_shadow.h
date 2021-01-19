#ifndef __HLSL_SHADOW_H__
#define __HLSL_SHADOW_H__

#include "_vertex_const.h"
#include "hlsl_math.h"
#include "__setup.h"

//-------------------------------------------------
/*
	FUEL is doing Cascased Shadow Maps (CSM's) to create 4 shadow map "slices" at various LOD ranges
	ie: higher resolution maps closer to player for better shadow detail,
		lower the further out you go on the LOD spectrum to save processing power.

	CSM processing chain boils down to...

	1) determine what's  in shadow (shadow map casts ray out, and, if blocked, tells shadow map chunk to be shadowed)
	2) determine how close shadow areas are to player to determine proper LOD slice to use
	3) anti-alias chunky shadow result with bi or trilinear PCF (Percent Closer Filtering) to smooth out shadow edges
	4) attenuate / bias end results to blend the slices together and prevent banding between them

	reference ... https://docs.microsoft.com/en-us/windows/desktop/dxtecharts/cascaded-shadow-maps
	and code  ... https://github.com/walbourn/directx-sdk-samples/tree/master/CascadedShadowMaps11

	(ref cod seems to be for DX9, but ref code has been updated to DX 11 it seems.. so.. makes it hard)

	Unfortunately, there doesn't seem to be enough CSM code exposed to really do much in the shaders.
	The CSM info is pretty high-level to understand as it is (hitting my level of incompetence). But,
	what I do understand.. they seem to be doing a lot of the calcualtions in the CPU game engine,
	and only offloading the CSM index / mul's, PCF & Bias to shaders. Doesn't expose much code to dick with.
	I feel like something is mis-calculated coming in from the game engine, b/c the trilinear PCF
	antialiasing shouldn't be making such chunky shadows. Plus, the banding between shadows slices is
	still very distinct (you can see distinct shadows make a line as they convert to smudgy shadows in
	the distance).

	I've messed with some stuff, and tried to optimize what I could, but it is what it is, so to speak.

*/


//---------------------------------

#define		NUM_SHADOW_TEXTURES				4

//---------------------------------

struct VS_Output_ShadowMap 
{
	float4	position	: POSITION;
	float4	uvzw		: TEXCOORD0;
};

struct ShadowMatrix
{
	float4x4	Mat;
};

struct ShadowMatrices
{
	ShadowMatrix	Slices[NUM_SHADOW_TEXTURES];
	float4			TextureSize;//x = Texture SizeX ; y = 1 / Texture Size X ; z = 1 /Texture Size Y ; w = Texture Size Y
};

//---------------------------------

	ShadowMatrices	gShadowMatrices;
	float4			gShadowSliceDist;		// Matrix selection and Shadow Projection
	sampler2D		sShadowMap;

//---------------------------------

#if ( SHADOW_SOFTEN == 1 )
	#define SOFTEN_SHADOW(_shadow_)		max( _shadow_, 1 - SHADOW_AMOUNT )
#else
	#define	SOFTEN_SHADOW(_shadow_)		_shadow_
#endif


#ifdef	bShadowMap
	#define	SHADOW_VTX_BIL(_slice_)		ComputeVertexShadow2( i.shadowtcproj0, i.shadowtcproj1, i.shadowtcproj2, i.shadowtcproj3, _slice_ )
	#define	SHADOW_VTX_TRI(_slice_)		ComputeVertexShadow(  i.shadowtcproj0, i.shadowtcproj1, i.shadowtcproj2, i.shadowtcproj3, _slice_ )
	#define	SHADOW_PXL_TRI(_slice_)		ComputeShadow( i.shadowtcproj, _slice_ )
#else
	#define	SHADOW_VTX_BIL(_slice_)		1.0
	#define	SHADOW_VTX_TRI(_slice_)		1.0
	#define	SHADOW_PXL_TRI(_slice_)		1.0
#endif


//---------------------------------
//	Shadow Projection in vertex shader
//---------------------------------

// there's 4 shadow "slices", 0 to 3, basically LOD 4x4 matrices.
// this compares the shadow slice dist to VScompare,
// and for each slice the step VSCompare will set the mask.xyzw's to 0 or 1
// to "true/false". Then we dot,1 the thing to add up how many slices
// deep we're into the LOD ... and that gives us the index value
// to use to look up the proper 4x4 LOD shadow matrix

// the indices point to specific shadow map resolutions
// and the position of the object in the game world compared
// to where player is determines which indices it uses.
// So, objects farther away use higher indices which has
// chunkier shadow map and creates blurrier / smudgier shadows.
// objects closer to player (and their vehicle shadow) are
// indices 0 which has a higher-resolution shadow map and
// thus makes a more distinct shadow.

// unfortunately, the LOD's that control when the game switches
// to a different indices, and also the shadow map resolution levels,
// seem to be controlled by the game engine. So, we can't just
// force index 0 and magically get high-res shadows on everything.


//------------------------------------------
// shadow slice matrix determination
//------------------------------------------

	// position + slices done in vertex shader
	// the bNoSlice flags were only set for shaders that
	// originally used vertex shadow matrices. So, anything
	// I bolted vertex shadow matrices onto weren't doing it
	// properly. So, we can ad-hoc by simply making a matrix
	// out of the fromVS* results and pick the right slice.
	float3 ProjVertexShadowPS( float4 fromVS0,  float3 fromVS1, float3 fromVS2, float3 fromVS3, out int slice )
	{
		float4x3
				slices;
				slices[0]	= fromVS0.xyz;
				slices[1]	= fromVS1.xyz;
				slices[2]	= fromVS2.xyz;
				slices[3]	= fromVS3.xyz;
				slice		= dot ( step ( gShadowSliceDist.xyz, fromVS0.w ), 1 );
		float3	tcproj		= slices [ slice ];	// set our shadow slice from slice matrix using index
		return	tcproj;
	}


	// slices done in pixel shader
	float3 ProjShadowPS( float3 fromVS, out int slice )
	{
		float4	pos;
				pos.xyz		= fromVS.xyz;
				pos.w		= 1.0;

		// tcproj is float3, and we're mul'ing pos.xyzw, so we can recast
		// the float4x4 slice matrix to float3x4 to get 3 rows of .xyzw to mul by
		// skipping the 4th one that's not used
				slice		= dot ( step ( gShadowSliceDist.xyz, pos.z ), 1 );
		float4x3	slices	= gShadowMatrices.Slices[ slice ].Mat;	// convert 4x4 to 4x3
		float3	tcproj		= mul ( pos, slices );
		return	tcproj;
	}


//------------------------------------------
// shadow anti-aliasing routines
//------------------------------------------

	// Bilinear PCF ( Percentage Closest Filtering )
	// tcproj.xy = tex coord
	// tcproj.z = depth
	float	PixelShadow2 ( float3 tcproj, float bias )
	{
		// frac(x) = x - floor(x) ... but need to do this multi-step-wise
		// to get weight set right .. asobo just frac()'ed the weight directly
		float2	weight			= tcproj.xy * gShadowMatrices.TextureSize.xw;
		float4	offset;
				offset.xy		= floor ( weight.xy );
				weight			-= offset.xy;
				offset.zw		= offset.xy + 1.0;
//				offset			*= 1.0 / gShadowMatrices.TextureSize.xwxw;	// gives smoother results then gShadowMatrices.TextureSize.yzyz
				offset			/= gShadowMatrices.TextureSize.xwxw;
		float4	vals;			// get values, then condense them down to final shadow
				vals.x			= tex2D ( sShadowMap, offset.xy ).r;
				vals.y			= tex2D ( sShadowMap, offset.zy ).r;
				vals.z			= tex2D ( sShadowMap, offset.xw ).r;
				vals.w			= tex2D ( sShadowMap, offset.zw ).r;

//				vals.x			= tex2Dbias ( sShadowMap, float4( offset.xy, 0, bias ) ).r;
//				vals.y			= tex2Dbias ( sShadowMap, float4( offset.zy, 0, bias ) ).r;
//				vals.z			= tex2Dbias ( sShadowMap, float4( offset.xw, 0, bias ) ).r;
//				vals.w			= tex2Dbias ( sShadowMap, float4( offset.zw, 0, bias ) ).r;


				vals			= step ( vals, tcproj.z );
				vals.xy			= lerp ( vals.xz, vals.yw, weight.x );
		float	shadow			= lerp ( vals.x,  vals.y,  weight.y );
		return	shadow;
	}

	//--------------------------------

	// Asobo called it Quadrilinear, but it's actually 3x3 Trilinear (cubic?)
	// trilinear 3x3 matrix for positioning and blending
	float	PixelShadow4 ( float3 tcproj, float bias )
	{
		float2	weight	= frac ( tcproj.xy * gShadowMatrices.TextureSize.xw );
		float2	offset	= gShadowMatrices.TextureSize.yz;

		float2	N 		= tcproj.xy - offset;	// negative
		float2	P 		= tcproj.xy + offset;	// positive
		float2	C 		= tcproj.xy;			// center

		float2	uv[9]		=
				{			//-x						// 0					//+x
					float2 ( N.x, N.y ),		float2 ( C.x, N.y ),	float2 ( P.x, N.y ),	// -y	// bottom
					float2 ( N.x, C.y ),		float2 ( C.x, C.y ),	float2 ( P.x, C.y ),	//  0	// center
					float2 ( N.x, P.y ),		float2 ( C.x, P.y ),	float2 ( P.x, P.y )		// +y	// top
				};


				#define	shdtex(_uv_)	tex2D ( sShadowMap, _uv_ ).x
//				#define	shdtex(_uv_)	tex2Dbias ( sShadowMap, float4(_uv_, 0, bias ) ).x

		// the uv's pull an inverted matrix, where bottom textures are
		// stored in top of matrix, and vice-versa
		float3x3
				vals	=
				{
					shdtex(uv[0]),	shdtex(uv[1]),	shdtex(uv[2]),	// bottom
					shdtex(uv[3]),	shdtex(uv[4]),	shdtex(uv[5]),	// center
					shdtex(uv[6]),	shdtex(uv[7]),	shdtex(uv[8])	// top
				};

				//--------------------------------------------
				// step function determines if shadow sample is viable or not (0 or 1) by comparing to tcproj.z value.
				// nvidia cg manual says step only works on float to float4 vars, but surprisingly it handles float3x3 fine.

				vals		= step ( vals, tcproj.z );


				//--------------------------------------------
				// weight.x merges rows (x-axis)
				// 7 8 9		._11._12._13
				// 4 5 6		._21._22._23
				// 1 2 3		._31._32._33


//				vals._11_12	= lerp ( vals._11_12, vals._12_13, weight.x );
//				vals._21_22	= lerp ( vals._21_22, vals._22_23, weight.x );
//				vals._31_32	= lerp ( vals._31_32, vals._32_33, weight.x );


				// lerp can't take in matrices, but we can process
				// 2 lerp's of float3's instead of 3 lerps of float2's
				// to leverage per-component math more

				vals._11_12_21	= lerp ( vals._11_12_21, vals._12_13_22, weight.x );
				vals._22_31_32	= lerp ( vals._22_31_32, vals._23_32_33, weight.x );

				//--------------------------------------------
				// weight.y merges columns (y-axis)
				// 7 8			._11._12
				// 4 5			._21._22
				// 1 2			._31._32

		float4	lerped	= lerp ( vals._11_21_12_22, vals._21_31_22_32, weight.y );
		float	shadow	= dot ( lerped, 0.25 );		// sum it all and get avg ( dot can do both at once )
		return	shadow;
	}



//------------------------------------------
// Shadow Attenuation
//------------------------------------------
// attenuation determines when shadows stop showing in distance.
// don't disable it, or the land in the very distance will show
// a big black square of shadow moving over it (IE: where the shadow
// processing stops, so a chunky of shadow just rotates around un-
// lit.)


	float ShadowAttenuation( float shadow, float eyeDist, float slice )
	{
		float4x2
				dists;									//	slice	min	max
				dists[0]	= gShadowSliceDist.xy;		//	  0		 x	 y
				dists[1]	= gShadowSliceDist.yz;		//	  1		 y	 z
				dists[2]	= gShadowSliceDist.zw;		//	  2		 z	 w
				dists[3]	= gShadowSliceDist.zw;		//	  3		 z	 w

		int		slinceindex	= slice;					// int's are a bit better to use with indexing, so recasting slice as int
		float	minDist		= dists[ slinceindex ].x;
		float	maxDist		= dists[ slinceindex ].y;
				minDist		= minDist + (maxDist - minDist) * 0.5;
		float	attenuation = smoothstep( minDist, maxDist, eyeDist );
				shadow		= saturate( shadow + attenuation);
		return	shadow;
	}


	float ShadowAttenuation( float shadow, float eyeDist )
	{
	#if !defined(bNoSlice3)
		float	minDist		= gShadowSliceDist.z;
		float	maxDist		= gShadowSliceDist.w;
				minDist		= minDist + (maxDist - minDist) * 0.5;
		float	attenuation = smoothstep( minDist, maxDist, eyeDist );
				shadow		= saturate( shadow + attenuation);
	#endif
		return	shadow;
	}


	float ShadowAntiAliasing( float3 tcproj, float bias )
	{
	#if	( FORCE_BILINEAR_SHADOWS == 1 )
		float	shadow		= PixelShadow2( tcproj, bias );
	#else
		float	shadow		= PixelShadow4( tcproj, bias );
	#endif
		return	shadow;
	}



//------------------------------
// Shadow Processing
//------------------------------


	// vertex slices ... trilinear shadow
	float ComputeVertexShadow( float4 fromVS0, float3 fromVS1, float3 fromVS2, float3 fromVS3, out int slice )
	{
		float3	tcproj		= ProjVertexShadowPS( fromVS0, fromVS1, fromVS2, fromVS3, slice );
				tcproj.xy 	= saturate( tcproj.xy );
float	bias	= slice * DOWNSAMPLE_AMOUNT;
		float	shadow		= ShadowAntiAliasing( tcproj, bias );
//				shadow		= ShadowAttenuation( shadow, fromVS0.w, slice );
				shadow		= ShadowAttenuation( shadow, fromVS0.w );
		return	shadow;
	}


    // vertex slices ... bilinear shadow
	float ComputeVertexShadow2( float4 fromVS0, float3 fromVS1, float3 fromVS2, float3 fromVS3, out int slice )
	{
		float3	tcproj		= ProjVertexShadowPS( fromVS0, fromVS1, fromVS2, fromVS3, slice );
				tcproj.xy	= saturate( tcproj.xy );
float	bias	= slice * DOWNSAMPLE_AMOUNT;
		float	shadow		= PixelShadow2( tcproj, bias );	// always does bilinear
//				shadow		= ShadowAttenuation( shadow, fromVS0.w, slice );
				shadow		= ShadowAttenuation( shadow, fromVS0.w );
		return	shadow;
	}


	// pixel slices ... trilinear shadow
	float ComputeShadow( float3 fromVS, out int slice )
	{
		float3	tcproj		= ProjShadowPS( fromVS, slice );
				tcproj.xy	= saturate( tcproj.xy );
float	bias	= slice * DOWNSAMPLE_AMOUNT;
		float	shadow		= ShadowAntiAliasing( tcproj, bias );
//				shadow		= ShadowAttenuation( shadow, fromVS.z, slice );
				shadow		= ShadowAttenuation( shadow, fromVS.z );
		return	shadow;
	}



//------------------------------
// Z Bias
//------------------------------

    float4 ReAddShadowBiasNormal( float3 wnorm )
	{
// new .. normalize wnorm.. but not sure if it screws things up, so disabling for now
//		float	dotp	= dot(normalize(wnorm.xyz), DLIGHT_DIR.xyz);
		float	dotp	= dot(wnorm.xyz, DLIGHT_DIR.xyz);
//				dotp	= abs(dotp);	// squaring value below returns a positive regardless, so can skip abs()'ing it
				dotp	*= dotp;
		float	doto	= C_INV4096 + dotp;
		float	bias	= C_UNIT / doto * SHADOWMAP_VERTEX_ADDBIAS;
		return	float4(-bias * DLIGHT_DIR.xyz, 0);
	}

#endif //__HLSL_SHADOW_H__
