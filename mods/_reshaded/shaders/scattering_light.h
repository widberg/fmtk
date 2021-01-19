#ifndef __SCATTERING_LIGHT_H__
#define __SCATTERING_LIGHT_H__

#include "__setup.h"
#include "hlsl_const.h"

//------------------------------------------------------
/*
	Everything below assumes the following passed in
	has already been normalized...

	- Normal
	- EyeVec	(Eye Vector)
	- SunVec	(Sun Vector)
	- SkyVec	(Sky Vector)


	-----------------------------------------------------

	Asobo's lighting model is additive,

	EG:

	  suncolor (light color)  * sundot(direction) * shadow (blocks sunlight)
	+ lerp(ambient color, skycolor (shadow color), occlusion) * (sqrt( (skydot(direction) * 0.5 + 0.5) * 0.9 + 0.1))
	======================================
	Ending light value for pixel

	The way they do this, though, is awkward...

	They make a sky dot (direction of sky lighting), but come to find out it's
	actually the sun / light direction they're using ... so

	dot ( normal, sky_vec )

	... is the same as...

	dot ( normal, sun_vec )

	sky_vec is set to sun_vec in the game engine...so we're wasting
	more calculations to calculate the same thing for sky as we
	did for sun (dot to add volume and light direction to normals).

	Since the dot tracks light direction, it's 0 when facing opposite
	the light. So, that will pitch-black anything opposite the light
	source. To keep that from happening in shadows, they do that
	hacky stuff...

	sqrt( skydot * 0.5 + 0.5 ) * 0.9 + 0.1;

	Assuming skydot is 1...

	sqrt( 1 * 0.5 + 0.5 ) * 0.9 + 0.1 
	sqrt( 0.5 + 0.5 ) * 0.9 + 0.1 
	sqrt( 1 ) * 0.9 + 0.1 
	1 * 0.9 + 0.1 
	0.9 + 0.1
	1

	... you end up with 1, which means full shadow / sky color shows through.

	Assuming skydot is 0...(looking at opposite
	direction of light source, which is the sun in this case)

	sqrt( 0 * 0.5 + 0.5 ) * 0.9 + 0.1 
	sqrt( 0 + 0.5 ) * 0.9 + 0.1 
	sqrt( 0.5 ) * 0.9 + 0.1 
	0.71 * 0.9 + 0.1 
	0.64 + 0.1
	0.65

	So, we basically have a value that's between 1 & 0.65. Which
	means it's always adding in a base amount of light value in
	a hackish way. It's hackish, because doing it this way...

	1) flattens out the normals in shadows, to where it's hard
		to tell of some objects are even using them (eg: tires on
		some vehicles). This makes things in shadows look very
		flat, since they lack detail. Reason being, the skydot
		is being directly tampered with via static variable
		mul & add's (0.5's, 0.9, 0.1).

	2) when you multiply skydot by occlusion, it makes skydot 0 in
		AO areas, then you mul that by the skycolor.. it pitch-blacks
		that, too. So, AO areas are pitch black.

	They try to work around the 2nd issue by using occlusion
	as a lerp factor in some cases to do the following...

	skycolor = (ambcolor, skycolor, occlusion);

	.. where ambcolor = ambient light color for AO areas.

	This keeps the skydot between 1 & 0.65 since a 0 occlusion
	(to denote AO in that spot) is no longer zero'ing out the skydot.
	But, the ambcolor piped in from game engine is pitch black..
	so AO areas are still pitch black...

	*sigh*

	----------------------
	"FIX IT, FELIX!"
	----------------------

	Asobo's lighting model is not wrong .. in a sense.

	The basic additive lighting model in shaders is...

		light * shadow + ambient color

	Ambient color is the ambient world lighting that exists
	just from light bouncing and scattering all over the place,
	even in shadows.

	Shadow is a 0 to 1 value that blocks (0) or allows light through
	(1) to some extent, b/c it's a decimal made from anti-aliasing
	shadowmap chunks together to see how much of a pixel is shadowed.

	light can then be multiplied by a light direction dot which 
	gets normal / bump maps to add 3D detail to textures...

		light dot = dot ( light direction vector, normal vector )

	In the direction opposite the light, you end up with 0 as the
	dot, though. So, you have both shadow and light dot that can
	be 0, canceling out light. So, it's important to add in ambient
	lighting color.

	So you end up with this...

		light * light dot * shadow + ambient color

	--------------------------------------------------------

	The thing is, Asobo is using 3 parts... 

		sunlight	(direct lighting color)
		skylight 	(shadow lighting color)
		amblight	(ambient color)

	Their execution of it was awkward, though, b/c it flattened
	out the normals in shadows and kept AO areas pitch black.

	--------------------------------------------------------

	If you haven't figured out the solution by now from the
	basic lighting model explanation above... We can fix all
	this by just taking the components they give us and
	doing the following...

	  suncolor (light color)  * sundot(normal maps) * shadow (blocks sunlight)
	+ skycolor (shadow color) * sundot(normal maps) * saturate ( shadow + occlusion) (darkens AO, but only in shadows)
	+ 0.65 (base light amount to keep AO from being pitch black)
	======================================
	Ending light value for pixel

	We're moving the 0.65 base AO ambient light color out from
	the sky calculations, so the sky calculations can do their thing
	and the sky normals will have body in shadow.

	The sky calculations use the sun dot, b/c they're already doing
	that anyways. So, no sense in calculating a separate skydot when
	we can just re-use the sun dot for it. This makes both sky and
	sun normals have body.

	Since occlusion is now a blocking factor for skylight (shadows)
	we have to do the saturation thing to make sure occlusion
	only impacts shadows. Otherwise we get weird stuff like
	faded lighting around trees and under cars which looks
	unnatural.

	So...

	Shadows block sunlight to create shaded areas.
	Occlusion blocks skylight to create AO areas.
	Final AO color added in
	==================
	Final light

	--------------------------------------------

	We also have specular.

	Specular uses a "sunny trail" reflection calculation Asobo
	came up with that creates nice, long sunny trails when looking
	at the sun on the horizon. While the calculations are similar
	to the reflect() CG /HLSL function, if you do a reflect() instead
	you get a blinn specular circle around player's vehicle instead
	of the long sunny trail.

	So, have to keep their sunny trail calculation.

	However, they were calculating speculars as follows

		specular color * specular dot (normal map direction) * shadow * occlusion

	.. so specular is blocked by both shadow & occlusion.

	This is unrealistic, because even in shadows there is light
	bouncing and scattering around in real life, which includes
	bouncing off of things and shining into shadows. (which is
	why shadows are not pitch black). Light rays that bounce
	into shadows also bounce off objects in the shadows..
	including their specular surface. So, specular surfaces
	in shadows still shine.. but less so since they aren't
	reflecting direct light; they're reflecting scattered
	ambient light.

	So, in many cases I switched the formula to...

		specular color * specular dot (shine direction) * (shadow * 0.8 + 0.2) * (occlusion * 0.9 + 0.1)

	I say many cases, b/c for water I used a little tweaking.
	Water is a special case, b/c the way light acts in water
	is different then air. Light refracts in water, light can
	bounce off particles in water to make shadows lighter colored,
	etc. So, lighting on water had some tweaks.
*/

