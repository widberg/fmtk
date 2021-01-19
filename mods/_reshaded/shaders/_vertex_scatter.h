#ifndef HLSL_SCATTERING
#define HLSL_SCATTERING

	#include "__setup.h"
	#include "_vertex_const.h"
	#include "hlsl_const.h"

// inlining this on vertex shaders .vhl's that need it

	//-------------------------------------------------
	// Atmospheric scattering
	//-------------------------------------------------

	// (Asobo) calculates an integral multiplier of atmospheric density between two points
	float scatterintegral ( float lookAtLen, float iPos1y, float iPos2y )
	{
		float4	r11;
				r11.xz		= max(iPos1y, iPos2y);
				r11.yw		= min(iPos1y, iPos2y);

		float2	or11		= r11.xy;

				r11.xy		*= C_TERM_MULTIPLIERS.w;
				r11.zw		*= C_SCATTERCONSTANTS2.w;

		float4	r10			= min(r11, C_UNIT);

				r11.xy		= r11.xz - r11.yw;
				r11.zw		= r10.xz - r10.yw;
				r11			= max(r11, C_TERM_MULTIPLIERS.w);
				r11.xy		= 1.0 / r11.xy;
				r11.xy		*= r11.zw;

				r10.yw		+= r10.xz;
				r10.yw		= C_UNIT - r10.yw * C_HALF;
				r10.yw		*= r11.xy * C_SCATTERCONSTANTS2.yz;
				r10.y		+= r10.w;

		float	integral	= r10.y * lookAtLen;

		float2	or10		= clamp(or11.xy, C_SCATTERCONSTANTS1.y, C_TERM_MULTIPLIERS.z);

		float	totheight	= or11.x - or11.y;
				totheight	= max(totheight, C_INV16384);
		float	cloudheight	= or10.x - or10.y;

		float	itotheight	= 1.0 / totheight;
		float	prop		= lookAtLen * cloudheight * itotheight * C_HG.w;
				integral	+= prop;
		return	integral;
	}

//-------------------------------------------------

	void scatteringfinish(float lookaty, float pntdensity,float CosTheta,out float3 extinction,out float3 inscatter)
	{
		// s = length of lookAt vector
		// Theta - Scattering angles
		// Theta = Angle between LookAt and LightDirection
		
		float	ClampCosTheta	= min(C_NULL, CosTheta);

		// Phase1(theta) = 1+cos^2(theta)
		float	tweenf			= ClampCosTheta * ClampCosTheta;
		float	Phase1Theta		= C_UNIT + tweenf;

		// E - Total extinction
		// Extinction term E = -(beta_1+beta_2) * s * log_2 e
				extinction		= -C_BETA_1_PLUS_2.xyz * C_SCATTERCONSTANTS2.x * pntdensity;

		// E1 = e^(-(beta_1 + beta_2) * s)
				extinction		= pow(C_TWO, extinction);

		// Phase2(theta) = (1-g^2)/(1+g-2g*cos(theta))^(3/2)
		// theta is 180 - actual theta (this corrects for sign)
		// ScatteringHG = [1-g^2, 1+g, 2g]
		float	Phase2ThetaSun	= rsqrt(C_HG.y + C_HG.z * CosTheta);
				Phase2ThetaSun	= C_HG.x * pow(Phase2ThetaSun, 4);// * Phase2ThetaSun * Phase2ThetaSun * Phase2ThetaSun);

		// Inscattering = (Beta'_1 * Phase_1(theta) + Beta'_2 * Phase_2(theta)) * 
		//        [1-exp(-Beta_1*s).exp(-Beta_2*s)] / (Beta_1 + Beta_2)
				inscatter		= C_BETA_DASH_1.xyz * Phase1Theta
								+ C_BETA_DASH_2.xyz * Phase2ThetaSun;
				inscatter		*= C_ONE_OVER_BETA_1_PLUS_2.xyz;

		// (epaisseur = french for "thickness".. how thick the haze is?)
		float	epaisseur		= max(C_NULL, C_UNIT + lookaty);
				epaisseur		= pow(epaisseur, 3) * C_SCATTERCONSTANTS3.w;
				epaisseur		= C_UNIT - C_SCATTERCONSTANTS3.w + epaisseur;
				inscatter 		*= (C_UNIT - extinction) * epaisseur;

//		tweenf+=pntdensity*0.00001*(0.5+ICosTheta*0.5);
		float	ICosTheta		= -CosTheta;
				tweenf			= C_HALF + ICosTheta * C_HALF;
				tweenf			= tweenf * tweenf;
//				tweenf			= clamp( tweenf, C_NULL, C_UNIT );
				tweenf			= saturate( tweenf );

		// Apply multipliers
		float	itweenf			= C_UNIT - tweenf;
		float3	suncolor		= DLIGHT_COLOR.xyz * tweenf
		#if ( SUNLIGHT_AS_SHADOWLIGHT == 1 ) || ( BLUER_SKIES == 0 )
								+ C_UNIT * itweenf;			// original
		#else
								+ DSKY_COLOR.xyz * itweenf;
		#endif
				inscatter		*= suncolor * C_TERM_MULTIPLIERS.y;

		float	lenextinction	= sqrt(length(extinction.xyz));
				extinction.xyz	*= lenextinction;
	}

//-------------------------------------------------

	void scatteringbase(float4 iPos, out float3 extinction, out float3 inscatter)
	{
	#if ( BLUER_SKIES == 1 )
		float	muldensity	= C_HALF;	// makes Dark Dust more bearable, since DSKY_COLOR increases haze
	#else
		float	muldensity	= C_UNIT;
	#endif

		float3	sunvec		= C_SCATTERCONSTANTS3.xyz;
		float3	lookAt 		= EYE_WORLD.xyz - iPos.xyz;
		float	lookAtLen 	= length(lookAt);
				lookAt 		/= lookAtLen;

		float	CosTheta 	= dot(lookAt.xyz, sunvec.xyz);
		float	pPosy 		= C_QUARTER + max(iPos.y, C_NULL);
		float	pntdensity	= scatterintegral(lookAtLen, pPosy, EYE_WORLD.y);
				pntdensity 	*= muldensity;

		scatteringfinish( min(lookAt.y, C_NULL), pntdensity, CosTheta, extinction, inscatter);
	}

//-------------------------------------------------

	void scattering(float4 iPos, out float3 extinction, out float3 inscatter, out float3 sunlight, out float3 skylight)
	{
		scatteringbase(iPos, extinction, inscatter);
				sunlight	= DLIGHT_COLOR.xyz * C_SCATTERCONSTANTS1.z;
		#if ( SUNLIGHT_AS_SHADOWLIGHT == 1 )
				skylight	= sunlight * float3 ( 0.45, 0.5, 0.55 );	// use sun color, but slightly gray
		#else
				skylight	= DSKY_COLOR.xyz;
		#endif
	}

#endif