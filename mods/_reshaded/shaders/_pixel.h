//-----------------------------------------------------------------------
// Texture Routines
//-----------------------------------------------------------------------

/*--------------------------------------------------
	texture compression routines (FUEL's weird compression style)
	NOTE - DANGER!
	"Texture" is a reserved key word, but "Sample" is not.

	So, you can get away with calling vars "sample", but
	if you name any vars "texture" the shader compile
	will blow up. It's best not to use any keywords.

	Some of FUEL's default texture compression does it funky.
	They swizzle .wyz as .xyz, then do some funky calc's to
	get the z value from the .x & .y.

	Single-sourcing that method here cuts down on code
	bloat in those calling modules.
--------------------------------------------------

	Asobo has some normal map texture pulls calculate a .z
	value from the .xy values. I think this is a derivatives
	/ partial derivatives method. IE: after researching for
	a bit, I found out there's various methods to use 2 values
	for a normal to calcualte the third. The methods I found
	used abs()'ing and othe methods.

	Asobo seems to be doing this, but wrote out the euclidean
	distance formula of xy by hand to get z texture pull.

	z = sqrt ( 1 - ( x * x + y * y ) )

	This can get simplified to...

	z = sqrt ( 1 - dot( xy, xy) )

	... which is basically the built-in HLSL length function, but
	without the "1 - " part ( length = sqrt(dot(xy,xy)) )

	I dicked around a bit, and determined that I can just do the
	following to get the same result as asobo's code...

	z = length ( 1 - xy ) ... (which would be sqrt(dot(1-xy,1-xy))

	.. so it's technically the same if you do the algebra.


	But, HLSL also has a euclidean distance function that you provide
	two values to, and it will do the subtraction for you. So, you get
	something like this...

	z = distance ( xy, 1 )

	.. and the distance formula will pre-subtract the 1-xy and then
	use it as a pre-calc'ed variable in the rest of the function
	(which is basically just piping to length function probably).

	This is all well and good, but we're dealing with pixel shaders,
	not vertex shaders. In vertex shaders, you need euclid distance
	to get precise distances in 3D space. But, for pixel shaders,
	we're just doing texture look ups; in most cases they don't have
	to be as precise.

	So, we can cheat by using Manhattan Distance, which is ...

	z = abs ( x - y )

	... because what euclid distance is doing is just squaring
	things to get rid of negatives, and then sqrt'ing them to bring
	them back down to size. Manhattan Distance skips all that and
	just jumps right to a single subtraction and absolute value'ing
	of it to get rid of negative.

	Again, I dicked around with this, and found that it works great
	for most everything that needs this, except for scat_mesh.phl...
	which handles checkpoint flags. These flags have spotlights on
	them that have a haze around the light. If you use Manhattan
	Distance, then the haze over-saturates or something to look
	like a square pane of glass instead of a faded haze around the light.

	So, we have to use euclid distance for scat_mesh.phl stuff for
	precise detail, but can use cheaper Manhattan Distance for
	everything else.
*/

#include "__setup.h"


//----------------------------------------

// if downsampling textures,
// set bias & use tex2Dbias,
// otherwise use tex2D

	// we're gonna force the bias on some things even when downsample flag isn't set
	// so keep this outside the if/else so everything can generate a bias
	#define	SET_BIAS(_slice_)	slice * DOWNSAMPLE_AMOUNT


#if	( DOWNSAMPLE_TEXTURES == 1 )
	#define	tex2DB(_sample_, _uv_, _bias_)	tex2Dbias( _sample_, float4(_uv_, 0, _bias_) )
//	#define	SET_BIAS(_slice_)	slice * DOWNSAMPLE_AMOUNT
#else
	#define	tex2DB(_sample_, _uv_, _bias_)	tex2D( _sample_, _uv_)
//	#define	SET_BIAS(_slice_)	0
#endif

//----------------------------------------

#if ( CHEAPER_TEXTURE_CALCS == 1 )
	#define	texDist(_tex)		1.0 - abs ( _tex.x - _tex.y )	// manhattan distance .. less expensive, less accurate
#else
	#define	texDist(_tex)		length ( _tex.xy )				// euclidean distance .. more expensive, but more accurate
//	#define	texDist(_tex)		sqrt(1 - dot(_tex.xy, _tex.xy))
#endif


	float3 tex2Ddist ( sampler2D sample, float2 uv, float bias )
	{
		float3	tex;
				tex.xy	= tex2DB ( sample, uv, bias ).wy;
				tex.z	= texDist ( tex.xy );
		return	tex;	// return as-is.. we might lerp it or something else before " * 2 - 1 "'ing it
	}


//----------------------------------------


	float	texDepth2D ( sampler2D depthBuffer, float2 uv )
	{
		#ifdef INVERTED_ZBUFFER
			return 1.0f - tex2D( depthBuffer, uv ).x;
		#else
			return tex2D( depthBuffer, uv ).x;
		#endif
	}

//----------------------------------------

#ifdef	bOmni
	float GetOmniOcclusion ( float3 omniuv, float bias )
	{
	#if	( DOWNSAMPLE_TEXTURES == 1 )
		float	omniv	= tex1D ( sProjSpotMap0, float4( omniuv.x, 0, 0, bias ) ).x
						* tex1D ( sProjSpotMap0, float4( omniuv.y, 0, 0, bias ) ).x
						* tex1D ( sProjSpotMap0, float4( omniuv.z, 0, 0, bias ) ).x;
	#else
		float	omniv	= tex1D ( sProjSpotMap0, omniuv.x ).x
						* tex1D ( sProjSpotMap0, omniuv.y ).x
						* tex1D ( sProjSpotMap0, omniuv.z ).x;
	#endif
				omniv	= 1.0 - omniv;
		return	omniv;
	}
#endif

	// make it where AO only occurs in shadows
	float SaturateOcclusion ( float occlusion, float shadow )
	{
	#ifdef	bShadowMap
		// saturate returns value trimmed down between 0 to 1
		// if shadow is 1, then doesn't matter what occlusion
		// is, since saturate will make it 1 and neuter it in sunlight
		return	saturate ( shadow + occlusion );
	#else
		return	occlusion;
	#endif
	}


//----------------------------------------

	// Cubemap with axis correction
	float3 GetCubeMap( float3 spcref, float bias )
	{
				// (asobo) Correction of the graduation. Our Z is in the opposite
				//         direction compared to that of the GPU for cubemaps
				spcref.z	= -spcref.z;

	#if	( DOWNSAMPLE_TEXTURES == 1 )
		return	texCUBEbias( sGlobalCube, float4( spcref, bias ) ).rgb;
	#else
		return	texCUBE( sGlobalCube, spcref ).rgb;
	#endif
	}


//----------------------------------
// omni headlights
//----------------------------------

float4		gViewportScaleOffset;
sampler2D	sLightBuffer;//	: register(s13);

//----------------------------------


float2 GetDeferredLightsPosition(float2 vPos)
{
	float2	pos			= vPos.xy + 0.1;	// VPOS_TEXEL_CORRECT .. it was defaulted on, so baked it in
			pos			= pos * gViewportScaleOffset.xy + gViewportScaleOffset.zw;
	return	pos;
}


float3 GetDeferredLights(float2 vPos, float bias)
{
	float2	pos			= GetDeferredLightsPosition(vPos);
	float3	light		= tex2DB( sLightBuffer, pos.xy, bias ).xyz;
	return	light;
}