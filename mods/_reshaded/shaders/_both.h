#ifndef	__BOTH_H__
#define	__BOTH_H__

//----------------------------------------
/*
	Things common between vertex & pixel shaders,
	EG: like S_TexSpace that's used on
	go-between structs.
*/
//----------------------------------------

	struct S_TexSpace {
#ifdef	bNormal
		float3x3	axe		: TEXCOORD1;	// TEXCOORD2, TEXCOORD3
#else
	#ifndef	bNormalLocal
		float3	normal		: TEXCOORD1;	//Vertex Normal or Texture is Local NormalMap
	#endif
#endif
	};


#endif