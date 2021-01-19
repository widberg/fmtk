#ifndef __GROUND_WATER__
#define __GROUND_WATER__

#define	bOmni
#define	VERTEX_SHADOW_SLICES

	struct VS_GroundWaterOutput_Z {
		float4	position	: POSITION;
		float4	diffuse		: COLOR0;
		float4	uv			: TEXCOORD0;
		float4	zBuffUV		: TEXCOORD1;	// needs to stay, game engine processes it or something, can't just use position

			#ifdef	bOmni
		float4	eyevec		: COLOR1;		// .xyz = eye vec		// .w = omni.x
		float4	inscatter	: COLOR2;		// .xyz = inscatter		// .w = omni.y
		float4	extinction	: COLOR3;		// .xyz = extinction	// .w = omni.z
			#else
		float3	eyevec		: COLOR1;
		float3	inscatter	: COLOR2;
		float3	extinction	: COLOR3;
			#endif

			#ifdef	bShadowMap
				#ifdef VERTEX_SHADOW_SLICES
		float4	shadowtcproj0 : TEXCOORD2;
		float3	shadowtcproj1 : TEXCOORD3;
		float3	shadowtcproj2 : TEXCOORD4;
		float3	shadowtcproj3 : TEXCOORD5;
				#else
		float3	shadowtcproj : TEXCOORD2;
				#endif
			#endif
	};

#endif