//------------------------------------------------------
// using struct to organize and pass around lights easier
//------------------------------------------------------

struct lights
{
	float3	sky;
	float3	sun;
	float3	amb;
	float3	spc;
};


// no specular
struct lights3
{
	float3	sky;
	float3	sun;
	float3	amb;
};

//------------------------------------------------------
// light dot processing
//------------------------------------------------------

	float3 lightdots(float3 normal, float3 sunvec, float3 spcref, float spcpow, float occlusion)
	{
		float3	dots;		// .x = sky (shadow) // .y = sun (direct) // .z = specular (shine)
//				dots.y		= dot( normal, sunvec ) * 0.5 + 0.5;	// "half lambert" to keep shadow-side from being pitch black ... shadows band when doing this, so don't do it
				dots.y		= dot( normal, sunvec );	// full lambert
				dots.z		= dot( spcref, sunvec );	// half lambert
				dots.yz		= saturate( dots.yz );
				dots.yz		= lit ( dots.y, dots.z, spcpow ).yz;	// process sun and specular dots
				dots.x		= dots.y;	// sky dot = sun dot in order to make normals look volumetric in shadows
				dots.z		*= ( 1 - gGlobalWetness );		// overcast skies tone down shine

			#if ( AO_SOFTEN == 1 )
		float	minamount	= 1 - AO_AMOUNT;	// minimum AO amount
		float	occ			= max( occlusion, minamount );
				dots.y		= max( dots.y,    minamount );	// since sun dot acts as AO on shadow-side, cap it, too
			#else
		float	occ			= occlusion;
			#endif

				dots.yz		*= occ;					// AO darkens shadows & speculars
		return	dots;
	}


