#ifndef	_OMNI_H
#define	_OMNI_H
//----------------------------------
/*
	omni here refers to the deferred headlights lighting.
	the following files use these common routines:

		omni.phl
		ground.phl
		ground_rotshape.phl
		scat_mesh.phl

*/
//----------------------------------

float4		gViewportScaleOffset;

sampler2D	sLightBuffer;//	: register(s13);

//----------------------------------


float2 GetDeferredLightsPosition(float2 vPos)
{
	float2	pos		= vPos.xy + float2(0.1, 0.1);	// VPOS_TEXEL_CORRECT .. it was defaulted on, so baked it in
			pos		= pos * gViewportScaleOffset.xy + gViewportScaleOffset.zw;
	return	pos;
}


float3 GetDeferredLights(float2 vPos)
{
	float2	pos		= GetDeferredLightsPosition(vPos);
	float3	light	= tex2D( sLightBuffer, pos.xy ).xyz;
	return	light;
}

//----------------------------------

#endif	// _OMNI_H