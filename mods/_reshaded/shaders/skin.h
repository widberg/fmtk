#ifndef	__SKIN_H__
#define	__SKIN_H__

	#include "_vertex_const.h"
	#include "hlsl_math.h"
	#include "hlsl_shadow.h"

//--------------------------------------------

	struct VS_Input_1Blend
	{
		float4	position	: POSITION;
		float3	normal		: NORMAL;
		float2	uv			: TEXCOORD0;
		float4	tangent		: POSITION1;
		int		matid		: BLENDINDICES;
		float	blend		: POSITION2;
	};

	struct VS_Input_4Blend
	{
		float4	position	: POSITION;
		float3	normal		: NORMAL;
		float2	uv			: TEXCOORD0;
		float4	tangent		: POSITION1;
		int4	matids		: BLENDINDICES;
		float4	blends		: POSITION2;
	};

	struct VS_Output_Blended
	{
		float4	position;		//blended
		float3	normal;
		float3	tangent;		
		float3	utangent;		//unpacked tangent
		float	handedness;
	};

//--------------------------------------------

	float4	cache_mat0[3]		: register(vs, c125 );
	float3	cache_rot0[3]		: register(vs, c128 );

//--------------------------------------------

	float3	BlendPos ( float4 p, int matid, float blend )
	{
		float3x4	m;
					m[0]	= cache_mat0[matid	  ].xyzw;
					m[1]	= cache_mat0[matid + 1].xyzw;
					m[2]	= cache_mat0[matid + 2].xyzw;
		return	blend * mul ( p, transpose ( m ) );	// transpose from float3x4 to float4x3 xxx,yyy,zzz,www
	}


	float3	BlendDir ( float3 p, int matid, float blend )
	{
		float3x3	m;
					m[0]	= cache_rot0[matid	  ].xyz;
					m[1]	= cache_rot0[matid + 1].xyz;
					m[2]	= cache_rot0[matid + 2].xyz;
		return	blend * mul ( p, transpose ( m ) );	// transpose to xxx, yyy, zzz
	}


	float4 BlendMorph ( float4 position, float3 morph_d0, float3 morph_d1, float3 morph_d2, float3 morph_d3)
	{
		float4	pos		= position;
				pos.xyz	= pos.xyz
						+ morph_d0 * C_MORPH_RATIO_0
						+ morph_d1 * C_MORPH_RATIO_1
						+ morph_d2 * C_MORPH_RATIO_2
						+ morph_d3 * C_MORPH_RATIO_3;
		return	pos;
	}

//--------------------------------------------

	void Blend1( VS_Input_1Blend i, float4 position, out VS_Output_Blended o)
	{
		float4	tangent	= i.tangent * C_TWO - C_UNIT;
		float3	normal	= i.normal  * C_TWO - C_UNIT;
		o.utangent.xyz	= tangent.xyz;
		o.handedness	= tangent.w;
		o.position.w	= C_UNIT;

		o.normal		= BlendDir(normal,		i.matid, i.blend);
		o.tangent		= BlendDir(o.utangent,	i.matid, i.blend);
		o.position.xyz	= BlendPos(position,	i.matid, i.blend);

//		o.tangent		= normalize(o.tangent);
//		o.normal		= normalize(o.normal);
	}

	void Blend4( VS_Input_4Blend i, float4 position, out VS_Output_Blended o)
	{
		float4	tangent	= i.tangent * C_TWO - C_UNIT;
		float3	normal	= i.normal  * C_TWO - C_UNIT;
		o.utangent.xyz	= tangent.xyz;
		o.handedness	= tangent.w;
		o.position.w	= C_UNIT;

		o.position.xyz	= BlendPos(position,	i.matids.x, i.blends.x)
						+ BlendPos(position,	i.matids.y, i.blends.y)
						+ BlendPos(position,	i.matids.z, i.blends.z)
						+ BlendPos(position,	i.matids.w, i.blends.w);

		o.normal		= BlendDir(normal,		i.matids.x, i.blends.x)
						+ BlendDir(normal,		i.matids.y, i.blends.y)
						+ BlendDir(normal,		i.matids.z, i.blends.z)
						+ BlendDir(normal,		i.matids.w, i.blends.w);

		o.tangent		= BlendDir(o.utangent,	i.matids.x, i.blends.x)
						+ BlendDir(o.utangent,	i.matids.y, i.blends.y)
						+ BlendDir(o.utangent,	i.matids.z, i.blends.z)
						+ BlendDir(o.utangent,	i.matids.w, i.blends.w);

//		o.tangent		= normalize(o.tangent);
//		o.normal		= normalize(o.normal);
	}

#endif //__SKIN_H__