//------------------------------------------------------
// light color setup
//------------------------------------------------------

#if ( SUNLIGHT_AS_SHADOWLIGHT == 1 )
	float3	SetSkyColor()	{return	gDSunColor.rgb * float3 ( 0.45, 0.5, 0.55 );}	// float3 makes more shadow-color like
	float3	SetSunColor()	{return	gDSunColor.rgb * ( 1 - gGlobalWetness );}		// lighten during rain to simulate overcast weather
#else
	float3	SetSkyColor()	{return	gDSkyColor.rgb;}
	float3	SetSunColor()	{return	gDSunColor.rgb;}
#endif
	float3	SetAmbColor()	{return	AMBIENT_OCCLUSION_COLOR;}


	// this one is for shaders with a float specular
	// which we'll mul by the specular dot.
	// so just set light.spc to sun color
	lights	SetLightColors( float shadow )
	{
		lights	light;
				light.sun	= SetSunColor();
				light.sky	= SetSkyColor();
				light.amb	= SetAmbColor();
//				light.spc	= lerp( light.sky, light.sun, shadow );
				light.spc	= light.sun;
		return	light;
	}

	// this one is for shaders with float3 specular
	// so set lights above, then mul specular by light.spc
	lights	SetLightColors(  float shadow, float3 specular )
	{
		lights	light		= SetLightColors( shadow );
				light.spc	*= specular;
		return	light;
	}



//------------------------------------------------------
// light color finalization
//------------------------------------------------------

	// no specular
	lights3 lightcols( float2 dots, lights3 light, float shadow )
	{
	#ifdef	bShadowMap
		float	shd			= SOFTEN_SHADOW(shadow);
				light.sun	= light.sun * shd + light.sky;		// sun color blocked by shadow
	#else
				light.sun	+= light.sky;
	#endif
				light.sun	*= dots.y;					// sun dot to add direction & volume to normals
				light.sun	+= light.amb;				// add ambient color so AO areas aren't pitch black
		return	light;
	}


	lights3 lightcols( float2 dots, lights3 light, float shadow, float3 omni )
	{
				light		= lightcols( dots, light, shadow );
				light.sun	+= omni;					// add headlights
		return	light;
	}



	//-----------------------------
	// with specular float3
	//-----------------------------

	lights lightcols( float3 dots, lights light, float shadow )
	{
	#ifdef	bShadowMap
		float	shd			= SOFTEN_SHADOW(shadow);
				light.sun	= light.sun * shd + light.sky;			// sun color blocked by shadow
				dots.z		*= max(shd, SPECULAR_SHADOW_AMOUNT);	// shadows still let some specular through
	#else
				light.sun	+= light.sky;
	#endif
				light.spc	*= dots.z;					// spc dot to add direction & volume to specular shine
				light.sun	*= dots.y;					// sun dot to add direction & volume to normals
				light.sun	+= light.amb;				// add ambient color so AO areas aren't pitch black
		return	light;
	}


	lights lightcols( float3 dots, lights light, float shadow, float3 omni )
	{
				light		= lightcols( dots, light, shadow );
				light.sun	+= omni;					// add headlights
		return	light;
	}


//------------------------------------------------------
/*
	rain sheen and enhance shadow have to get mixed into
	light, b/c if you mix them into the diffuse/color then
	they can make alpha transparent areas look like glass panes.
*/
//------------------------------------------------------

#if ( RAIN_SHEEN == 1 )
	lights	SetLightWetness( lights light, float3 reflection, float wetness, float night )
	{
//	if ( gGlobalWetness && !night )
//	{
				wetness			*= gGlobalWetness;// * 2;
				wetness			*= ( 1 - night  );				// don't do at night
//				wetness			= lerp( wetness, 0, night );	// turn off at night (otherwise too shiny)
//				wetness			= Select( wetness, 0, night );	// turn off at night (otherwise too shiny)
//				wetness			= night ? 0 : wetness;			// turn off at night (otherwise too shiny)
				wetness			= saturate( wetness );
				light.sun		= lerp(light.sun, reflection, wetness);
				light.sky		= lerp(light.sky, reflection, wetness * 0.75);	// do some in shadows, but reduced
//	}
		return	light;
	}
#endif

//------------------------------------------------------
/*
#if ( SHADOW_HIGHLIGHTS == 1 )
	lights	SetEnhancedShadow( lights light, float3 reflection, float3 inscatter, float night, float shadow )
	{
	#ifdef	bShadowMap
//	if ( !shadow && !night )
//	{
		float	highlight		= 0.05;
				highlight		*= ( 1 - saturate ( dot ( inscatter.rgb, 0.33 ) ) );	// as haze increases, decrease reflection
				highlight		*= ( 1 - shadow );				// only highlight in shadows
				highlight		*= ( 1 - night  );				// don't do at night
//				highlight		= lerp( highlight, 0, night );	// don't do at night
//				highlight		= Select( highlight, 0, night );	// don't do at night
//				highlight		= night ? 0 : highlight;		// don't do at night
				light.sky		= lerp( light.sky, reflection, highlight );
				light.amb		= lerp( light.amb, reflection, highlight );
//	}
	#endif
		return	light;
	}
#endif
*/

//-------------------------------
//	velour shadow highlighting to add depth to shadows
//-------------------------------

	// dot( eyevec, normal ) pre-calculated

	//	dotsvelour.x = dot( eyevec, normal )
	// 	dotsvelour.y = dot( sunvec, normal ) ... our usual dots.y

	// velour highlight creates "frosty" highlights to things
	// when camera is facing sun, but seems to cancel out normals
	// detail when camera's back is to the sun. So, we create both
	// the velour detail using the dotEyeNormal, and also a counter-
	// balance using exact same formula but with dotSunNormal (dots.y).
	// then we use dots.y as a lerp factor to determine which to use...
	// velour highlight when looking behind object towards sun, and
	// alternate highlight when back to sun and looking at object.

	// eg: when looking behind a vehicle towards the sun, the velour
	// would add a nice frosty highlight to tires. But, when you
	// rotated camera to look at vehicle with back to sun, the
	// vehicle would flatten out; the normal maps weren't highlighting.
	// so, we solve that by switching to a different highlight when
	// looking at the vehicle with our back to the sun.. a highlight
	// that shines from the sun behind us lighting it up naturally
	// like specular.. and then when we rotate to look at vehicle
	// with sun in our eyes, the alt shine stays put, but the frosty
	// highlights dynamically show up to act like light reflecting
	// off things into our eyes again.

	float3	ShadowHighlight( float3 lightcolor, float2 dotsvelour )
	{
	#if ( SHADOW_HIGHLIGHTS == 1 )
		float	Width			= 0.25;

		// velour highlight on object when looking
		// towards the sun
		float	dotEyeNormal	= dotsvelour.x;
		float	depth			= Width * dotEyeNormal;
		float	transparency	= 1 - dotEyeNormal;
		float	velourlight		= pow( transparency, 1.5 );
				velourlight		= dotsvelour.y + velourlight * 3;
				velourlight		*= SHADOW_HIGHLIGHTS_AMOUNT;	// adjusts how much to let through
//		float	value			= velourlight;
//				lightcolor		*= velourlight;


		// alt highlight on object when back is against the
		// sun and looking at object
/*
				dotEyeNormal	= -dotsvelour.y;
				depth			= Width * dotEyeNormal;
				transparency	= 1 - dotEyeNormal;
				velourlight		= pow( transparency, 1.5 );
				velourlight		= dotsvelour.y + velourlight * 3;
				velourlight		*= 0.3;	// adjusts how much to let through
*/
				// use velour highlight when facing sun, otherwise use alternate value
//				lightcolor		*= lerp( value, velourlight, dotsvelour.y ); // this one
//				lightcolor		*= lerp( velourlight, value, dotsvelour.y ); 

				// this works... velour highlights when camera facing sun,
				// else use lightcolor as-is
//				lightcolor		*= lerp( 1.0, velourlight, dotsvelour.y );

	// NOTE - I'm still undecided on this.. with softer shadows this isn't needed,
	// so disabled and just using velourlight as-is


				lightcolor		*= velourlight;

	#endif
		return	lightcolor;
	}

	// dot( eyevec, normal ) needs to be calculated first
	float3	ShadowHighlight( float3 lightcolor, float3 dots, float3 eyevec, float3 normal )
	{
	#if ( SHADOW_HIGHLIGHTS == 1 )
		float2	dotsvelour;
				dotsvelour.x	= dot( eyevec, normal );
				dotsvelour.y	= dots.y;
				lightcolor		= ShadowHighlight( lightcolor, dotsvelour );
	#endif
		return	lightcolor;
	}


#endif