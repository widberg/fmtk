#pragma once

#ifndef _NOESIS_PLUGIN_SHARE_H
#define _NOESIS_PLUGIN_SHARE_H

//general rule is, don't change anything in this file.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <new>

#pragma pack(push, 1)

//styles/conventions in this API are based on the shitty roots of this codebase from ~15 years ago.
//i hate them all now, and sometimes you'll notice I decide to just say fuck it and not adhere
//to convention. this has led to a horrible c/c++ style hybrid.
#include "pluginbasetypes.h"

//if you're trying to compile a noesis plugin on a non-ms compiler, well... i'm sorry your life has come to this
#ifdef _NOE64
typedef __int64 TResvInt;
#else
typedef int TResvInt;
#endif

#ifndef NoeCtAssert
	#define NoeCtAssert(ctExpression) typedef int compileTimeAssert[(ctExpression) ? 1 : -1 ];
#endif

#ifndef NoeAssert
	#define NoeAssert assert
#endif

#ifndef NoeInline
	#define NoeInline inline
#endif

#define NOESIS_PLUGIN_VERSION		3

#define NOESIS_PLUGINAPI_VERSION	75 //make your plugin require this version if you use new api functions.
//Noesis 4.406 - 75
//Noesis 4.403 - 74
//Noesis 4.2 - 73
//Noesis 4.0974 - 72
//Noesis 4.0969 - 71
//Noesis 4.0968 - 70
//Noesis 4.0965 - 69
//Noesis 4.096 - 68
//Noesis 4.092 - 67
//Noesis 4.0896 - 66
//Noesis 4.0875 - 65
//Noesis 4.0869 - 64
//Noesis 4.0843 - 63
//Noesis 4.084 - 62
//Noesis 4.0836 - 61
//Noesis 4.0828 - 60
//Noesis 4.0824 - 59
//Noesis 4.081 - 58
//Noesis 4.0799 - 57
//Noesis 4.079 - 56
//Noesis 4.0783 - 55
//Noesis 4.0781 - 54
//Noesis 4.074 - 53
//Noesis 4.066 - 52
//Noesis 4.061 - 51
//Noesis 4.06 - 50
//Noesis 4.04 - 49
//Noesis 4.02 - 48
//Noesis 4.0 - 47
//Noesis 3.997 - 46
//Noesis 3.996 - 45
//Noesis 3.994 - 44
//Noesis 3.991 - 43
//Noesis 3.99 - 42
//Noesis 3.98 - 41
//Noesis 3.971 - 40
//Noesis 3.97 - 39
//Noesis 3.93 - 38
//Noesis 3.9 - 37
//Noesis 3.89 - 36
//Noesis 3.862 - 35
//Noesis 3.856 - 34
//Noesis 3.852 - 33
//Noesis 3.848 - 32
//Noesis 3.84 - 31
//Noesis 3.7 - 30
//Noesis 3.69 - 29
//Noesis 3.66 - 28
//Noesis 3.56 - 27
//Noesis 3.54 - 26
//Noesis 3.5 - 25
//Noesis 3.46 - 24
//Noesis 3.31 - 23
//Noesis 3.28 - 22
//Noesis 3.27 - 21
//Noesis 3.26 - 20
//Noesis 3.22 - 19
//Noesis 3.17 - 18
//Noesis 3.15 - 17
//Noesis 3.1 - 16
//Noesis 3.0 - 15
//Noesis 2.99 - 14
//Noesis 2.981 - 13
//Noesis 2.97 - 12
//Noesis 2.95 - 11
//Noesis 2.9 - 10
//Lower than Noesis 2.9 - 9

#define MAX_NOESIS_PATH				4096

typedef unsigned int noesisIntPtr_t;

typedef struct rexp_s rexp_t;

#define NOESISBUTTON_LBUTTON		(1<<0)
#define NOESISBUTTON_RBUTTON		(1<<1)
#define NOESISBUTTON_SHIFT			(1<<2)
#define NOESISBUTTON_CONTROL		(1<<3)
#define NOESISBUTTON_MBUTTON		(1<<4)
#define NOESISBUTTON_LCLICK			(1<<5)
#define NOESISBUTTON_RCLICK			(1<<6)
#define NOESISBUTTON_MOUSEEXIT		(1<<7)
#define NOESISBUTTON_NOCURSOR		(1<<8)
#define NOESISBUTTON_ALT			(1<<9)

#define NTOOLFLAG_CONTEXTITEM		(1<<0)		//appears on the right-click file list context menu instead of the tools menu
#define NTOOLFLAG_USERBITS			(15<<28)	//you can have bits 28-31 to do whatever the hell you want

#define NFORMATFLAG_ARCREAD			(1<<0)		//has an archive read handler
#define NFORMATFLAG_IMGREAD			(1<<1)		//has an image read handler
#define NFORMATFLAG_IMGWRITE		(1<<2)		//has an image write handler
#define NFORMATFLAG_MODELREAD		(1<<3)		//has a model/anim read handler
#define NFORMATFLAG_MODELWRITE		(1<<4)		//has a model write handler
#define NFORMATFLAG_ANIMWRITE		(1<<5)		//has an animation write handler

typedef enum
{
	NOESISTEX_UNKNOWN = 0,
	NOESISTEX_RGBA32,
	NOESISTEX_RGB24,
	NOESISTEX_DXT1,
	NOESISTEX_DXT3,
	NOESISTEX_DXT5,
	NUM_NOESIS_TEXTYPES
} noesisTexType_e;

typedef struct jobHandle_s
{
	int				index;
	int				count;
} jobHandle_t;

typedef struct modelMatrix_s
{
	float				x1[3];
	float				x2[3];
	float				x3[3];
	float				o[3];
} modelMatrix_t;

typedef struct modelMatrixD_s
{
	double				x1[3];
	double				x2[3];
	double				x3[3];
	double				o[3];
} modelMatrixD_t;

typedef struct fourxMatrix_s
{
	float				c1[4];
	float				c2[4];
	float				c3[4];
	float				c4[4];
} fourxMatrix_t;

extern modelMatrix_t g_identityMatrix;
extern fourxMatrix_t g_identityMatrix4x4;

extern double g_dbPI;
extern float g_flPI;
extern float g_flDegToRad;
extern float g_flRadToDeg;

typedef struct modelVert_s
{
	float				x;
	float				y;
	float				z;
} modelVert_t;

typedef struct modelTan4_s
{
	float				v[4];
} modelTan4_t;

typedef struct modelTangent_s
{
	float				normal[3];
	float				tangent[3];
	float				bitangent[3];
} modelTangent_t;

typedef struct modelVertWInfo_s
{
	int					weightIndex;
	int					numWeights;
} modelVertWInfo_t;

typedef struct modelTexCoord_s
{
	float				u;
	float				v;
} modelTexCoord_t;

typedef struct modelTriFace_s
{
	WORD				a;
	WORD				b;
	WORD				c;
	WORD				flag;
} modelTriFace_t;

typedef struct modelTriNeighbors_s
{
	int					a;
	int					b;
	int					c;
} modelTriNeighbors_t;

typedef struct modelLongTri_s
{
	int					idx[3];
} modelLongTri_t;

typedef struct modelRGBA_s
{
	float				rgba[4];
} modelRGBA_t;

typedef struct newVertWeight_s
{
	int					boneIndex;
	float				weightFactor;
} newVertWeight_t;

#define BONEFLAG_ORTHOLERP		(1<<0)
#define BONEFLAG_DIRECTLERP		(1<<1)
#define BONEFLAG_NOLERP			(1<<2)
#define BONEFLAG_DECOMPLERP		(1<<3)

//this shit is pretty messy. sorry about that.
//you don't need to modify anything in modelBoneExData_t except for parent, as far as your plugin is concerned.
typedef struct modelBone_s modelBone_t;
typedef struct modelBoneExData_s
{
	modelBone_t			*parent;

	//ignore these fields, they're used internally be pure-legacy code
	modelBone_t			*next;
	modelMatrix_t		transMat;
	modelMatrix_t		relativeMat;
	modelMatrix_t		baseMat;
	modelMatrix_t		transWithLenMat;
	short				lastFrameAngles[3];

	//may eventually add extended data pointers, long name strings, etc.
	void				*resv[4];
} modelBoneExData_t;

#define BONE_STRUCT_VER				0x246A0E06
//for feeding data back, only this structure and eData.parent matter.
//IT IS VERY IMPORTANT THAT EACH BONE'S 'name' CONTAINS A UNIQUE STRING.
#define MAX_BONE_NAME_LEN			128 //pushed from 32 to 128 for Noesis 3.7
typedef struct modelBone_s
{
	//ver is automatically set to the current BONE_STRUCT_VER when you call Noesis_AllocBones.
	//this is how noesis deals with changes to this bone structure while maintaining compatibility with old plugins.
	int					ver;

	//index will be filled in for you noesis-side.
	int					index;
	char				name[MAX_BONE_NAME_LEN];
	//plugins should just leave parentName blank and set eData.parent instead. parentName is set and used internally by pure-legacy code.
	char				parentName[MAX_BONE_NAME_LEN];
	//you need to fill in mat yourself when passing bone data to noesis - it's the modelspace (not parent-relative) bone matrix.
	modelMatrix_t		mat;

	modelBoneExData_t	eData;

	int					flags;
	int					userIndex;
	TResvInt			resv[6];
} modelBone_t;

//deprecated bone structures
typedef struct modelBone369_s modelBone369_t;
typedef struct modelBoneExData369_s
{
	modelBone369_t			*parent;

	//ignore these fields, they're used internally be pure-legacy code
	modelBone369_t			*next;
	modelMatrix_t		transMat;
	modelMatrix_t		relativeMat;
	modelMatrix_t		baseMat;
	modelMatrix_t		transWithLenMat;
	short				lastFrameAngles[3];

	//may eventually add extended data pointers, long name strings, etc.
	void				*resv[4];
} modelBoneExData369_t;
typedef struct modelBone369_s
{
	int					index;
	char				name[32];
	
	modelMatrix_t		mat;

	char				parentName[32];

	modelBoneExData369_t	eData;
} modelBone369_t;

typedef struct modelVMorphFr_s
{
	modelVert_t			*pos;
	modelVert_t			*nrm;
} modelVMorphFr_t;

struct SExtraPerMorphData
{
	char				*mpName;

	void				*pResv[16];
};

#define MAX_MORPH_NAME_SIZE 256

struct SMorphGroup
{
	SMorphGroup()
		: mStartFrame(0)
		, mEndFrame(0)
		, mMeshIndex(-1)
	{
		mName[0] = 0;
		mMeshName[0] = 0;
		memset(mResv, 0, sizeof(mResv));
	}

	//name of the morph group
	char mName[MAX_MORPH_NAME_SIZE];

	//name of the mesh which this morph references
	char mMeshName[MAX_MORPH_NAME_SIZE];

	//NOTE - where morphs are considered, "frame 0" is actually the mesh base geometry.
	//therefore, morphFrames[0]..morphFrames[numMorphFrames - 1] is represented as frame 1..numMorphFrames.
	int mStartFrame;
	//mEndFrame is inclusive, so if there are 86 frames (including the base geo as frame 0), mEndFrame should be no greater than 85.
	int mEndFrame;

	int mMeshIndex; //set internally, do not modify

	TResvInt mResv[16];
};

struct SMorphGroupInfo
{
	SMorphGroupInfo()
		: mpGroups(NULL)
		, mGroupCount(0)
	{
		memset(mResv, 0, sizeof(mResv));
	}

	SMorphGroup *mpGroups;
	int mGroupCount;

	TResvInt mResv[16];
};

typedef struct modelUserStream_s
{
	const char					*name;
	void						*data;
	int							dataSize;
	int							dataElemSize;
	int							flags;

	TResvInt					resv[8];
} modelUserStream_t;

struct SSoftTagEx
{
	int			mResv[32];
};

struct SSoftTagInfoEntry
{
	int			mId;
	double		mSize;
	TResvInt	mResv[4];
};
#define SOFT_TAG_ID_GENERAL -1

struct SSoftTagTransform
{
	double		mRot[9];
	double		mTrn[3];
	double		mError;
	int			mId;

	double		mPixelCenter[2];
	double		mPixelPoints[4][2];

	SSoftTagEx	*mpEx;
};

class RichComplex;

typedef struct noesisTex_s noesisTex_t;

//this exposes a chunk of my horrifying standard-type math library. i don't guarantee that any of these functions work, but most of them probably do.
typedef struct mathImpFn_s
{
	void (*Math_CalcPlaneEq)(float *x, float *y, float *z, float *planeEq);
	float (*Math_Max2)(float a, float b);
	float (*Math_Max3)(float a, float b, float c);
	float (*Math_Min2)(float a, float b);
	float (*Math_Min3)(float a, float b, float c);
	void (*Math_TransformPointByMatrix)(modelMatrix_t *matrix, float *in, float *out);
	void (*Math_TransformPointByMatrixNoTrans)(modelMatrix_t *matrix, float *in, float *out);
	void (*Math_MatrixInverse)(modelMatrix_t *in, modelMatrix_t *out);
	void (*Math_TransformPointByMatrix4x4)(float *matrix, float *in, float *out);
	void (*Math_MatrixInverse4x4)(float *mat, float *dst);
	float (*Math_MatRotDist)(modelMatrix_t *mat);
	float (*Math_VecRotDist)(float *v1, float *v2);
	void (*Math_MatToAngles)(float *angles, const modelMatrix_t *mat);
	void (*Math_AnglesToMat)(const float *incAngles, modelMatrix_t *mat);
	void (*Math_AngleVectors)(float *angles, float *forward, float *right, float *up);
	bool (*Math_MatrixIsSkewed)(modelMatrix_t *mat);
	void (*Math_OrthogonalizeMatrix)(modelMatrix_t *mat, bool keepScale, bool keepFlip, bool straightCross);
	void (*Math_LerpMatricesQ)(modelMatrix_t &preMat, modelMatrix_t &postMat, float lerpFrac, modelMatrix_t &outMat,
							   bool nonUniform);
	void (*Math_LerpMatrices)(modelMatrix_t &preMat, modelMatrix_t &postMat, float lerpFrac, modelMatrix_t &outMat,
							   bool nonUniform, bool orthogonalize);
	void (*Math_RotationMatrix)(float phi, int axis, modelMatrix_t *mat);
	void (*Math_MatrixMultiply)(modelMatrix_t *in, modelMatrix_t *in2, modelMatrix_t *out);
	void (*Math_4x4ToModelMat)(fourxMatrix_t *matFour, modelMatrix_t *mat);
	void (*Math_ModelMatToGL)(modelMatrix_t *mat, float *out);
	void (*Math_ModelMatFromGL)(modelMatrix_t *mat, float *in);
	void (*Math_TransposeMat)(modelMatrix_t *in, modelMatrix_t *out);
	void (*Math_TranslateMatrix)(modelMatrix_t *mat, float *v);
	void (*Math_RotateMatrix)(modelMatrix_t *mat, float ang, float x, float y, float z);
	void (*Math_MatrixMultiply4x4)(fourxMatrix_t *in, fourxMatrix_t *in2, fourxMatrix_t *out);
	void (*Math_RotationMatrix4x4)(float phi, int axis, fourxMatrix_t *mat);
	void (*Math_TranslateMatrix4x4)(fourxMatrix_t *mat, float *v);
	void (*Math_RotateMatrix4x4)(fourxMatrix_t *mat, float ang, float x, float y, float z);
	void (*Math_AxisForNormal)(float *normal, float *fwd, float *right, float *up);
	void (*Math_ExpandBounds)(float *baseMins, float *baseMaxs, float *mins, float *maxs);
	float (*Math_MaxExtent)(float *mins, float *maxs);
	bool (*Math_PointInOnBox)(float *point, float *boxMins, float *boxMaxs);
	bool (*Math_BoxesOverlap)(float *mins1, float *maxs1, float *mins2, float *maxs2);
	void (*Math_PlaneFromPoints)(float *p0, float *p1, float *p2,
					 float *plane);
	void (*Math_ConfinePointToBox)(float *mins, float *maxs, float *ptIn, float *ptOut);
	void (*Math_BuildBoxPlanes2D)(float *mins, float *maxs, float *planes);
	void (*Math_VecCopy)(float *a, float *out);
	void (*Math_VecSub)(float *a, float *b, float *out);
	void (*Math_VecSub2)(float *a, float *b, float *out);
	void (*Math_VecAdd)(float *a, float *b, float *out);
	void (*Math_VecScale)(float *a, float scale);
	void (*Math_VecScaleVec)(float *a, float *scale);
	float (*Math_VecNorm)(float *v);
	float (*Math_DotProduct)(const float *v1, const float *v2);
	void (*Math_CrossProduct)(const float *v1, const float *v2, float *cross);
	float (*Math_VecLen)(float *a);
	float (*Math_VecLenSq)(float *a);
	float (*Math_VecLen2)(float *a);
	void (*Math_VecMA)(float *veca, float scale, float *vecb, float *vecc);
	void (*Math_VecToAngles)(const float *value1, float *angles);
	void (*Math_ProjectOntoPlane)(const float *plane, const float *pos, float *out);
	bool (*Math_PointInTriPlanes)(float *v1, float *v2, float *v3, float *pos, float fudge);
	void (*Math_ConfinePointToTri)(float *verts[3], float *triPlane, float *point);
	void (*Math_GetTriEdgeFracs)(float *v1, float *v2, float *v3, float *pos, float *edgeFracs);
	float (*Math_ConstantLerp)(float y0, float y1, float amount);
	float (*Math_LinearLerp)(float y0, float y1, float frac);
	float (*Math_BilinearLerp)(float y0, float y1, float y2, float y3, float fracX, float fracY);
	float (*Math_TriLerp)(float y0, float y1, float y2, float fracX, float fracY, float fracZ);
	float (*Math_CubicLerp)(float y0, float y1, float y2, float y3, float frac);
	float (*Math_PlaneDist)(const float *pl, const float *p);
	float (*Math_GetFloat16)(WORD w);
	void (*Math_QuatToMat)(float *quat, modelMatrix_t *mat, bool compressed, bool transposed);
	void (*Math_MatToQuat)(modelMatrix_t *mat, float *quat, bool compressed);
	void (*Math_QuatSlerp)(float *from, float *to, float f, float *out);
	int (*Math_NextPow2)(int val);
	bool (*Math_CheckPointInTri)(float *v1, float *v2, float *v3, float *point);
	void (*Math_ExpandTriangle)(float *v1, float *v2, float *v3, float units, int components);
	float (*Math_GetMappedValue)(float perc, float *values, int numValues);
	void (*Math_RandSetSeed)(unsigned int seed);
	int (*Math_RandInt)(int low, int high);
	float (*Math_RandFloat)(float low, float high);
	int (*Math_RandIntOnSeed)(int low, int high, unsigned int &seed);
	float (*Math_RandFloatOnSeed)(float low, float high, unsigned int &seed);
	int (*Math_RandIntUnseeded)(int low, int high);
	float (*Math_RandFloatUnseeded)(float low, float high);

	//noesis 2.6 and later
	float (*Math_AngleMod)(float angle);
	float (*Math_BlendAngleLinear)(float in, float goal, float amount);
	void (*Math_RotateMatrixTP)(modelMatrix_t *mat, float ang, float x, float y, float z);

	//noesis 3.46 and later
	WORD (*Math_EncodeFloat16)(float f);

	//noesis 3.56 and later
	void (*Math_AnglesToMatAxis)(float *angles, const modelMatrix_t *mat, int *axOrder);

	//noesis 3.66 and later
	void (*Math_TransformPointByMatrixD)(modelMatrixD_t *matrix, double *in, double *out);
	void (*Math_TransformPointByMatrixNoTransD)(modelMatrixD_t *matrix, double *in, double *out);
	void (*Math_MatrixInverseD)(modelMatrixD_t *in, modelMatrixD_t *out);
	void (*Math_MatrixMultiplyD)(modelMatrixD_t *in, modelMatrixD_t *in2, modelMatrixD_t *out);
	void (*Math_MatrixDToMatrix)(modelMatrixD_t *in, modelMatrix_t *out);
	void (*Math_MatrixToMatrixD)(modelMatrix_t *in, modelMatrixD_t *out);

	//noesis 3.862 and later
	int (*Math_Morton2D)(int x, int y);

	//noesis 3.996 and later
	void (*Math_WorldToScreenSpace)(float *mvp, float screenW, float screenH, float *point, float *out);
	void (*Math_ScreenToWorldSpace)(float *mvp, float screenW, float screenH, float *screenPt, float *out);
	int (*Math_PointRelativeToPlane)(float *pos, float *side, float *planeEq);
	bool (*Math_LineIntersectTri)(float *start, float *end, float *dir, float *triVertX, float *triVertY, float *triVertZ, float *triPlane,
								  float *distOut);

	//noesis 4.0799 and later
	float (*Math_CatmullRomLerp)(float y0, float y1, float y2, float y3, float frac);
	float (*Math_HermiteLerp)(float y0, float y1, float y2, float y3, float frac, float tension, float bias);
	void (*Math_Bezier3D)(const float *points, int numPoints, float frac, float *out);
	int (*Math_ClampInt)(int i, int min, int max);
	int (*Math_WrapInt)(int i, int count);

	//noesis 4.081 and later
	void (*Math_CubicBezier3D)(const float *p0, const float *p1, const float *p2, const float *p3, const float frac, float *out);
	void (*Math_BezierTangent3D)(const float *p0, const float *p1, const float *p2, const float *p3, const float frac, float *out);

	//noesis 4.0824 and later
	float (*Math_CatmullRomTangent)(float y0, float y1, float y2, float y3, float frac);
	void (*Math_CatmullRomLerp3D)(const float *y0, const float *y1, const float *y2, const float *y3, float frac, float *out);
	void (*Math_CatmullRomTangent3D)(const float *y0, const float *y1, const float *y2, const float *y3, float frac, float *out);
	void (*Math_CreateProjectionMatrix)(fourxMatrix_t *mat, float fovX, float fovY, float zNear, float zFar, float ofsW, float ofsH, float fullW, float fullH);

	//noesis 4.143 and later
	//encode/decode functions assume all 0 exponent bits equal 0, and don't handle any other IEEE standards cases.
	//exponent bias for source and destination formats is assumed to be 2^(exponentBits-1)-1.
	unsigned __int64 (*Math_EncodeFloatBitsFromBits)(const unsigned __int64 floatBits, const unsigned __int64 fractionBits, const unsigned __int64 exponentBits, const unsigned __int64 signBits,
														const unsigned __int64 destFractionBits, const unsigned __int64 destExponentBits, const unsigned __int64 destSignBits);
	float (*Math_DecodeFloatFromBits)(const unsigned __int64 floatBits, const unsigned __int64 fractionBits, const unsigned __int64 exponentBits, const unsigned __int64 signBits);

	//noesis 4.144 and later

	//get base-e log for v
	float (*Math_Log)(const float v);

	//get base-2 log for v
	float (*Math_Log2)(const float v);

	//get base-e log for v (double)
	double (*Math_LogD)(const double v);

	//get base-2 log for v (double)
	double (*Math_Log2D)(const double v);

	//convert from linear color space to gamma space
	float (*Math_LinearToGamma)(const float v);

	//convert from gamma space to linear space
	float (*Math_GammaToLinear)(const float v);

	//convert from linear color space to gamma space (double)
	double (*Math_LinearToGammaD)(const double v);

	//convert from gamma space to linear space (double)
	double (*Math_GammaToLinearD)(const double v);

	//calculates fraction bits with the IEEE-based assumption of a leading 1.
	//because the leading 1 is assumed, fractionValues starting under 1 will return a best-case of 0.
	unsigned __int64 (*Math_CalculateFractionBits)(const double fractionValue, const unsigned __int64 fractionBitCount);

	//as above, assuming 32-bit float.
	unsigned int (*Math_CalculateFractionBits32)(const float fractionValue);

	//calculates exponent bits, explicitly clamping between values of 0 and (2^exponentBitCount)-1 under the assumption
	//that those values are reserved as per IEEE 754.
	unsigned __int64 (*Math_CalculateExponentBits)(const double exponentValue, const unsigned __int64 exponentBitCount);

	//as above, assuming 32-bit float.
	unsigned int (*Math_CalculateExponentBits32)(const float exponentValue);

	//extracts the fraction and exponent from a 32-bit float.
	void (*Math_ExtractFractionAndExponent32)(float *pFractionOut, float *pExponentOut, const float v);

	//extracts the fraction and exponent from a 64-bit float.
	void (*Math_ExtractFractionAndExponent64)(double *pFractionOut, double *pExponentOut, const double v);

	void (*Math_TransformQST)(modelMatrix_t *pInOut, const float *pScalingCenter, const float *pScalingRotation,
								const float *pScaling, const float *pRotationCenter, const float *pRotation, const float *pTranslation);

	void (*Math_GetFirstLastBitSet64)(unsigned int *pFirstBit, unsigned int *pLastBitPlusOne, const unsigned __int64 val);

	bool (*Math_SHProjectCubemap)(float *pOutR, float *pOutG, float *pOutB, const noesisTex_t *pTex, const int order);

	//unless otherwise specified, inputs and outputs assume 4-channel rgba
	bool (*Math_CreateIrradianceCubemap)(float *pOut, const int outWidth, const int outHeight, const noesisTex_t *pSrcCubeTex);
	bool (*Math_CreateIrradianceCubemapLambert)(float *pOut, const int outWidth, const int outHeight, const noesisTex_t *pSrcCubeTex);
	bool (*Math_PrefilterCubemapGGX)(float *pOut, const int outMipCount, const noesisTex_t *pSrcCubeTex, int sampleCount, int threadCount, int miscFlags);
	//Math_SampleSphericalProjectionIntoHDRCubemap flags - 1: flip theta, 2: flip phi
	bool (*Math_SampleSphericalProjectionIntoHDRCubemap)(float *pOut, const int outWidth, const int outHeight, const int flags, const noesisTex_t *pSrcSphereTex);

	//calculate approximate derivative of pFn(x)
	double (*Math_CalculateDerivative)(const double x, double (*pFn)(const double x));
	//calculate definite integral of pFn(x) from xMin to xMax, stopping when delta is < errorTolerance
	double (*Math_CalculateIntegral)(const double xMin, const double xMax, const double errorTolerance, double (*pFn)(const double x));

	//DFT & DCT functions are not at all optimized and were done from original formulas as a reference implementation. makes it easier to see data transforms under the hood when doing things
	//properly and not using bit twiddling/look tables/etc. if you have a serious use for these, you should probably be using something like FFTW instead.
	//note that transformed dft input/output (RichComplex) is in the form of complex double*2 numbers. you can cast this memory as std::complex<double> to operate on it.
	void (*Math_DiscreteFourierTransform)(RichComplex *pDftData, const double *pSampleData, const unsigned int sampleCount);
	void (*Math_InverseDiscreteFourierTransform)(double *pSampleData, const RichComplex *pDftData, const unsigned int sampleCount);
	void (*Math_DiscreteFourierTransform2D)(RichComplex *pDftData, const double *pSampleData, const unsigned int sampleWidth, const unsigned int sampleHeight);
	void (*Math_InverseDiscreteFourierTransform2D)(double *pSampleData, const RichComplex *pDftData, const unsigned int sampleWidth, const unsigned int sampleHeight);

	void (*Math_DiscreteCosineTransform)(double *pDctData, const double *pSampleData, const unsigned int sampleCount);
	void (*Math_InverseDiscreteCosineTransform)(double *pSampleData, const double *pDctData, const unsigned int sampleCount);
	void (*Math_DiscreteCosineTransform2D)(double *pDctData, const double *pSampleData, const unsigned int sampleWidth, const unsigned int sampleHeight);
	void (*Math_InverseDiscreteCosineTransform2D)(double *pSampleData, const double *pDctData, const unsigned int sampleWidth, const unsigned int sampleHeight);
	void (*Math_DiscreteCosineTransform3D)(double *pDctData, const double *pSampleData, const unsigned int sampleWidth, const unsigned int sampleHeight, const unsigned int sampleDepth);
	void (*Math_InverseDiscreteCosineTransform3D)(double *pSampleData, const double *pDctData, const unsigned int sampleWidth, const unsigned int sampleHeight, const unsigned int sampleDepth);

	void (*Math_QuatMultiply)(float *pDest, const float *pQ1, const float *pQ2);

	//reserved, do not call.
	int					(*resvA)(void);
	int					(*resvB)(void);
	int					(*resvC)(void);
	int					(*resvD)(void);
	int					(*resvE)(void);
	int					(*resvF)(void);
	int					(*resvG)(void);
	int					(*resvH)(void);
} mathImpFn_t;

typedef struct textParser_s textParser_t;
typedef struct defineDict_s defineDict_t;
//cheap hack for various common comment codes, otherwise use cmtChar.
#define NOE_TEXTPARSER_FLAG_CMT_HASH			(1<<0)
#define NOE_TEXTPARSER_FLAG_CMT_SEMICOLON		(1<<1)
#define NOE_TEXTPARSER_FLAG_CMT_DOUBLESLASH		(1<<2)
typedef struct textParser_s
{
	char				*groupName;
	char				*basePtr;
	char				*curPtr;
	int					lineNum;
	int					groupNum;
	textParser_t		*subGroup;
	bool				textOwner;
	const char			*includeKey;
	bool				isInclude;
	bool				unformattedText;
	bool				parseDoubleQuotes;

	bool				noGroupNames;
	bool				doNotStartGroups;
	bool				resbA;
	char				cmtChar; //defaults to ';' (new as of Noesis 3.6, replaces old reserved bool)
	defineDict_t		*defines;
	int					nestedGroups;

	int					flags;
	int					(*pWhitespaceCallback)(textParser_t *pParser, const char *pPtr);
	void				*resv[3];
} textParser_t;
#define MAX_TOKEN_SIZE		1024
typedef struct parseToken_s
{
	const char			*groupName;
	char				text[MAX_TOKEN_SIZE];
	int					lineNum;
	int					groupNum;
	int					nestedGroups;

	void				*resv[7];
} parseToken_t;

typedef enum
{
	NOEFSMODE_READBINARY = 0,
	NOEFSMODE_WRITEBINARY,
	NOEFSMODE_READWRITEBINARY
} noeFSMode_e;


#ifndef MAKEFOURCC
	#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
					((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) |   \
					((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif
#ifndef FOURCC_DXT1
	#define FOURCC_DXT1  (MAKEFOURCC('D','X','T','1'))
#endif
#ifndef FOURCC_DXT3
	#define FOURCC_DXT3  (MAKEFOURCC('D','X','T','3'))
#endif
#ifndef FOURCC_DXT5
	#define FOURCC_DXT5  (MAKEFOURCC('D','X','T','5'))
#endif
#ifndef FOURCC_ATI1
	#define FOURCC_ATI1  (MAKEFOURCC('A','T','I','1'))
#endif
#ifndef FOURCC_ATI2
	#define FOURCC_ATI2  (MAKEFOURCC('A','T','I','2'))
#endif
#ifndef FOURCC_DXT1NORMAL
	#define FOURCC_DXT1NORMAL  (MAKEFOURCC('D','T','1','N'))
#endif
#ifndef FOURCC_DX10
	#define FOURCC_DX10  (MAKEFOURCC('D','X','1','0'))
#endif
#ifndef FOURCC_BC1
	#define FOURCC_BC1		FOURCC_DXT1
	#define FOURCC_BC2		FOURCC_DXT3
	#define FOURCC_BC3		FOURCC_DXT5
	#define FOURCC_BC4		FOURCC_ATI1
	#define FOURCC_BC5		FOURCC_ATI2
	#define FOURCC_BC6H		(MAKEFOURCC('B','C','6','H'))
	#define FOURCC_BC6S		(MAKEFOURCC('B','C','6','S'))
	#define FOURCC_BC7		(MAKEFOURCC('B','C','7','X'))
#endif

#define NOE_ENCODEDXT_BC1 0
#define NOE_ENCODEDXT_BC3 1
#define NOE_ENCODEDXT_BC4 2

//explicit encode additions (the encode function also accepts the usual fourcc's)
#define NOE_ENCODEDXT_BC2	3
#define NOE_ENCODEDXT_BC5	4
#define NOE_ENCODEDXT_BC6H	5
#define NOE_ENCODEDXT_BC6S	6
#define NOE_ENCODEDXT_BC7	7


typedef struct cntArray_s cntArray_t;
typedef struct cntStream_s cntStream_t;
typedef struct noeRAPI_s noeRAPI_t;
#include "pluginclasses.h"

#define PS2_VIFCODEFROMBITS(a, b, c, d, e, f, g) ( (a<<6)|(b<<5)|(c<<4)|(d<<3)|(e<<2)|(f<<1)|(g<<0) )
#define PS2_VIFCODE_NOP			0
#define PS2_VIFCODE_STCYCL		PS2_VIFCODEFROMBITS(0,0,0,0,0,0,1) //sets CYCLE register
#define PS2_VIFCODE_OFFSET		PS2_VIFCODEFROMBITS(0,0,0,0,0,1,0) //sets OFFSET register
#define PS2_VIFCODE_BASE		PS2_VIFCODEFROMBITS(0,0,0,0,0,1,1) //sets BASE register
#define PS2_VIFCODE_ITOP		PS2_VIFCODEFROMBITS(0,0,0,0,1,0,0) //sets ITOPS register
#define PS2_VIFCODE_STMOD		PS2_VIFCODEFROMBITS(0,0,0,0,1,0,1) //sets MODE register
#define PS2_VIFCODE_MSKPATH3	PS2_VIFCODEFROMBITS(0,0,0,0,1,1,0) //masks GIF PATH3 transfer
#define PS2_VIFCODE_MARK		PS2_VIFCODEFROMBITS(0,0,0,0,1,1,1) //sets MARK register
#define PS2_VIFCODE_FLUSHE		PS2_VIFCODEFROMBITS(0,0,1,0,0,0,0) //waits for end of micro program
#define PS2_VIFCODE_FLUSH		PS2_VIFCODEFROMBITS(0,0,1,0,0,0,1) //waits for end of micro program and end of GIF (PATH1/PATH2) transfer
#define PS2_VIFCODE_FLUSHA		PS2_VIFCODEFROMBITS(0,0,1,0,0,1,1) //waits for end of micro program and end of GIF transfer
#define PS2_VIFCODE_MSCAL		PS2_VIFCODEFROMBITS(0,0,1,0,1,0,0) //activates micro programs
#define PS2_VIFCODE_MSCNT		PS2_VIFCODEFROMBITS(0,0,1,0,1,1,1) //execute micro programs continuously
#define PS2_VIFCODE_MSCALF		PS2_VIFCODEFROMBITS(0,0,1,0,1,0,1) //activates micro programs (vif1)
#define PS2_VIFCODE_STMASK		PS2_VIFCODEFROMBITS(0,1,0,0,0,0,0) //sets MASK register
#define PS2_VIFCODE_STROW		PS2_VIFCODEFROMBITS(0,1,1,0,0,0,0) //sets ROW register
#define PS2_VIFCODE_STCOL		PS2_VIFCODEFROMBITS(0,1,1,0,0,0,1) //sets COL register
#define PS2_VIFCODE_MPG			PS2_VIFCODEFROMBITS(1,0,0,1,0,1,0) //loads micro program
#define PS2_VIFCODE_DIRECT		PS2_VIFCODEFROMBITS(1,0,1,0,0,0,0) //transfers data to GIF (via PATH2)
#define PS2_VIFCODE_DIRECTHL	PS2_VIFCODEFROMBITS(1,0,1,0,0,0,1) //transfers data to GIF (via PATH2)
//#define PS2_VIFCODE_UNPACK	PS2_VIFCODEFROMBITS(1,1,m,v,n,v,l) //decompresses data and writes to vu mem

//vif structures introduced in 3.99
typedef enum
{
	kPS2UPT_Unknown = 0,
	kPS2UPT_S_32,
	kPS2UPT_S_16,
	kPS2UPT_S_8,
	kPS2UPT_V2_32,
	kPS2UPT_V2_16,
	kPS2UPT_V2_8,
	kPS2UPT_V3_32,
	kPS2UPT_V3_16,
	kPS2UPT_V3_8,
	kPS2UPT_V4_32,
	kPS2UPT_V4_16,
	kPS2UPT_V4_8,
	kPS2UPT_V4_5,

	kPS2UPT_Count
} ps2UnpackType_e;
typedef enum
{
	kPS2VifStat_VPS = 0,
	kPS2VifStat_VEW = 2,
	kPS2VifStat_VGW = 3,
	kPS2VifStat_MRK = 6,
	kPS2VifStat_DBF = 7,
	kPS2VifStat_VSS = 8,
	kPS2VifStat_VFS = 9,
	kPS2VifStat_VIS = 10,
	kPS2VifStat_INT = 11,
	kPS2VifStat_ER0 = 12,
	kPS2VifStat_ER1 = 13,
	kPS2VifStat_FDR = 23,
	kPS2VifStat_FQC = 24
} ps2VifStatusBitOfs_e;
typedef enum
{
	kPS2VifMf_None = 0,
	kPS2VifMF_Halt = (1 << 0)
} ps2VifManualFlags_e;
typedef struct ps2VifContext_s
{
	unsigned int	mVIF0Rn[4];
	unsigned int	mVIF0Cn[4];
	unsigned int	mVIF1Rn[4];
	unsigned int	mVIF1Cn[4];

	unsigned int	mVIFnCYCLE_cl[2];
	unsigned int	mVIFnCYCLE_wl[2];
	unsigned int	mVIFnMASK[2];
	unsigned int	mVIFnMODE[2];
	unsigned int	mVIFnITOP[2];
	unsigned int	mVIFnITOPS[2];
	unsigned int	mVIF1BASE;
	unsigned int	mVIF1OFST;
	unsigned int	mVIF1TOP;
	unsigned int	mVIF1TOPS;
	unsigned int	mVIFnMARK[2];
	unsigned int	mVIFnNUM[2];
	unsigned int	mVIFnCODE[2];

	unsigned int	mVIFnSTAT[2];
	unsigned int	mVIFnFBRST[2];
	unsigned int	mVIFnERR[2];

	int				mVIF; //must be 0 or 1

	unsigned int	mVIFDataOffset;
	
	//mDataOffset will be ignored when feeding in a vif context to use with processing.
	//take care to adjust your buffer pointer and size when resuming vif processing instead of relying on this member.
	unsigned int	mDataOffset;

	//mManualFlags may be set from callback functions as desired. if a callback wishes to force vif processing to
	//abort, it can set kPS2VifMF_Halt.
	int				mManualFlags;

	TResvInt mResv[32];
} ps2VifContext_t;
typedef struct ps2UnpackInfo_s
{
	DWORD			numElem;
	DWORD			elemBits;
	ps2UnpackType_e	type;
	TResvInt		resv[3];
} ps2UnpackInfo_t;
typedef struct ps2VifCode_s
{
	WORD			imm;
	BYTE			num;
	BYTE			cmd:7;
	BYTE			icb:1;
} ps2VifCode_t;
typedef struct ps2VifULog_s
{
	int				vifOfs;
	DWORD			dst;
	DWORD			size;
	ps2VifCode_t	vifCode;
	ps2UnpackInfo_t	*upInfo;
	DWORD			dstEnd;
	DWORD			writeStride;
	TResvInt		resv[2];
} ps2VifULog_t;
#define PS2_VIF_CALLBACK_CMD_UNPACK				0xFFFFFFFF
#define PS2_VIF_CALLBACK_CMD_ALL				0xFFFFFFFE
#define PS2_VIF_CALLBACK_CMD_ONINTERRUPT		0xFFFFFFFD
typedef struct ps2VifCallback_s
{
	DWORD			vifCmd; //if this vif command is run, the cb function is called with the vif data and provided user data
	void			*user;
	void			(*cb)(BYTE *vuMem, BYTE *vifData, int dataSize, CArrayList<ps2VifULog_t> &unpackLog, ps2VifContext_t *pCtx, void *user);
	TResvInt		resv[4];
} ps2VifCallback_t;
typedef struct ps2VifExParams_s
{
	//context will be written out to mPreservedContext upon completion of VIF execution.
	//if mUsePreservedContext is true, mPreservedContext will also override the context
	//prior to execution.
	ps2VifContext_t mPreservedContext;

	bool			mIgnoreCycleReg;
	bool			mFullUnpack;
	bool			mUsePreservedContext;
	bool			mFillSkippedData;
	bool			mFillMaskedData;
	bool			mApplyOffsetTo32AsFloat;
	bool			mResvB[2];

	TResvInt mResv[32];
} ps2VifExParams_t;

typedef struct ps2PrimHdr_s
{
	BYTE			numVerts;
	BYTE			pphA;
	BYTE			pphF;
	BYTE			pphB;
	WORD			pphCA;
	WORD			pphCB;
	int				pphD;
	int				pphE;
} ps2PrimHdr_t;
//not part of format spec
typedef enum
{
	PS2GEO_INVALID = -3,
	PS2GEO_LISTS = -2,
	PS2GEO_PRIMITIVE = -1,
	PS2GEO_POSITION = 0,
	PS2GEO_CMPNA,
	PS2GEO_BONEWEIGHTS,
	PS2GEO_BONEINDICES,
	PS2GEO_NORMAL,
	PS2GEO_CMPNB,
	PS2GEO_UV,
	PS2GEO_CMPNC,
	MAX_PS2_COMPONENTS
} ps2CmpnType_e;
typedef struct ps2Prim_s
{
	ps2PrimHdr_t	hdr;
} ps2Prim_t;
typedef struct ps2DrawLists_s
{
	CArrayList<ps2Prim_t>		primList;
	CArrayList<BYTE>			cmpnLists[MAX_PS2_COMPONENTS];
	TResvInt					resv[4];
} ps2DrawLists_t;
typedef struct ps2GeoChunkHdr_s
{
	int				gcA[2];
	WORD			gcB;
	WORD			gcC;
	BYTE			componentType;
	BYTE			gcD;
	BYTE			numPrims;
	BYTE			primFlag;
} ps2GeoChunkHdr_t;

#define NOESPLINEFLAG_CLOSED		(1<<0)

typedef struct noesisSplineKnot_s
{
	float			pos[3];
	float			in[3];
	float			out[3];

	TResvInt		resv[8];
} noesisSplineKnot_t;
typedef struct noesisSpline_s
{
	//knots can be null, if the spline is passed back to indicate only a volume of space
	noesisSplineKnot_t	*knots;
	int					numKnots;
	int					flags;
	float				mins[3];
	float				maxs[3];

	TResvInt			resv[8];
} noesisSpline_t;
typedef struct noesisSplineSet_s
{
	noesisSpline_t		*splines;
	int					numSplines;

	TResvInt			resv[8];
} noesisSplineSet_t;

#define NTEXFLAG_WRAP_REPEAT			0 //this is the default, in absence of other wrap flags
#define NTEXFLAG_WRAP_T_REPEAT			0 //this is the default, in absence of other wrap flags
#define NTEXFLAG_ISNORMALMAP			(1<<0)
#define NTEXFLAG_SEGMENTED				(1<<1)
#define NTEXFLAG_STEREO					(1<<2) //indicates this is a stereo (side by side) image
#define NTEXFLAG_STEREO_SWAP			(1<<3) //should only be used in conjunction with NTEXFLAG_STEREO. Indicates left and right eyes are switched.
#define NTEXFLAG_FILTER_NEAREST			(1<<4) //nearest neighbor filtering is preferred
#define NTEXFLAG_WRAP_CLAMP				(1<<5) //clamp at edges
#define NTEXFLAG_UPLOADED				(1<<6) //has been uploaded to gpu.
#define NTEXFLAG_PREVIEWLOAD			(1<<7) //loaded for preview, not by the loader module
#define NTEXFLAG_CUBEMAP				(1<<8) //cubemap (6 2d textures)
#define NTEXFLAG_PERSISTENT				(1<<9) //not cleaned up with texture pool, user is responsible for freeing
#define NTEXFLAG_ISLINEAR				(1<<10) //is in linear space
#define NTEXFLAG_HDRISLINEAR			(1<<11) //hdr data is in linear space
#define NTEXFLAG_WANTSEAMLESS			(1<<12) //prefer seamless cubemap filtering, if available
#define NTEXFLAG_ISLINEAR_ANY			(NTEXFLAG_ISLINEAR | NTEXFLAG_HDRISLINEAR)
#define NTEXFLAG_WRAP_MIRROR_REPEAT		(1<<13) //mirrored repeat wrap mode, if available
#define NTEXFLAG_WRAP_MIRROR_CLAMP		(1<<14) //mirror clamp to edge wrap mode, if available
#define NTEXFLAG_WRAP_SEP_ST			(1<<15) //separate s/t wrap modes
#define NTEXFLAG_WRAP_T_CLAMP			(1<<16)
#define NTEXFLAG_WRAP_T_MIRROR_REPEAT	(1<<17)
#define NTEXFLAG_WRAP_T_MIRROR_CLAMP	(1<<18)


#define NMATFLAG_NMAPSWAPRA				(1<<0) //swap red and alpha channels when displaying normal map
#define NMATFLAG_TWOSIDED				(1<<1) //no face culling
#define NMATFLAG_PREVIEWLOAD			(1<<2) //loaded/generated for preview, not by the loader module
#define NMATFLAG_USELMUVS				(1<<3) //use lmuv's instead of uv's, if possible
#define NMATFLAG_BLENDEDNORMALS			(1<<4) //use blended normals
#define NMATFLAG_KAJIYAKAY				(1<<5) //kajiya-kay specular, uses alpha of specmap as offset
#define NMATFLAG_SORT01					(1<<6) //sort layer flag
#define NMATFLAG_RESERVE01				(1<<7)
#define NMATFLAG_RESERVE02				(1<<8)
#define NMATFLAG_RESERVE03				(1<<9)
#define NMATFLAG_RESERVE04				(1<<10)
#define NMATFLAG_RESERVE05				(1<<11)
#define NMATFLAG_RESERVE06				(1<<12)
#define NMATFLAG_GAMMACORRECT			(1<<13) //gamma-correct lighting
#define NMATFLAG_VCOLORSUBTRACT			(1<<14) //subtract vertex colors
#define NMATFLAG_PBR_SPEC				(1<<15) //PBR specular lighting model, spec tex with roughness in alpha and spec color in rgb
#define NMATFLAG_PBR_METAL				(1<<16) //PBR metallic lighting model, spec tex with roughness in alpha and metalness in green
#define NMATFLAG_NORMALMAP_FLIPY		(1<<17) //flip y of normal map when rendering, does not affect texture data
#define NMATFLAG_NORMALMAP_NODERZ		(1<<18) //don't derive z when sampling normal map
#define NMATFLAG_PBR_SPEC_IR_RG			(1<<19) //PBR specular lighting model, spec tex with roughness in green, spec intensity (or metalness when combined with metal flag) in red
#define NMATFLAG_ENV_FLIP				(1<<20) //flip environment map
#define NMATFLAG_WANTCLAMP				(1<<21) //prefer clamped texture addressing mode
#define NMATFLAG_PBR_ALBEDOENERGYCON	(1<<22) //albedo energy conservation
#define NMATFLAG_PBR_COMPENERGYCON		(1<<23) //compensate for energy conservation
#define NMATFLAG_PBR_ANY				(NMATFLAG_PBR_SPEC | NMATFLAG_PBR_METAL | NMATFLAG_PBR_SPEC_IR_RG)
#define NMATFLAG_SPRITE_FACINGXY		(1<<24) //a hint (not necessarily obeyed by renderer) which specifies this material applies to a sprite which always faces the camera rotating about z
#define NMATFLAG_NORMAL_UV1				(1<<25) //sample normal map from uv1 instead of uv0
#define NMATFLAG_SPEC_UV1				(1<<26) //sample spec map from uv1 instead of uv0
#define NMATFLAG_BASICBLEND				(1<<27) //hint to use basic blend shader
#define NMATFLAG_FORCESELFSORT			(1<<28) //force cpu sorting of triangles
#define NMATFLAG_PBR_ROUGHNESS_NRMALPHA	(1<<29) //roughness in normal map alpha
#define NMATFLAG_DIFFUSE_UV1			(1<<30) //as with normal/spec, for diffuse/albedo
#define NMATFLAG_UV1_ANY				(NMATFLAG_NORMAL_UV1 | NMATFLAG_SPEC_UV1 | NMATFLAG_DIFFUSE_UV1)

#define NMATFLAG2_LMMASK				(1<<0) //hint to mask lightmap with non-lm material's diffuse/albedo alpha
#define NMATFLAG2_VCOLORMATDIFFUSE		(1<<1) //pre-multiply vertex colors with material diffuse
#define NMATFLAG2_PREFERPPL				(1<<2) //prefer the ppl path even if the appropriate texture resources have not been provided
#define NMATFLAG2_SPEC_IS_GAMMASPACE	(1<<3) //consider spec map to be in gamma space rather than linear space
#define NMATFLAG2_OCCL_UV1				(1<<4) //as with normal/spec, for occlusion
#define NMATFLAG2_OCCL_ISLM				(1<<5) //occlusion map is actually a lightmap - hack for gltf where we let naive implementations just use the lm to mask irradiance
#define NMATFLAG2_OCCL_BLUE				(1<<6) //only use blue channel for occlusion map (default is red)
#define NMATFLAG2_ENV_FLIP_Y			(1<<7) //flip environment map
#define NMATFLAG2_UV1_ANY				(NMATFLAG2_OCCL_UV1 | NMATFLAG2_OPACITY_UV1 | NMATFLAG2_OPACITY_UV2)
#define NMATFLAG2_DECAL					(1<<8) //provides a hint to the renderer that the surface being rendered with the material is a decal
#define NMATFLAG2_CAVITY_PBR_BLUE		(1<<9) //treat blue channel of spec map as a cavity map
#define NMATFLAG2_OPACITY_UV1			(1<<10)
#define NMATFLAG2_OPACITY_UV2			(1<<11)
#define NMATFLAG2_PBR_ROUGHNESS_NRMBLUE	(1<<12) //roughness in normal map blue
#define NMATFLAG2_OCCL_GREEN			(1<<13) //as above, with green

typedef struct noesisModel_s noesisModel_t;
typedef struct mdlMemLocal_s mdlMemLocal_t;

class CNoeCustomData;

class CNoeCustomDataList
{
public:
	CNoeCustomDataList()
		: mpDataHead(NULL)
	{
	}

	CNoeCustomData *CreateCustomData(const char *pName, const char *pType, noeRAPI_t *pRapi, bool allocPooled = true);
	void DestroyCustomData(CNoeCustomData *pData);
	bool DestroyCustomDataByName(const char *pName);
	CNoeCustomData *FindCustomDataByName(const char *pName) const;
	CNoeCustomData *FindCustomDataByType(const char *pType) const;
	void DuplicateListData(CNoeCustomDataList &otherList, noeRAPI_t *pRapi, bool allocPooled = true);
	void AssumeOwnership(CNoeCustomDataList &otherList);
	void DestroyList();

	CNoeCustomData *GetCustomDataRoot() const { return mpDataHead; }

	CNoeCustomDataList &operator=(CNoeCustomDataList &otherList);

private:
	CNoeCustomData *mpDataHead;
};
//this can't change, many plugin-shared structures make assumptions about the size here
NoeCtAssert(sizeof(CNoeCustomDataList) == sizeof(void *));

//will remain constant even if fbx types change
enum ENoeFbxType
{
	kNoeFbxProp_Undefined = 0,
	kNoeFbxProp_Char,
	kNoeFbxProp_UChar,
	kNoeFbxProp_Short,
	kNoeFbxProp_UShort,
	kNoeFbxProp_UInt,
	kNoeFbxProp_LongLong,
	kNoeFbxProp_ULongLong,
	kNoeFbxProp_HalfFloat,
	kNoeFbxProp_Bool,
	kNoeFbxProp_Int,
	kNoeFbxProp_Float,
	kNoeFbxProp_Double,
	kNoeFbxProp_Double2,
	kNoeFbxProp_Double3,
	kNoeFbxProp_Double4,
	kNoeFbxProp_Double4x4,
	kNoeFbxProp_Enum,
	kNoeFbxProp_String,
	kNoeFbxProp_Time,
	kNoeFbxProp_Reference,
	kNoeFbxProp_Blob,
	kNoeFbxProp_Distance,
	kNoeFbxProp_DateTime,

	kNoeFbxProp_Reserved
};

static const int skSharedFbxPropVersion = 1;
//followed by SSharedFbxPropEntry * mPropCount, then followed by data. string/data offsets for each entry are based from mDataOffset.
struct SSharedFbxPropHeader
{
	int mVersion; //skSharedFbxPropVersion
	int mPropCount;
	int mDataOffset;
};
struct SSharedFbxPropEntry
{
	int mNameOffset;
	int mHierarchicalNameOffset;
	int mPropType; //ENoeFbxType
	int mOptionalDataOffset; //-1 if none
	
	int mDataOffset; //-1 if none
	int mDataSize;

	TResvInt mResv[16]; //must be 0
};

#define NSEQFLAG_NONLOOPING				(1<<0) //sequence is not intended to repeat
#define NSEQFLAG_REVERSE				(1<<1) //sequence is reversed

typedef struct noesisASeq_s
{
	char				*name;
	int					startFrame;
	int					endFrame;
	float				frameRate;

	BYTE				userTag[8];
	void				*userData;
	int					userDataSize;
	int					flags;
	TResvInt			resv[3];
} noesisASeq_t;

typedef struct noesisASeqList_s
{
	noesisASeq_t		*s;
	int					numSeq;

	TResvInt			resv[8];
} noesisASeqList_t;

#define NANIMFLAG_FORCENAMEMATCH			(1<<0)	//when set, even if the animation's bone count matches the model's, Noesis will still attempt to use bone names to match bones instead of indices
#define NANIMFLAG_INVALIDHIERARCHY			(1<<1)	//when set, the animation data's hierarchy will be overwritten with that of any available model's
#define NANIMFLAG_FILENAMETOSEQ				(1<<2)	//use filename as sequence name when relevant (hack)
typedef struct noesisAnim_s
{
	char				*filename;
	BYTE				*data;
	int					dataLen;
	bool				shouldFreeData;
	bool				copied;

	noesisModel_t		*mdlPtr;
	noesisASeqList_t	*aseq;
	modelBone_t			*bones;
	int					numBones;
	int					flags;
	int					seqFlags;
	TResvInt			resv[6];
} noesisAnim_t;
typedef struct noesisTexFr_s
{
	int				ofsX;
	int				ofsY;
	int				frameIdx;
	int				viewType;
	float			rad; //i have no idea why you'd want this, but it's preserved mainly for quake spr's at the moment
	int				frameDelay; //3.84
	TResvInt		resv[15];
} noesisTexFr_t; //new in 3.31
enum ENoeHdrTexFormat
{
	kNHDRTF_RGB_F96 = 0,
	kNHDRTF_RGBA_F128,
	kNHDRTF_Lum_F32,
	kNHDRTF_RGBA_F64
};
struct SNoeHDRTexData
{
	void				*pData;
	int					dataLen;
	ENoeHdrTexFormat	hdrFormat;
	int					hdrFlags;

	TResvInt			resv[32];
};
enum ENoePalFormat
{
	kNPF_RGB888 = 0,
	kNPF_RGBA8888
};
#define PALFL_TRANSPARENT_INDEX (1<<0)
struct SNoePalData
{
	void				*pData;
	int					dataLen;
	bool				notDataOwner; //if true, assume pData is pool-allocated or externally managed
	int					colorCount; //dataLen may be padded
	ENoePalFormat		palFormat;
	int					flags;
	int					transparentIndex; //only valid if flags & PALFL_TRANSPARENT_INDEX

	void				*pOriginalIndices; //notDataOwner also applies to this
	int					originalIndexBpp;

	int					originalWidth;
	int					originalHeight;

	TResvInt			resv[30];
};
typedef struct noesisTex_s
{
	char			*filename;
	int				w;
	int				h;
	int				type;
	BYTE			*data;
	int				dataLen;
	int				gltex;
	int				globalIdx; //used optionally by some modules
	int				flags;
	bool			shouldFreeData;

	int				refCount; //do not modify refCount. it's managed internally.
	noesisTexFr_t	*frameInfo; //new in 3.31 - allocate with Noesis_TexFrameInfoAlloc if you want to use it
	int				mipCount;
	short			unused[2]; //must be 0
	SNoeHDRTexData	*pHdr;
	SNoePalData		*pPal;
	TResvInt		resv[3]; //THESE VALUES MUST BE 0 (this is done by Noesis_TextureAlloc)
} noesisTex_t;

typedef struct noesisExtTexRef_s
{
	char			*diffuse;
	char			*normal;
	char			*specular;
	char			*opacity;
	char			*bump;
	char			*env;

	char			*occl;

	TResvInt		reserved[29];
} noesisExtTexRef_t;
/*
Noesis blends:
0 - "None"
1 - "GL_ZERO"
2 - "GL_ONE"
3 - "GL_SRC_COLOR"
4 - "GL_ONE_MINUS_SRC_COLOR"
5 - "GL_SRC_ALPHA"
6 - "GL_ONE_MINUS_SRC_ALPHA"
7 - "GL_DST_ALPHA"
8 - "GL_ONE_MINUS_DST_ALPHA"
9 - "GL_DST_COLOR"
10 - "GL_ONE_MINUS_DST_COLOR"
11 - "GL_SRC_ALPHA_SATURATE"
*/
typedef enum
{
	NOEBLEND_NONE = 0,
	NOEBLEND_ZERO,
	NOEBLEND_ONE,
	NOEBLEND_SRC_COLOR,
	NOEBLEND_ONE_MINUS_SRC_COLOR,
	NOEBLEND_SRC_ALPHA,
	NOEBLEND_ONE_MINUS_SRC_ALPHA,
	NOEBLEND_DST_ALPHA,
	NOEBLEND_ONE_MINUS_DST_ALPHA,
	NOEBLEND_DST_COLOR,
	NOEBLEND_ONE_MINUS_DST_COLOR,
	NOEBLEND_SRC_ALPHA_SATURATE,
	NUM_NOE_BLENDS
} noeBlendMode_e;

//expression functions
/*
	sin(a)
	cos(a)
	tan(a)
	pow(a, b)
	mod(a, b)
	vmap(a, b, ...)
	abs(a)
	floor(a)
	ceil(a)
	rand(a, b)
	min(a, b)
	max(a, b)
	vlen(a, b, c)
*/
//expression variables
/*
	time
	frametime
	pi
	vert_idx
	vert_pos_x
	vert_pos_y
	vert_pos_z
	vert_nrm_x
	vert_nrm_y
	vert_nrm_z
	vert_uv_u
	vert_uv_v
	vert_clr_r
	vert_clr_g
	vert_clr_b
	vert_clr_a
	view_x
	view_y
	view_z
	mtl_diffuse_r
	mtl_diffuse_g
	mtl_diffuse_b
	mtl_diffuse_a
	mtl_spec_r
	mtl_spec_g
	mtl_spec_b
	mtl_spec_exp
	mtl_texidx
	mtl_nrmtexidx
	mtl_spctexidx
	mdl_numtex
*/
typedef struct noesisMatExpr_s
{
	//per-vertex expression evaluators
	rexp_t			*v_posExpr[3];
	rexp_t			*v_nrmExpr[3];
	rexp_t			*v_uvExpr[2];
	rexp_t			*v_clrExpr[4];

	//global material expression evaluators
	rexp_t			*diffuse[4];
	rexp_t			*specular[4];

	rexp_t			*uvTrans[3];
	rexp_t			*uvRot[3];

	rexp_t			*texIdx;
	rexp_t			*normalTexIdx;
	rexp_t			*specularTexIdx;

	void			*resv[16];
} noesisMatExpr_t;

#define PBR_INTERNAL_TEX_COUNT		8
#define PBR_INTERNAL_IRRADIANCE		0
#define PBR_INTERNAL_PREFILTERED	1
#define PBR_INTERNAL_INTEGRATIONMAP	2

typedef struct noesisMatEx_s
{
	float			envMapColor[4]; //alpha is the fresnel term multiplier
	float			ambientColor[4];
	float			blendedNormalFracs[4];

	float			rimColor[3];
	float			rimWidth;
	float			rimPow;
	float			rimOfs[3];
	float			rimBias;

	BYTE			userTag[8];
	void			*userData;
	int				userDataSize;

	//only applicable for pbr. if you have gloss instead of roughness, use roughnessBias 1.0, roughnessScale -1.0
	float			roughnessScale;
	float			roughnessBias;
	float			metalScale;
	float			metalBias;

	//started out roughness-based, became fake-ass anisotropy. could easily do roughness-x/roughness-y as presented by Disney for lights,
	//but want it to be unified with IBL and don't want to do importance sampling at runtime.
	float			roughnessAnisoScale;
	float			roughnessAnisoAngle;

	int				pbrGenTexIdx[PBR_INTERNAL_TEX_COUNT];

	float			*pUvScaleBias; //float * 4
	float			*pUvPlanes; //float * 16

	float			fresnelScale;

	float			*pSpecSwizzle; //float * 4 * 4

	int				flags2;

	int				occlTexIdx;
	float			occlScale;

	TResvInt		resv[22];
} noesisMatEx_t;

typedef struct noesisMaterial_s
{
	char			*name;
	bool			skipRender;
	float			diffuse[4];
	float			specular[4];
	bool			noDefaultBlend;
	int				texIdx;
	int				extTex; //external texture override

	int				blendSrc;
	int				blendDst;
	float			alphaTest;
	bool			noLighting;

	int				normalTexIdx;
	int				specularTexIdx;
	int				transTexIdx;
	int				obsoleteProgramIndex;

	int				flags;

	int				refCount; //do not modify refCount. it's managed internally.
	noesisExtTexRef_t	*extRefs;
	noesisMaterial_s	*nextPass;

	noesisMatExpr_t		*expr; //new in Noesis 4.0 - material expressions

	int				bumpTexIdx;
	int				envTexIdx;

	//extended material structure (running out of room in this one to maintain backwards-compatibility)
	noesisMatEx_t	*ex;

	CNoeCustomDataList mCustomData;

	TResvInt			resv[1];
} noesisMaterial_t;

typedef struct noesisMatData_s
{
	noesisTex_t			*textures;
	int					numTextures;

	noesisMaterial_t	*materials;
	int					numMaterials;

	int					refCount; //do not modify refCount. it's managed internally.
	int					internalFlags; //likewise
	TResvInt			resv[7];
} noesisMatData_t;

typedef struct noesisTexRef_s
{
	noesisTex_t				*t;
	int						loadedIdx;
	char					*loadedName;
	int						pageX;
	int						pageY;
	int						origIdx;
} noesisTexRef_t;

#define NSCENELIGHTFL_ENABLED (1 << 0)
#define NSCENELIGHTFL_DRAWREPRESENTATION (1 << 1)

enum ENoeSceneLightAttenuation
{
	kNSLA_None = 0,
	kNSLA_InverseSquare,

	kNSLA_Count
};

struct SNoeSceneLight
{
	float						mPos[4];
	float						mColor[4];
	int							mFlags;
	float						mRadius;
	int							mAtten; //ENoeSceneLightAttenuation

	TResvInt					mResv[16]; //must be 0
};

typedef enum
{
	RPGEO_NONE = 0,
	RPGEO_POINTS,
	RPGEO_TRIANGLE,
	RPGEO_TRIANGLE_STRIP,
	RPGEO_QUAD, //ABC_DCB
	RPGEO_POLYGON,
	RPGEO_TRIANGLE_FAN,
	RPGEO_QUAD_STRIP,
	RPGEO_TRIANGLE_STRIP_FLIPPED,
	RPGEO_QUAD_ABC_BCD,
	RPGEO_QUAD_ABC_ACD,
	RPGEO_QUAD_ABC_DCA,
	NUM_RPGEO_TYPES
} rpgeoPrimType_e;

typedef enum
{
	RPGEODATA_FLOAT = 0,
	RPGEODATA_INT,
	RPGEODATA_UINT,
	RPGEODATA_SHORT,
	RPGEODATA_USHORT,
	RPGEODATA_HALFFLOAT,
	RPGEODATA_DOUBLE,
	RPGEODATA_BYTE,
	RPGEODATA_UBYTE,
	NUM_RPGEO_DATATYPES
} rpgeoDataType_e;

typedef enum
{
	RPGEOPARAMTYPE_PLANAR_DEFAULT = 0
} rpgeoParamertizationType_e;

#define RPGOPT_BIGENDIAN				(1<<0)
#define RPGOPT_TRIWINDBACKWARD			(1<<1)
#define RPGOPT_TANMATROTATE				(1<<2) //should be used if you want to swap the tangent/bitangent on any tangent matrices derived from normal+tan4
#define RPGOPT_DERIVEBONEORIS			(1<<3) //if this option is set, and exdata bones have been set, the bone matrices will be reset based on vertex weights when the model is constructed
#define RPGOPT_FILLINWEIGHTS			(1<<4) //if set and exdata bones are set, meshes with no vertex weights will be auto-filled based on bone proximity
#define RPGOPT_SWAPHANDEDNESS			(1<<5) //swaps the handedness of the model. this automatically changes handedness of bones and animations too.
#define RPGOPT_UNSAFE					(1<<6) //disables safety bounds checking even when using Safe functions
#define RPGOPT_MORPH_RELATIVEPOSITIONS	(1<<7) //morph positions are relative to mesh positions
#define RPGOPT_MORPH_RELATIVENORMALS	(1<<8) //morph normals are relative to mesh normals
#define RPGOPT_SNAPTANGENTW				(1<<9) //snap tangent w to -1 or 1
#define RPGOPT_TANGENT0HACK				(1<<10) //not important, look away. I SAID LOOK AWAY
#define RPGOPT_GEOTWOSIDEDPRV			(1<<11) //i don't add flags here for specialized game hacks, that accusation is offensive
#define RPGOPT_SANITIZEWEIGHTS			(1<<12) //when rpgSetExData_Bones is used, checks weights against provided bones to find out of range values

#define RPGVUFLAG_PERINSTANCE			(1<<0)
#define RPGVUFLAG_NOREUSE				(1<<1)

#define NMSHAREDFL_WANTNEIGHBORS				(1<<0)
#define NMSHAREDFL_WANTGLOBALARRAY				(1<<1)
#define NMSHAREDFL_WANTTANGENTS					(1<<2)
#define NMSHAREDFL_FLATWEIGHTS					(1<<3)
#define NMSHAREDFL_FLATWEIGHTS_FORCE4			(1<<4) //forces 4 weights per vertex, whether actual source weights are more or less
#define NMSHAREDFL_REVERSEWINDING				(1<<5) //reverses the face winding on the model
#define NMSHAREDFL_WANTTANGENTS4				(1<<6) //requests 4-vector tangents with bitangent sign stored in w
#define NMSHAREDFL_WANTTANGENTS4R				(1<<7) //same as above with reverse winding on tangent calculations
#define NMSHAREDFL_UNIQUEVERTS					(1<<8) //produces a unique vertex for every triangle point
#define NMSHAREDFL_LOCALPOOL					(1<<9) //you must free the model with Noesis_FreeSharedModel if you use this flag
#define NMSHAREDFL_BONEPALETTE					(1<<10) //sets per-mesh bone palettes and modifies bone indices to reference the palette
#define NMSHAREDFL_NOEMPTYMESHES				(1<<11) //ensures that meshes have triangles and positions at a minimum, creating a degenerate tri if needed.
#define NMSHAREDFL_NO_CREATE_UVS				(1<<12) //don't create empty uv's, even if none exist
#define NMSHAREDFL_MULTIMATS					(1<<13) //generate SMultiMaterialMesh list
#define NMSHAREDFL_MULTIMATS_UNIQUE				(1<<14) //generate unique indices for SMultiMaterialMesh list
#define NMSHAREDFL_MULTIMATS_STUB				(1<<15) //stub out SMultiMaterialMesh list without actually collapsing anything
#define NMSHAREDFL_MULTIMATS_MATCHVERTCOMP		(1<<16) //only meaningful with NMSHAREDFL_MULTIMATS, prevents SMultiMaterialMesh from encapsulating meshes with different vertex components
#define NMSHAREDFL_NO_DUP_MATNAMES				(1<<17) //renames materials to prevent duplicate material names if detected		

#define RPG_WELD_FLAG_XY						(1<<0)

typedef struct convertDxtExParams_s
{
	//if non-0, instead of standard z=sqrt(1 - x*x + y*y) for ati2, z=ati2ZScale and then normalize.
	//most games using this technique will simply use z=1 universally.
	float					ati2ZScale;

	//will not attempt to derive z and will not normalize after decoding ati2. should be used in cases
	//where ati2 is housing something that isn't actually a normal map.
	bool					ati2NoNormalize;

	//decodes block color value as signed instead of unsigned
	bool					decodeAsSigned;
	bool					resvBB;
	bool					resvBC;

	TResvInt				resv[15];
} convertDxtExParams_t;

typedef struct sharedPAnimParm_s
{
	int						boneIdx;
	float					angAmt;
	int						axis;
	float					timeScale;

	TResvInt				resv[32]; //this is mainly a debugging/testing feature anyway, might as well bloat it.
} sharedPAnimParm_t;

typedef enum
{
	SHAREDSTRIP_LIST,
	SHAREDSTRIP_STRIP,
	SHAREDSTRIP_NUM
} sharedStripType_e;
typedef struct sharedStripList_s
{
	sharedStripType_e		type;
	WORD					*idx;
	int						numIdx;
} sharedStripList_t;

struct SMultiMaterialMeshIndex
{
	int mMeshIndex;
	int mVertIndex;
};

struct SMultiMaterialMesh
{
	const char *mpName;
	int *mpMeshRefs;
	int mMeshRefCount;
	int mFlags;

	//for each triangle index
	SMultiMaterialMeshIndex *mpUniqueIndices;
	int mUniqueIndexCount;

	//for each vertex
	SMultiMaterialMeshIndex *mpUniqueVertices;
	int mUniqueVertexCount;

	void *mpResv[16];
};

typedef struct sharedUvxChannel_s
{
	int mElemCount; //elements per uv, e.g. 2
	float *mpData;

	void *mpResv[8];
} sharedUvxChannel_t;

typedef struct sharedUvxData_s
{
	int mChanCount;
	sharedUvxChannel_t *mpChans;

	void *mpResv[8];
} sharedUvxData_t;

typedef struct sharedMesh_s
{
	int							numVerts;
	modelVert_t					*verts;
	modelVert_t					*normals;
	modelTangent_t				*tangents;
	modelTexCoord_t				*uvs;
	modelVertWInfo_t			*vertWInfo;
	modelRGBA_t					*colors;

	modelTexCoord_t				*lmCoords;
	modelTangent_t				*lmTangents;
	int							lmIndex;

	int							numWeights;
	newVertWeight_t				*weights;

	modelVMorphFr_t				*morphFrames;
	int							numMorphFrames;

	int							numTris;
	modelTriFace_t				*tris;
	modelTriNeighbors_t			*triNeighbors;

	char						*name;
	char						*skinName;

	//only filled in if requested with NMSHAREDFL_WANTGLOBALARRAY
	int							firstVert;
	int							firstTri;

	//only filled in if requested with NMSHAREDFL_FLATWEIGHTS
	int							*flatBoneIdx;
	float						*flatBoneWgt;
	int							numWeightsPerVert;

	//transformed arrays are only filled in by rpgTransformModel
	modelVert_t					*transVerts;
	modelVert_t					*transNormals;

	void						*internalMesh; //new in Noesis 2.1

	int							*texRefIdx; //new in Noesis 2.2

	int							materialIdx; //new in Noesis 2.95 - is -1 if material is not valid. otherwise an index into the model's matDat material list.

	modelTan4_t					*tan4;

	char						*lmMatName; //new in Noesis 3.52, the optional lightmap material name

	modelUserStream_t			*userStreams; //new in Noesis 4.0965
	int							numUserStreams;

	//only filled in if requested with NMSHAREDFL_BONEPALETTE
	int							*bonePalette;
	int							bonePaletteBoneCount;

	char						*mpSourceName;

	sharedUvxData_t				*mpUvx;

	SExtraPerMorphData			*mpMorphEx;

	void						*resv[4];
} sharedMesh_t;

#define IMRF_SKIPRENDER				(1<<0)
#define IMRF_UVFLIP					(1<<1)
typedef struct sharedMeshInternalProperties_s
{
	int							renderFlags;

	TResvInt					resv[32];
} sharedMeshInternalProperties_t;

typedef struct sharedLightData_s
{
	int						numLightmaps;
	int						lightmapSize;
	BYTE					*lightmapData;
	BYTE					*lightmapVectors;

	//plan to provide a variety of lightgrid data here
	void					*resv[128];
} sharedLightData_t;
typedef struct sharedVMap_s
{
	int							meshIdx;
	int							vertIdx;
} sharedVMap_t;
typedef struct sharedModel_s
{
	sharedMesh_t				*meshes;
	int							numMeshes;

	modelBone_t					*bones;
	int							numBones;

	noesisMatData_t				*matData;
	noesisAnim_t				*animData;

	sharedLightData_t			*lightData;

	//abs array data is zeroed out unless you request it with NMSHAREDFL_WANTGLOBALARRAY
	int							numAbsTris;
	modelLongTri_t				*absTris;
	modelTriNeighbors_t			*absTriNeighbors;
	int							numAbsVerts;
	sharedVMap_t				*absVertMap;

	void						*internalMdl; //new in Noesis 2.2
	mdlMemLocal_t				*memLocal; //new in Noesis 3.46 (do not touch)

	CNoeCustomDataList			mCustomData;

	SMorphGroupInfo				*mpMorphGroupInfo;

	SMultiMaterialMesh			*mpMultiMatMeshes;
	int							mMultiMatMeshCount;

	void						*resv[10];
} sharedModel_t;

typedef struct smNrmParm_s
{
	int							flags;
	TResvInt					resv[8];
} smNrmParm_t; //introduced in noesis 3.871
#define SMNRMPARM_FLAT			(1<<0)
#define SMNRMPARM_PLANESPACEUVS	(1<<1)
#define SMNRMPARM_PLANESPACEUVS_IFNONE	(1<<2)
#define SMNRMPARM_FAKEUVTAN_IFNONE (1<<3)
#define SMNRMPARM_GENMORPHS		(1<<4)

typedef enum
{
	NOEKF_ROTATION_QUATERNION_4 = 0,
	NUM_NOEKF_ROTATION_TYPES
} noeKeyFrameRotation_e;

typedef enum
{
	NOEKF_TRANSLATION_VECTOR_3 = 0,
	NOEKF_TRANSLATION_SINGLE,
	NUM_NOEKF_TRANSLATION_TYPES
} noeKeyFrameTranslation_e;

typedef enum
{
	NOEKF_SCALE_SCALAR_1 = 0,
	NOEKF_SCALE_SINGLE,
	NOEKF_SCALE_VECTOR_3,
	NOEKF_SCALE_TRANSPOSED_VECTOR_3,
	NUM_NOEKF_SCALE_TYPES
} noeKeyFrameScale_e;

typedef enum
{
	NOEKF_INTERPOLATE_LINEAR = 0,
	NOEKF_INTERPOLATE_NEAREST,
	NUM_NOEKF_INTERPOLATION_TYPES
} noeKeyFrameInterpolation_e;


#define KFDFLAG_COMPONENT_MASK		7 //first 3 bits are used to store component index for SINGLE_ key types

typedef struct noeKeyFrameData_s
{
	float						time;
	int							dataIndex; //index into noeKeyFramedAnim's float array
	int							flags; //see KFDFLAG_ values - for _SINGLE types, acts as the component index
	TResvInt					resv;
} noeKeyFrameData_t;

#define KFBONEFLAG_MODELSPACE		(1<<0) //anim data is provided in model space
#define KFBONEFLAG_ADDITIVE			(1<<1) //bone is additive on top of base pose
#define KFBONEFLAG_ADDITIVE_TP		(1<<2) //as above, but transpose result

typedef struct noeKeyFramedBone_s
{
	int							boneIndex;

	noeKeyFrameData_t			*rotationKeys;
	int							numRotationKeys;
	noeKeyFrameRotation_e		rotationType;
	noeKeyFrameInterpolation_e	rotationInterpolation;

	noeKeyFrameData_t			*translationKeys;
	int							numTranslationKeys;
	noeKeyFrameTranslation_e	translationType;
	noeKeyFrameInterpolation_e	translationInterpolation;

	noeKeyFrameData_t			*scaleKeys;
	int							numScaleKeys;
	noeKeyFrameScale_e			scaleType;
	noeKeyFrameInterpolation_e	scaleInterpolation;

	//only used/trusted if anim has KFANIMFLAG_USEBONETIMES is specified
	float						minTime;
	float						maxTime;

	int							flags;

	TResvInt					resv[5];
} noeKeyFramedBone_t;

#define KFANIMFLAG_SEPARATETS		(1<<0) //pull out translation and scale in localspace, apply separately in model space, then transform results back to local
#define KFANIMFLAG_TRANSPOSESCALE	(1<<1) //when used with KFANIMFLAG_SEPARATETS, applies scale to transposed matrix in model space
#define KFANIMFLAG_USEBONETIMES		(1<<2) //use minTime/maxTime for each bone instead of trying to divine range from individual keys
#define KFANIMFLAG_PLUSONE			(1<<3) //add 1 to the sampled frame count, to ensure keys on the edge of the sample duration are caught
#define KFANIMFLAG_ROUNDUP			(1<<4) //round up sample times
typedef struct noeKeyFramedAnim_s
{
	char						*name;
	int							numBones;
	float						framesPerSecond;

	//number of kfBones doesn't necessarily need to be numBones, but numBones must be >= numKfBones
	noeKeyFramedBone_t			*kfBones;
	int							numKfBones;

	float						*data;
	int							numDataFloats;

	int							flags;
	TResvInt					resv[31];
} noeKeyFramedAnim_t;

//added to replace reserved void pointer for rpgSkinPreconstructedVertsToBones.
typedef struct skinVertsToBonesParam_s
{
	int							vertStartIndex;
	int							vertCount;

	TResvInt					resv[16];
} skinVertsToBonesParam_t;

typedef struct decompDrawSeg_s
{
	int						id;
	modelUserStream_t		*streams;
	int						numStreams;
	int						numVertices;
	int						numIndices;

	TResvInt				resv[8];
} decompDrawSeg_t;

typedef struct decompDrawSegList_s
{
	decompDrawSeg_t			*drawSegs;
	int						drawSegCount;

	TResvInt				resv[8];
} decompDrawSegList_t;

#define NOESIS_UDCOMMON_MESH_HIERARCHY_STREAMNAME	"NOESIS_UDCOMMON_MESH_HIERARCHY"
#define NOESIS_UDCOMMON_MESH_HIERARCHY_VERSION		28
#define NOESIS_UDCOMMON_MESH_HIERARCHY_MAXNAMELEN	512
typedef struct noeUDCommonMeshHierarchy_s
{
	int				version;
	char			parentName[NOESIS_UDCOMMON_MESH_HIERARCHY_MAXNAMELEN];
	TResvInt		resv[8];
} noeUDCommonMeshHierarchy_t;

typedef struct noeStringPool_s noeStringPool_t;

typedef void (*NOEXFUNCTION)();

//WIP - not currently available
class CNoeSharedM68000;

//FIXME - this shit is terrible (in implementation too), and these are bad naming conventions. namespace or at least common prefix.
extern void SafeStrCopy(char *dst, int dstLen, const char *src);
extern int AlignInt(int val, int alignTo);
extern int SignedBits(int val, int bits);
extern WORD GetBigWord(WORD w);
extern WORD GetBigWordRaw(BYTE *b);
extern int GetBigInt(int dw);
extern int GetBigIntRaw(BYTE *b);
extern void LittleBigSwap(void *in, int numBytes);
#define LITTLE_BIG_SWAP(a) LittleBigSwap(&a, sizeof(a))
extern noeUDCommonMeshHierarchy_t *GetUDCommonMeshHierarchy(sharedMesh_t *mesh);
size_t fseekread(__int64 ofs, void *dst, size_t elementSize, size_t count, FILE *file);
int freadint(FILE *file, bool bigEnd = false);
int fseekreadint(__int64 ofs, FILE *file, bool bigEnd = false);
short freadshort(FILE *file, bool bigEnd = false);
short fseekreadshort(__int64 ofs, FILE *file, bool bigEnd = false);

#define PVRTC_DECODE_PVRTC2						(1 << 0)
#define PVRTC_DECODE_LINEARORDER				(1 << 1)
#define PVRTC_DECODE_BICUBIC					(1 << 2)
#define PVRTC_DECODE_PVRTC2_ROTATE_BLOCK_PAL	(1 << 3)
#define PVRTC_DECODE_PVRTC2_NO_OR_WITH_0_ALPHA	(1 << 4)

typedef enum
{
	NOESISEYE_CENTER = 0,
	NOESISEYE_LEFT,
	NOESISEYE_RIGHT,
	NUM_NOESIS_EYES
} noesisEye_e;
typedef enum
{
	NGL_PRIM_QUADS,
	NGL_PRIM_TRIANGLES,
	NGL_PRIM_LINES,
	NGL_PRIM_POINTS,
	NGL_PRIM_POLYGONS,
	NUM_NGL_PRIMS
} nglPrimTypes;
typedef struct nglDrawElemParams_s
{
	nglPrimTypes		primType;

	void				*indices;
	int					indicesCount;
	int					indicesType;

	void				*positions;
	int					positionCount;
	int					positionType;
	int					positionStride;

	void				*normals;
	int					normalCount;
	int					normalType;
	int					normalStride;

	void				*colors;
	int					colorElemCount;
	int					colorCount;
	int					colorType;
	int					colorStride;

	void				*uvs;
	int					uvElemCount;
	int					uvCount;
	int					uvType;
	int					uvStride;

	TResvInt			reserved[256];
} nglDrawElemParams_t;
typedef struct noeSharedGL_s
{
	//clear the framebuffer
	void				(*NGL_ClearScreen)(int clearBits, float clrR, float clrG, float clrB, float clrA);
	//check for errors
	bool				(*NGL_CheckErrors)(void);
	//get framebuffer dimensions.
	void				(*NGL_GetResolution)(float &w, float &h);
	//get near and far plane distances.
	void				(*NGL_GetNearFar)(float &zNear, float &zFar);
	//get fov
	float				(*NGL_GetFOV)(void);

	//upload a texture
	void				(*NGL_UploadTexture)(noesisTex_t *texture);
	//bind a texture
	void				(*NGL_BindTexture)(noesisTex_t *texture);

	//primitive drawing and state management. state functions take GL-compliant parameters. (parameters are translated in the graphics module)
	void				(*NGL_Begin)(int primType);
	void				(*NGL_End)(void);
	void				(*NGL_Vertex3fv)(float *v);
	void				(*NGL_Vertex3f)(float a, float b, float c);
	void				(*NGL_Vertex2fv)(float *v);
	void				(*NGL_Vertex2f)(float a, float b);
	void				(*NGL_TexCoord2fv)(float *tc);
	void				(*NGL_TexCoord2f)(float s, float t);
	void				(*NGL_Color4fv)(float *clr);
	void				(*NGL_Color4f)(float r, float g, float b, float a);
	void				(*NGL_Color3fv)(float *clr);
	void				(*NGL_Color3f)(float r, float g, float b);
	void				(*NGL_Enable)(int cap);
	void				(*NGL_Disable)(int cap);
	void				(*NGL_Scissor)(int x, int y, int width, int height);
	void				(*NGL_BlendFunc)(int srcBlend, int dstBlend);
	void				(*NGL_DepthMask)(bool depthMasking);
	void				(*NGL_ColorMask)(bool r, bool g, bool b, bool a);
	void				(*NGL_DepthFunc)(int depthTest);
	void				(*NGL_CullFace)(int cullFace);
	void				(*NGL_PolygonMode)(int face, int mode);
	void				(*NGL_PointSize)(float pointSize);
	void				(*NGL_LineWidth)(float lineWidth);
	void				(*NGL_PolygonOffset)(float factor, float units);
	void				(*NGL_AlphaFunc)(int func, float ref);

	//transforms
	void				(*NGL_ResetProjection)(bool ortho);
	void				(*NGL_Rotate)(float ang, float x, float y, float z);
	void				(*NGL_Translate)(float x, float y, float z);
	void				(*NGL_Scale)(float x, float  y, float z);
	void				(*NGL_OrthoMatrix)(float left, float right, float bottom, float top, float zNear, float zFar);
	void				(*NGL_Frustum)(float left, float right, float bottom, float top, float zNear, float zFar);
	void				(*NGL_LoadIdentity)(void);
	void				(*NGL_LoadMatrix)(float *m);
	void				(*NGL_MultMatrix)(float *m);
	void				(*NGL_PushMatrix)(void);
	void				(*NGL_PopMatrix)(void);
	void				(*NGL_MatrixMode)(int mode);
	void				(*NGL_GetFloat)(int e, float *outBuf);

	noesisEye_e			(*NGL_GetEye)(void);
	void				(*NGL_DrawElems)(const nglDrawElemParams_t *params);

	void				(*NSGL_DrawText)(const float x, const float y, const char *text);
} noeSharedGL_t;

typedef struct noeRAPI_s
{
	void				*(*Noesis_PooledAlloc)(size_t size); //pooled allocations are automatically cleared once the preview/conversion is closed/reset
	char				*(*Noesis_PooledString)(char *str);
	void				*(*Noesis_UnpooledAlloc)(size_t size); //you must free up unpooled allocations yourself
	void				(*Noesis_UnpooledFree)(void *ptr);

	noeStringPool_t		*(*Noesis_CreateStrPool)(void);
	void				(*Noesis_DestroyStrPool)(noeStringPool_t *pool);
	int					(*Noesis_StrPoolGetOfs)(noeStringPool_t *pool, char *str);
	int					(*Noesis_StrPoolSize)(noeStringPool_t *pool);
	BYTE				*(*Noesis_StrPoolMem)(noeStringPool_t *pool);

	NOEXFUNCTION		(*Noesis_GetExtProc)(char *extName);
	char				*(*Noesis_GetExtList)(void);

	BYTE				*(*Noesis_ReadFile)(const char *filename, int *sizeOut); //you must free the pointer returned by Noesis_ReadFile with Noesis_UnpooledFree! (unless it's NULL)
	bool				(*Noesis_WriteFile)(const char *filename, void *data, int dataSize);
	bool				(*Noesis_WriteFileMakePath)(const char *filename, void *data, int dataSize);

	BYTE				*(*Noesis_LoadPairedFile)(char *fileDescr, char *fileExt, int &outLen, char *outPath); //creates an actual "open dialog" prompt for the user
	char				*(*Noesis_GetOutputName)(void);
	char				*(*Noesis_GetInputName)(void);
	char				*(*Noesis_GetLastCheckedName)(void);
	bool				(*Noesis_CheckFileExt)(const char *filename, const char *ext);
	void				(*Noesis_GetLocalFileName)(char *dst, char *src);
	void				(*Noesis_GetExtensionlessName)(char *dst, char *src);
	void				(*Noesis_GetDirForFilePath)(char *dst, char *src);

	noesisTex_t			*(*Noesis_TextureAlloc)(char *filename, int w, int h, BYTE *data, int type);
	noesisMaterial_t	*(*Noesis_GetMaterialList)(int numMaterials, bool texByIndex);
	noesisAnim_t		*(*Noesis_AnimAlloc)(char *filename, BYTE *data, int dataLen);
	noesisMatData_t		*(*Noesis_GetMatData)(noesisMaterial_t *mats, int numMats, noesisTex_t *tex, int numTex);
	noesisMatData_t		*(*Noesis_GetMatDataFromLists)(CArrayList<noesisMaterial_t *> &mats, CArrayList<noesisTex_t *> &tex);

	bool				(*Noesis_HasActiveGeometry)(void);
	int					(*Noesis_GetActiveType)(void);

	//RichPGeo exposure
	void				*(*rpgCreateContext)(void); //create a new context
	void				(*rpgDestroyContext)(void *ctx); //always do this after you're done with a context
	void				(*rpgSetActiveContext)(void *ctx); //note that rpgCreateContext will automatically set the newly-created context as the active one

	void				(*rpgReset)(void);
	void				(*rpgSetMaterial)(char *matName);
	void				(*rpgSetMaterialIndex)(int index);
	void				(*rpgClearMaterials)(void);
	void				(*rpgSetName)(char *objName);
	void				(*rpgClearNames)(void);
	void				(*rpgClearMorphs)(void);
	void				(*rpgSetTransform)(modelMatrix_t *mat); //transforms all vertices/normals
	void				(*rpgSetPosScaleBias)(float *scale, float *bias);
	void				(*rpgSetUVScaleBias)(float *scale, float *bias);
	void				(*rpgSetBoneMap)(int *boneRefMap); //use this for models which use draw-relative bone indices
	//these 2 functions are deprecated, and should be replaced respectively with
	//rpgSetOption(RPGOPT_BIGENDIAN, isBig)
	//rpgSetOption(RPGOPT_TRIWINDBACKWARD, backward)
	void				(*rpgSetEndian)(bool isBig);
	void				(*rpgSetTriWinding)(bool backward);

	void				(*rpgBegin)(rpgeoPrimType_e type);
	void				(*rpgEnd)(void);

	void				(*rpgDataToInt)(int *dst, void *src, int numElem, rpgeoDataType_e srcType, bool bigEndian);
	void				(*rpgDataToFloat)(float *dst, void *src, int numElem, rpgeoDataType_e srcType, bool bigEndian, bool scaleBias);

	void				(*rpgVertex3f)(float *pos);
	void				(*rpgVertexX)(void *data, rpgeoDataType_e dataType, int numComponents);
	void				(*rpgVertex3s)(short *pos);
	void				(*rpgVertex3hf)(WORD *pos);
	void				(*rpgVertNormal3f)(float *nrm);
	void				(*rpgVertNormalX)(void *data, rpgeoDataType_e dataType, int numComponents);
	void				(*rpgVertNormal3us)(WORD *nrm);
	void				(*rpgVertNormal3s)(short *nrm);
	void				(*rpgVertNormal3hf)(WORD *nrm);
	void				(*rpgVertUV2f)(float *uv, int idx);
	void				(*rpgVertUVX)(void *data, rpgeoDataType_e dataType, int numComponents, int idx);
	void				(*rpgVertUV2us)(WORD *uv, int idx);
	void				(*rpgVertUV2s)(short *uv, int idx);
	void				(*rpgVertUV2hf)(WORD *uv, int idx);
	void				(*rpgVertColor4f)(float *clr);
	void				(*rpgVertColorX)(void *data, rpgeoDataType_e dataType, int numComponents);
	void				(*rpgVertColor4us)(WORD *clr);
	void				(*rpgVertColor4ub)(BYTE *clr);
	void				(*rpgVertColor3f)(float *clr);
	void				(*rpgVertColor3us)(WORD *clr);
	void				(*rpgVertColor3ub)(BYTE *clr);
	void				(*rpgVertBoneIndexI)(int *idx, int num);
	void				(*rpgVertBoneIndexX)(void *data, rpgeoDataType_e dataType, int num);
	void				(*rpgVertBoneIndexUB)(BYTE *idx, int num);
	void				(*rpgVertBoneIndexUS)(WORD *idx, int num);
	void				(*rpgVertBoneIndexUI)(DWORD *idx, int num);
	void				(*rpgVertBoneIndexB)(char *idx, int num);
	void				(*rpgVertBoneIndexS)(short *idx, int num);
	void				(*rpgVertBoneWeightF)(float *wgt, int num);
	void				(*rpgVertBoneWeightX)(void *data, rpgeoDataType_e dataType, int num);
	void				(*rpgVertBoneWeightHF)(WORD *wgt, int num);
	void				(*rpgVertBoneWeightUS)(WORD *wgt, int num);
	void				(*rpgVertBoneWeightUB)(BYTE *wgt, int num);
	void				(*rpgVertMorphIndex)(int idx);

	void				(*rpgBindPositionBuffer)(void *data, rpgeoDataType_e dataType, int stride);
	void				(*rpgBindNormalBuffer)(void *data, rpgeoDataType_e dataType, int stride);
	void				(*rpgBindUV1Buffer)(void *data, rpgeoDataType_e dataType, int stride);
	void				(*rpgBindUV2Buffer)(void *data, rpgeoDataType_e dataType, int stride);
	void				(*rpgBindColorBuffer)(void *data, rpgeoDataType_e dataType, int stride, int numChannels);
	void				(*rpgBindBoneIndexBuffer)(void *data, rpgeoDataType_e dataType, int stride, int numWeightsPerVert);
	void				(*rpgBindBoneWeightBuffer)(void *data, rpgeoDataType_e dataType, int stride, int numWeightsPerVert);
	void				(*rpgClearBufferBinds)(void);
	//morph target functionality
	void				(*rpgFeedMorphTargetPositions)(void *data, rpgeoDataType_e dataType, int stride);
	void				(*rpgFeedMorphTargetNormals)(void *data, rpgeoDataType_e dataType, int stride);
	//do not call CommitMorphFrame unless you are sure data fed by FeedMorphX will remain valid
	void				(*rpgCommitMorphFrame)(int numVerts);
	//call once an entire morph set has been committed
	void				(*rpgCommitMorphFrameSet)(void);

	//CommitTriangles should only be called once all of the appropriate vertex buffers are bound
	void				(*rpgCommitTriangles)(void *idxData, rpgeoDataType_e dataType, int numIdx, rpgeoPrimType_e primType, bool usePlotMap);

	void				(*rpgOptimize)(void); //optimizes lists to remove duplicate vertices, sorts triangles by material, etc.
	noesisModel_t		*(*rpgConstructModel)(void); //constructs the model from all given input
	bool				(*rpgFromModel)(noesisModel_t *mdl); //constructs the RichPGeo's contents from a model (returns false if it failed)

	void				(*rpgSetExData_Bones)(modelBone_t *bones, int numBones);
	void				(*rpgSetExData_Materials)(noesisMatData_t *md);
	void				(*rpgSetExData_Anims)(noesisAnim_t *anims);

	//this interface exists in order to allow the noesis model structures to change freely and drastically without breaking plugin compatibility.
	//accessing data directly around a noesisModel_t would be extremely unwise, and virtually guarantees future versions of noesis will break your plugin.
	//additionally, you needn't worry about freeing/destroying shared model handles, as they're pool-allocated.
	sharedModel_t		*(*rpgGetSharedModel)(noesisModel_t *mdl, int sharedMdlFlags);

	//creates transformed vertex arrays using the provided animation matrices
	void				(*rpgTransformModel)(sharedModel_t *pmdl, modelMatrix_t *animMats, int frame);

	//grabs various extents of the model, using transformed positions if available. any input may be null if it's not desired.
	void				(*rpgGetModelExtents)(sharedModel_t *pmdl, float *mins, float *maxs, float *rad, float *xyRad, bool radFromCenter);

	//extracts a list of parent-relative matrices from an animation, in the format of ((matrix for each bone) for each frame).
	//if pooled is false, non-null returned pointer must be freed, via Noesis_UnpooledFree.
	modelMatrix_t		*(*rpgMatsFromAnim)(noesisAnim_t *anim, int &numFrames, float &frameRate, int *numBones, bool pooled);

	//rpgAnimFromBonesAndMats expects a list of matrices, ordered as ((matrix for each bone) for each frame)
	//animation matrices should be fed as parent-relative. data does not need to be freed. (it's pooled)
	//IMPORTANT NOTE!!!!!!!
	//you should use rpgAnimFromBonesAndMatsFinish instead if you are passing the final data for export (which is going to be most of the time)
	//only use this function instead of rpgAnimFromBonesAndMatsFinish when you are going to be ripping the data back out for your own local purposes.
	noesisAnim_t		*(*rpgAnimFromBonesAndMats)(modelBone_t *bones, int numBones, modelMatrix_t *animMats, int numFrames, float frameRate);

	void				(*rpgMultiplyBones)(modelBone_t *bones, int numBones);

	void				(*SetPreviewAnimSpeed)(float animSpeed);
	void				(*SetPreviewAngOfs)(float *mdlAngOfs);

	int					(*LogOutput)(const char *fmt, ...);

	//ADDED IN NOESIS 2.1
	//performs rpgAnimFromBonesAndMats, and adds additional process-global rotations, translations, etc. to the animation matrices
	noesisAnim_t		*(*rpgAnimFromBonesAndMatsFinish)(modelBone_t *bones, int numBones, modelMatrix_t *animMats, int numFrames, float frameRate);

	//ADDED IN NOESIS 2.2
	//returns a value string for an option, or NULL if the option is not available.
	char							*(*Noesis_GetOption)(char *optionName);
	//takes a source rgba32 image, a destination 8bpp (palette index) of the same dimensions, and a 256-entry rgba32 (1024 bytes) palette.
	//fills in the contents of dstPix appropriately, using the provided palette to match colors from the rgba32 image.
	void							(*Noesis_ApplyPaletteRGBA)(BYTE *rgba, int w, int h, BYTE *dstPix, BYTE *pal);
	//same as Noesis_ApplyPaletteRGBA, except the dstPal is filled in with this operation. if firstClear is set, the first entry in the palette
	//is set to (0,0,0,0) (black, no alpha)
	void							(*Noesis_PalettizeRGBA)(BYTE *rgba, int w, int h, BYTE *dstPix, BYTE *dstPal, bool firstClear);
	//sets and gets extra animation data, to be checked/used by formats that wish to compile animation data into output models
	void							(*Noesis_SetExtraAnimData)(BYTE *animData, int animDataSize);
	BYTE							*(*Noesis_GetExtraAnimData)(int &animDataSize);
	//loads model textures into a single list, including externally-referenced textures. note that this may change the address of material data.
	CArrayList<noesisTexRef_t>		&(*Noesis_LoadTexturesForModel)(sharedModel_t *pmdl);
	//creates a single RGBA32 texture page from a list of textures, filling pageWidth and pageHeight with the end result's dimensions.
	//after calling this function, the texref's pageX/Y values can be used to locate the texture within the page.
	//non-null returned pointer must be freed with Noesis_UnpooledFree.
	BYTE							*(*Noesis_CreateRefImagePage)(CArrayList<noesisTexRef_t> &trefs, int &pageWidth, int &pageHeight);
	//performs a bilinear image resize on rgba32 image data
	void							(*Noesis_ResampleImageBilinear)(unsigned char *src, int srcW, int srcH, unsigned char *dst, int dstW, int dstH);
	//generates a list of triangle strip indices from a mesh triangle list (stripOut pointer is pool-allocated)
	bool							(*rpgGenerateStripIndices)(modelTriFace_t *tris, int numTris, WORD **stripOut, int &stripIdxNum);
	//generates a list of triangle strips, each of which can contain indices in the form of triangle lists or strips. (unless doStitch is true, which results in a single list of strip indices)
	//all data provided by this function is pool-allocated.
	bool							(*rpgGenerateStripLists)(const WORD *idx, int numIdx, sharedStripList_t **stripOut, int &stripNum, bool doStitch);
	//text parsing functions
	textParser_t					*(*Parse_InitParser)(char *rawText);
	textParser_t					*(*Parse_InitParserFromFile)(const char *filename);
	bool							(*Parse_WhiteSpace)(char chr);
	void							(*Parse_FreeParser)(textParser_t *parser);
	void							(*Parse_EnableInclude)(textParser_t *parser, const char *includeKey);
	bool							(*Parse_GetNextToken)(textParser_t *parser, parseToken_t *parseToken);
	//check if a file exists
	bool							(*Noesis_FileExists)(const char *filename);

	//ADDED IN NOESIS 2.3
	//allows you to specify an array of animation data
	void				(*rpgSetExData_AnimsNum)(noesisAnim_t *anims, int animsNum);
	//creates a contiguous block of animations from a list (pool-allocated)
	noesisAnim_t		*(*Noesis_AnimsFromList)(CArrayList<noesisAnim_t *> &animList, int &animsOut);
	//creates a contiguous block of models from a list (pool-allocated)
	noesisModel_t		*(*Noesis_ModelsFromList)(CArrayList<noesisModel_t *> &modelList, int &modelsOut);
	//procedurally generates an animation, rotating given bone(s) along given axis(es). useful for testing your bone weights.
	noesisAnim_t		*(*rpgCreateProceduralAnim)(modelBone_t *bones, int numBones, sharedPAnimParm_t *parms, int numParms, int numFrames);
	//DEPRECATED. DO NOT USE.
	modelBone369_t		*(*Noesis_AllocBones_369_DEPRECATED)(int numBones);
	//allocates a placeholder texture
	noesisTex_t			*(*Noesis_AllocPlaceholderTex)(char *name, int width, int height, bool normalMap);
	//performs a variety of normal-correction operations on a single 4-byte rgba32 pixel
	void				(*Noesis_SwizzleNormalPix)(BYTE *p, bool swapAR, bool deriveB, bool isSigned);
	//untiles raw pixel data
	void				(*Noesis_UntileImageRAW)(BYTE *dst, BYTE *src, int dstSize, int imgW, int imgH, int bytesPerPix);
	//untiles dxt-encoded pixel data
	void				(*Noesis_UntileImageDXT)(BYTE *dst, BYTE *src, int dstSize, int imgW, int imgH, int blockSize);

	//ADDED IN NOESIS 2.5
	//decompression functions return < 0 on failure.
	//zlib inflate (return 0 means success)
	int					(*Decomp_Inflate)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);
	//puff
	int					(*Decomp_Puff)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD *dstSize);
	//blast
	int					(*Decomp_Blast)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD *dstSize);
	//lzs used in ff7/ff8
	int					(*Decomp_LZS01)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);
	//used in various capcom games, mainly
	int					(*Decomp_FPK)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);
	//lzma
	int					(*Decomp_LZMA)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize, BYTE *props, int propsSize);
	//lzo 1x (used in silent hill homecoming, maybe others)
	int					(*Decomp_LZO)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);
	//lzo (used in metroid prime 2, maybe others)
	int					(*Decomp_LZO2)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);
	//zlib deflate, returns compressed size
	int					(*Compress_Deflate)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize, int level);
	//compress lzma, recommended settings: level=9, dictSize=(1<<16), lc=3, lp=0, pb=2, fb=32, returns compressed size
	int					(*Compress_LZMA)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize, BYTE *props, int *propsSize, int level, int dictSize, int lc, int lp, int pb, int fb);
	//converts dxt data to rgba32. dxtFmt is the FOURCC code for the pixel data. you must free the non-null return pointer with Noesis_UnpooledFree.
	//this function also handles FOURCC_ATI2. (special normal compression mode)
	BYTE				*(*Noesis_ConvertDXT)(int w, int h, BYTE *data, DWORD dxtFmt);
	//untwiddles data which has been twiddled for the psp gpu. pixDataSize (in bytes) can be larger than width*height*bpp would dictate, to indicate padding.
	bool				(*Noesis_UntwiddlePSP)(BYTE *pixData, int pixDataSize, int width, int height, int bitsPerPixel);
	//writes an exported file entry from an archive. filename should be the archive-relative path, NOT the absolute path.
	bool				(*Noesis_ExportArchiveFile)(char *filename, BYTE *buf, int bufSize);
	//opens a FILE stream in binary write mode, using the same path system as the above function. you are responsible for closing the file.
	//DO NOT USE THIS FUNCTION ANYMORE - use Noesis_ExportArchiveFileOpen instead
	FILE				*(*Noesis_ExportArchiveFileOpenDEPRECATED)(char *filename, FILE *(*openfunc)(const char *filename, const char *mode));

	//new in Noesis 2.7
	//get a bone list along with other anim data, which describes hierarchy and anim bones
	modelMatrix_t		*(*rpgMatsAndInfoFromAnim)(noesisAnim_t *anim, int &numFrames, float &frameRate, int *numBones, modelBone_t **boneInfo, bool pooled);
	//final output point for export of animation data
	bool				(*Noesis_WriteAnimFile)(char *animName, char *extension, BYTE *buf, int len);
	//allocate animation sequence data
	noesisASeqList_t	*(*Noesis_AnimSequencesAlloc)(int numSequences, int numFrames);
	//allocate a model container
	noesisModel_t		*(*Noesis_AllocModelContainer)(noesisMatData_t *matData, noesisAnim_t *animData, int animDataNum);
	//generic lzss decompression. an example for schemes commonly seen on the wii would be
	//Decomp_LZSSGeneric(srcBuf, dstBuf, srcSize, dstSize, 8, 4, 12, false, 3, -1, true, false, true);
	int					(*Decomp_LZSSGeneric)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize,
											int ctrlBits, int lenBits, int ofsBits, bool lenFirst, int addLen, int addOfs, bool revCtrlBits, bool nzLiteral, bool bigRefBits);
	//do a qsort operation on a bone list. returns pointer to a mapping of which bone went where. map is pool-allocated and does not need to be freed.
	//this function also re-associates parents correctly within the bones list. if postHieSort is true, final list will also be sorted by hierarchy.
	int					*(*Noesis_QSortBones)(modelBone_t *bones, int numBones, int (*compareFunc)(const modelBone_t *a, const modelBone_t *b), bool postHieSort);
	//pool-allocates a full copy of the bones list, correctly adjusts parent, etc. pointers
	modelBone_t			*(*Noesis_CopyBones)(modelBone_t *bones, int numBones);
	//uses hashing to quickly create a list of unique elements. (using memory comparison)
	//returned pointer is to a list of unique elements, and must be freed with Noesis_UnpooledFree.
	//numElems will be modified to contain the output number of unique elments.
	//elemMap should be NULL, or an array of numElems ints, which will contain the mapped element indices from elemData into the returned array.
	void				*(*Noesis_GetUniqueElements)(void *elemData, int elemStride, int elemSize, int &numElems, int *elemMap);

	//new in Noesis 2.95
	//allocate external texture reference memory, which you may attach to a noesisMaterial_t.
	//all memory (including string copies passed back on the object) is pooled and does not need to be freed.
	//any and/or all arguments may be null.
	noesisExtTexRef_t	*(*Noesis_AllocTexRefsOLD)(char *diffuse, char *normal, char *specular, char *opacity);

	//sets preview options. current options available are:
	//key									value(s)
	//----------------------------------------------
	//"drawAllModels"						"0"/"1" (toggles drawing all models at once in preview mode by default)
	//"noTextureLoad"						"0"/"1" (toggles auto-loading of textures for previewed model based on tex/mat names)
	//"setAnimPlay"							"0"/"1" (if 1, auto-starts animation in preview)
	//"setSkelToShow"						"0"/"1" (if 1, displays skeleton by default)
	void				(*SetPreviewOption)(char *optName, char *optVal);

	//new in Noesis 2.97
	wchar_t				*(*Noesis_GetOutputNameW)(void);
	wchar_t				*(*Noesis_GetInputNameW)(void);
	wchar_t				*(*Noesis_GetLastCheckedNameW)(void);
	bool				(*Noesis_CheckFileExtW)(const wchar_t *filename, const wchar_t *ext);
	void				(*Noesis_GetLocalFileNameW)(wchar_t *dst, wchar_t *src);
	void				(*Noesis_GetExtensionlessNameW)(wchar_t *dst, wchar_t *src);
	void				(*Noesis_GetDirForFilePathW)(wchar_t *dst, wchar_t *src);
	//sets anim data for a model. this call is only necessary to set animations for a model if rpgSetExData_Anims was not used before its creation.
	void				(*Noesis_SetModelAnims)(noesisModel_t *mdl, noesisAnim_t *anims, int numAnims);
	//sets material data for a model. this call is only necessary to set materials for a model if rpgSetExData_Materials was not used before its creation.
	void				(*Noesis_SetModelMaterials)(noesisModel_t *mdl, noesisMatData_t *md);
	BYTE				*(*Noesis_LoadPairedFileW)(wchar_t *fileDescr, wchar_t *fileExt, int &outLen, wchar_t *outPath);
	BYTE				*(*Noesis_ReadFileW)(const wchar_t *filename, int *sizeOut); //you must free the pointer returned by Noesis_ReadFile with Noesis_UnpooledFree! (unless it's NULL)
	bool				(*Noesis_WriteFileW)(const wchar_t *filename, void *data, int dataSize);
	bool				(*Noesis_WriteFileMakePathW)(const wchar_t *filename, void *data, int dataSize);

	//new in Noesis 2.98
	//checks the properties of a material against others in the list, and returns the index of a material that matches if one already exists. if none exists, returns index of the new material in the list.
	//if the includeName argument is true, the names of the materials will also be compared in checking that they are identical.
	int					(*Noesis_FindOrAddMaterial)(CArrayList<noesisMaterial_t *> &matList, noesisMaterial_t *mat, bool includeName);
	//if enabled is non-0, the model will have its triangles sorted by material and distance from the viewer before being drawn.
	//enabled=1 means that triangles will be sorted by distance of mesh before distance of triangles, generally keeping draw batching intact.
	//enabled=2 means that triangles will be sorted purely by their own distance. however, this can interrupt draw batching, creating thousands of draw calls and destroying performance.
	void				(*Noesis_SetModelTriSort)(noesisModel_t *mdl, int enabled);

	//new in Noesis 2.981
	noesisModel_t		*(*rpgConstructModelAndSort)(void); //performs rpgConstructModel with a triangle sort

	//new in Noesis 2.99
	//performs an in-place ps2 untwiddling. bpp (bits per pixel) must be 4 or 8.
	void				(*Noesis_UntwiddlePS2)(BYTE *pix, int w, int h, int bpp);

	//new in Noesis 3.0
	//creates a single animation with a sequence list from all of the combined animations fed in.
	//all animations must have the same bone count, or the function will fail and return NULL.
	noesisAnim_t		*(*Noesis_AnimFromAnims)(noesisAnim_t *anims, int numAnims);
	//identical to Noesis_AnimFromAnims, but accepts a pointer list as input, and returns NULL if any entry in the list is NULL.
	noesisAnim_t		*(*Noesis_AnimFromAnimsList)(CArrayList<noesisAnim_t *> &anims, int numAnims);

	//new in Noesis 3.1
	//opens a file stream handle
	void				*(*Noesis_FSOpen)(const wchar_t *filename, noeFSMode_e mode);
	//closes a file stream handle
	void				(*Noesis_FSClose)(void *file);
	//get file size
	__int64				(*Noesis_FSGetSize)(void *file);
	//seek
	void				(*Noesis_FSSeek)(void *file, __int64 pos, bool seekRelative);
	//get position
	__int64				(*Noesis_FSTell)(void *file);
	//eof?
	bool				(*Noesis_FSEOF)(void *file);
	//reads from a file stream handle
	__int64				(*Noesis_FSRead)(void *dstBuf, __int64 size, void *file);
	//write to a file stream handle
	__int64				(*Noesis_FSWrite)(const void *srcBuf, __int64 size, void *file);
	//guess the extension of a file based on its contents (currently fairly limited in possible results, will be expanding in the future)
	char				*(*Noesis_GuessFileExt)(BYTE *data, int dataLen);

	//new in Noesis 3.15
	//generates smooth normals for the rpgeo context. should be called after all geometry has been added, but before rpgConstructModel.
	//resv MUST BE NULL. (may be utilized in future updates)
	void				(*rpgSmoothNormals)(smNrmParm_t *parms);

	//new in Noesis 3.17
	//pixStride is expected to be 3 for rgb888 or 4 for rgba8888 data. desiredColors can be any number.
	//if useAlpha is true (and pixStride is 4), the alpha channel will be considered in the process.
	//the returned buffer will be in rgba8888 format (regardless of pixStride), and must be freed by Noesis_UnpooledFree.
	BYTE				*(*Image_GetMedianCut)(BYTE *pix, int pixStride, int w, int h, int desiredColors, bool useAlpha);

	//New in Noesis 3.26
	//opens a file stream in binary write mode, using the same path system as the above function. you are responsible for closing the file.
	//use Noesis_FSClose to close the file handle.
	void				*(*Noesis_ExportArchiveFileOpen)(char *filename);

	//new in Noesis 3.27
	//functions similarly to LoadPairedFile, but returns a read-only file handle instead. The file must be closed with Noesis_FSClose
	//when you're done with it.
	void				*(*Noesis_OpenPairedFile)(wchar_t *fileDescr, wchar_t *fileExt, wchar_t *outPath);

	//new in Noesis 3.28
	//calculates tangents (allocates a pooled buffer if tangents is NULL)
	//also triStride should be the size of a *triangle*, not the size of a single index.
	modelTan4_t			*(*rpgCalcTangents)(int numVerts, int numIdx, void *idxData, rpgeoDataType_e idxDataType, int triStride,
												void *posData, rpgeoDataType_e posDataType, int posStride,
												void *nrmData, rpgeoDataType_e nrmDataType, int nrmStride,
												void *uvData, rpgeoDataType_e uvDataType, int uvStride,
												modelTan4_t *tangents, bool reverseWinding);
	bool				(*Noesis_ExportArchiveFileCheckExists)(char *filename);

	//new in Noesis 3.31
	//allocates a noesisTexFr_t for a texture
	noesisTexFr_t		*(*Noesis_TexFrameInfoAlloc)(void);

	//new in Noesis 3.46
	//gets the NPAPI_Register-returned index associated with the current type handler
	int					(*Noesis_GetTypeHandlerIdx)(void);
	//creates a dds file in memory from dxt data and supplied parameters. dxtFmt can be NOESISTEX_DXT* or a FOURCC code.
	//you must free the non-null return pointer with Noesis_UnpooledFree.
	BYTE				*(*Image_CreateDDSFromDXTData)(BYTE *data, int dataSize, int w, int h, int numMips, int dxtFmt, int *sizeOut);
	//same as above but with tga and rgba32 data
	//you must free the non-null return pointer with Noesis_UnpooledFree.
	BYTE				*(*Image_CreateTGAFromRGBA32)(BYTE *data, int dataSize, int w, int h, int *sizeOut);
	//you probably shouldn't use this unless you're me or you really know how things work internally
	void				(*rpgSetExData_LocalPool)(bool usePool);
	//typically you will not want to call this function at all, as long as you pass back your created model(s) to noesis.
	//this call is dangerous and can also destroy data linked to the model if no other models are referencing it anymore (such as textures, materials, and anims)
	//you must also never call Noesis_FreeModels on a model before calling Noesis_FreeSharedModel on any shared models created from it.
	void				(*Noesis_FreeModels)(noesisModel_t *mdl, int numMdl);
	//this only needs to be called if the shared model was created with NMSHAREDFL_LOCALPOOL
	void				(*Noesis_FreeSharedModel)(sharedModel_t *mdl);
	//copies all mesh data off of shared model, so that the shared model can be safely freed/discarded afterward.
	//however, any material, animation, or external data pointed to be the shared model will only be pointed to be the noesis model, and should *not* be freed until the model itself is.
	noesisModel_t		*(*Noesis_ModelFromSharedModel)(sharedModel_t *mdl);
	//reads encoded 8-byte path name into wchar buffer (assumed dst is MAX_NOESIS_PATH long)
	void				(*Noesis_GetEncodedWidePath)(wchar_t *dst, char *src);
	//the deferred anim object should not be shared by any models, as it will be freed along with the rapi instance. always copy the deferred anim (and its data) off before using it.
	void				(*Noesis_SetDeferredAnim)(noesisAnim_t *anim);
	noesisAnim_t		*(*Noesis_GetDeferredAnim)(void);
	//assumes dst is MAX_NOESIS_PATH long
	bool				(*Noesis_GetTypeExtension)(char *dst, int fh);

	//new in Noesis 3.5
	//tangents are expected to be 4 components, with the vector in the first 3 and bitangent sign (-1 or 1) in the 4th
	void				(*rpgVertTan4f)(float *tan);
	void				(*rpgVertTanX)(void *data, rpgeoDataType_e dataType, int numComponents);
	void				(*rpgVertTan4us)(WORD *tan);
	void				(*rpgVertTan4s)(short *tan);
	void				(*rpgVertTan4hf)(WORD *tan);
	void				(*rpgBindTangentBuffer)(void *data, rpgeoDataType_e dataType, int stride);
	//resv must be NULL
	void				(*rpgSmoothTangents)(void *resv);
	//use the RPGOPT_ flags with these functions
	void				(*rpgSetOption)(int optFlag, bool enabled);
	bool				(*rpgGetOption)(int optFlag);

	//new in Noesis 3.52
	//attempts to load a texture on the given path (also tries input-relative paths), returns NULL if nothing was found.
	//automatically attempts to load the texure in all importable image formats.
	noesisTex_t			*(*Noesis_LoadExternalTex)(char *texName);
	//sets the second-pass/lightmap material
	void				(*rpgSetLightmap)(char *lmMatName);

	//new in Noesis 3.54
	//parses through a deflate stream to determine the final destination size
	int					(*Noesis_GetInflatedSize)(BYTE *srcBuf, DWORD srcSize);

	//new in Noesis 3.55
	//loads a texture by matching the loading handler(s) to the given extension
	noesisTex_t			*(*Noesis_LoadTexByHandler)(BYTE *srcBuf, DWORD srcSize, char *ext);

	//new in Noesis 3.59
	//performs an in-place ps2 twiddling. bpp (bits per pixel) must be 4 or 8.
	void				(*Noesis_TwiddlePS2)(BYTE *pix, int w, int h, int bpp);

	//new in Noesis 3.66
	void				(*rpgVertex3d)(double *pos);
	void				(*rpgVertNormal3d)(double *nrm);
	void				(*rpgVertUV2d)(double *uv, int idx);
	void				(*rpgVertColor4d)(double *clr);
	void				(*rpgVertColor3d)(double *clr);
	void				(*rpgVertBoneWeightD)(double *wgt, int num);

	//new in Noesis 3.69
	void				(*Noesis_FilterFileName)(char *filename);
	void				(*Noesis_FilterFileNameW)(wchar_t *filename);

	//new in Noesis 3.7
	//provides a unified interface for pool-allocating bones
	//(this is a new version of the Noesis_AllocBones function which allocates the new bone structure)
	modelBone_t			*(*Noesis_AllocBones)(int numBones);

	//new in Noesis 3.72
	int					(*Decomp_LZHMelt)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);
	int					(*Noesis_GetLZHMeltSize)(BYTE *srcBuf, DWORD srcSize);

	//new in Noesis 3.73
	void				(*rpgSetStripEnder)(int stripEndVal);
	int					(*rpgGetStripEnder)(void);

	//new in Noesis 3.84
	//for a typical xmemcompress'd stream, windowBits should be 17, resetInterval should be -1, and frameSize should be -1. (for LZX default)
	//returns -1 if failed, or actual size of decompressed data.
	int					(*Decomp_XMemLZX)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize, int windowBits, int resetInterval, int frameSize);

	//Image_GetTexRGBA will get you a buffer of raw RGBA32 data from a noesis texture.
	//=====IMPORTANT READ OR SMALL CHILDREN AND ANIMALS WILL DIE AS A RESULT OF YOUR NEGLIGENCE=====
	//IMPORTANT: if shouldFree comes back true, you must free the returned buffer with Noesis_UnpooledFree when you're done with it.
	//shouldFree will be false if the texture data is already in native rgba32 form, in this case you will simply be given a pointer
	//directly to the texture data buffer, and freeing that pointer would be a terrible world-ending thing to do. So don't do that.
	//=====IMPORTANT READ OR SMALL CHILDREN AND ANIMALS WILL DIE AS A RESULT OF YOUR NEGLIGENCE=====
	BYTE				*(*Image_GetTexRGBA)(noesisTex_t *tex, bool &shouldFree);

	//new in Noesis 3.852
	BYTE				*(*Image_DecodePVRTC)(BYTE *dataPtr, int sz, int srcW, int srcH, int bitsPP);

	//new in Noesis 3.97
	//grabs an interpolated sample from a rgba32 image. returns rgba colors as 4 floats in a 0.0-1.0 range in dst.
	void				(*Image_InterpolatedSample)(BYTE *img, int w, int h, float fracX, float fracY, float *dst);
	//sets the module into global data mode and loads a file. this should only be invoked by tools, do not invoke it in format handlers or you will probably crash noesis.
	//use Noesis_FreeGData when you're done with the loaded data.
	bool				(*Noesis_LoadGData)(wchar_t *filename);
	//frees global data.
	void				(*Noesis_FreeGData)(void);
	//sets global data. same as LoadGData except this allows you to create the model yourself instead of loading from storage.
	void				(*Noesis_SetGData)(noesisModel_t *mdl, int numMdl);
	//exports global data to file.
	bool				(*Noesis_ExportGData)(wchar_t *filename, char *options);
	//gets the loaded model count
	int					(*Noesis_GetLoadedModelCount)(void);
	//gets a loaded model.
	noesisModel_t		*(*Noesis_GetLoadedModel)(int modelIdx);

	//new in Noesis 3.971
	//returns a gaussian-blurred rgba32 image from a source rgba32 image. the returned pointer must be freed with Noesis_UnpooledFree
	BYTE				*(*Image_GaussianBlur)(BYTE *img, int width, int height, float sigma);

	//new in Noesis 3.98
	//functions the same as rpgCalcTangents, but converts a full tangent matrix to an array of tan4's
	modelTan4_t			*(*rpgConvertTangents)(int numVerts,
											void *nrmData, rpgeoDataType_e nrmDataType, int nrmStride,
											void *tanData, rpgeoDataType_e tanDataType, int tanStride,
											void *bitanData, rpgeoDataType_e bitanDataType, int bitanStride,
											modelTan4_t *tangents);

	//new in Noesis 3.99
	//if returned pointer is non-NULL, it must be freed with Noesis_UnpooledFree
	WORD				*(*Noesis_DecompressEdgeIndices)(BYTE *idxData, int numIdx, bool bigEndian);
	//if vuMem points to a NULL pointer, it will be auto-allocated. auto-allocated vuMem must be freed with Noesis_UnpooledFree.
	//cbs may be NULL if no callback handlers are desired.
	void				(*Noesis_PS2_ProcessVIFCodes)(BYTE **vuMem, BYTE *vifData, int dataSize, CArrayList<ps2VifULog_t> &unpackLog, ps2VifCallback_t *cbs, int numCbs, ps2VifExParams_t *pParams);
	//commits ps2 draw lists to the active rpg context and empties the draw list. provided material index should correspond to material name "mat%i" (where %i is the material index) and to a texture in the texList.
	//this is because uv's must be fit to texture dimensions.
	//bindBones acts as a map for vertex weights into a main bone list.
	//resv must be NULL.
	void				(*Noesis_PS2_RPGCommitLists)(ps2DrawLists_t &dl, int materialIdx, CArrayList<noesisTex_t *> &texList, int *bindBones, int numBones, void *resv);
	//gets the component index for a given chunk.
	int					(*Noesis_PS2_GetComponentIndex)(ps2GeoChunkHdr_t *chunk);
	//handles a chunk. may automatically call Noesis_PS2_RPGCommitLists, if a new primitive type is encountered.
	//parameters have the same meaning as with Noesis_PS2_RPGCommitLists.
	//chunkOfs must be the offset of a ps2GeoChunkHdr_t in the fileBuffer, and will automatically be incremented past the chunk as it's handled.
	//resv must be NULL.
	bool				(*Noesis_PS2_RPGHandleChunk)(BYTE *fileBuffer, int &chunkOfs, int materialIdx, int *bindBones, int numBones,
							ps2DrawLists_t &dl, CArrayList<noesisTex_t *> &texList, void *resv);

	//new in Noesis 3.991
	bool				(*rpgGenerateStripListsEx)(const WORD *idx, int numIdx,
							sharedStripList_t **stripOut, int &stripNum, bool doStitch/*true*/, unsigned int restart/*0xFFFFFFFF*/, unsigned int cacheSize/*16*/,
							unsigned int minStripSize/*0*/, bool listsOnly/*false*/);

	//new in Noesis 3.994
	//decodes and encodes raw image data from a format string. returned pointer must be freed with Noesis_UnpooledFree.
	//see python documentation for info on format string. (rapi.imageDecodeRaw)
	BYTE				*(*Noesis_ImageDecodeRaw)(BYTE *in, int inSize, int inW, int inH, char *fmtStr);
	BYTE				*(*Noesis_ImageEncodeRaw)(BYTE *in, int inSize, int inW, int inH, char *fmtStr, int *outSize);
	//tries reading a file relative to the path of the input file
	BYTE				*(*Noesis_InputReadFile)(const char *filename, int *sizeOut); //you must free the pointer returned by Noesis_ReadFile with Noesis_UnpooledFree! (unless it's NULL)
	BYTE				*(*Noesis_InputReadFileW)(const wchar_t *filename, int *sizeOut); //you must free the pointer returned by Noesis_ReadFile with Noesis_UnpooledFree! (unless it's NULL)
	//creates a bone map. returns the number of bones in the bonemap, and if bmap is non-null, it is filled in by a Noesis_UnpooledAlloc'd bone list
	//if bmap is non-null, the weight bone indices are modified in-place to reference the map.
	int					(*Noesis_CreateBoneMap)(newVertWeight_t *weights, int numWeights, int **bmap); //if *bmap comes back non-null, it must be freed with Noesis_UnpooledFree

	//new in Noesis 3.996
	//returns -1 if no preview model loaded
	int					(*Noesis_GetSelectedPreviewModel)(void);
	//queries time with highprecision performance counters
	unsigned long		(*Noesis_GetTimeMS)(void);
	//functions the same as rpgTransformModel, but accepts base-relative matrices instead to be applied directly for skin transforms
	void				(*rpgSkinModel)(sharedModel_t *pmdl, modelMatrix_t *skinMats);
	//registers a button. rgbUp and rgbDown may be NULL to use the default user tool image. all resv parameters must be NULL.
	int					(*Noesis_RegisterUserButton)(BYTE *rgbUp, BYTE *rgbDown, int rgbW, int rgbH, void (*useFunc)(void), bool (*checkVisible)(void *resv),
								   char *helpText, void *resvA, void *resvB, void *resvC, void *resvD);

	//new in Noesis 3.997
	//copies transformed verts from the internal model. returns true on success, false on failure. will fail if internal geometry is no longer identical to geometry in the shared model.
	bool				(*Noesis_CopyInternalTransforms)(sharedModel_t *pmdl);

	//new in Noesis 4.0
	//sets an expression system variable
	void				(*Express_SetEVar)(const char *varName, const char *value);
	//sets an expression system function handler
	bool				(*Express_SetUserFunc)(const char *fnName, void *userData, float (*fnHandler)(rexp_t *rex, rexp_t **args, int numArgs, noeRAPI_s *rapi, void *userData, void *resv));
	//parses an expression
	rexp_t				*(*Express_Parse)(const char *expr);
	//evaluates an expression
	float				(*Express_Evaluate)(rexp_t *rex);
	//set the user-data pointer on an expression object
	void				(*Express_SetUserData)(rexp_t *rex, void *userData);
	//get the user-data pointer
	void				*(*Express_GetUserData)(rexp_t *rex);
	//get the original expression string
	char				*(*Express_GetString)(rexp_t *rex);
	//allocate material expressions. resv must be NULL.
	noesisMatExpr_t		*(*Noesis_AllocMaterialExpressions)(void *resv);

	//new in Noesis 4.02
	int					(*Noesis_GetPRSSize)(BYTE *srcBuf, DWORD srcSize);
	int					(*Decomp_PRS)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);

	//new in Noesis 4.04
	void				*(*Noesis_ExportArchiveFileOpenEx)(char *filename, char *mode, bool checkOverwrite, wchar_t *pOutPath);

	//new in Noesis 4.06
	//these function identically to their non-safe counterparts, but allow you to provide a buffersize.
	//when using rpgCommitTrianglesSafe instead of rpgCommitTriangles, indices will be bounds-checked against actual buffer sizes,
	//and the function will return a negative value if anything is out of bounds.
	void				(*rpgBindPositionBufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int bufferSize);
	void				(*rpgBindNormalBufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int bufferSize);
	void				(*rpgBindTangentBufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int bufferSize);
	void				(*rpgBindUV1BufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int bufferSize);
	void				(*rpgBindUV2BufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int bufferSize);
	void				(*rpgBindColorBufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int numChannels, int bufferSize);
	void				(*rpgBindBoneIndexBufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int numWeightsPerVert, int bufferSize);
	void				(*rpgBindBoneWeightBufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int numWeightsPerVert, int bufferSize);
	void				(*rpgFeedMorphTargetPositionsSafe)(void *data, rpgeoDataType_e dataType, int stride, int bufferSize);
	void				(*rpgFeedMorphTargetNormalsSafe)(void *data, rpgeoDataType_e dataType, int stride, int bufferSize);
	int					(*rpgCommitTrianglesSafe)(void *idxData, rpgeoDataType_e dataType, int numIdx, rpgeoPrimType_e primType, bool usePlotMap);

	//New in Noesis 4.074
	noesisExtTexRef_t	*(*Noesis_AllocTexRefs)(char *diffuse, char *normal, char *specular, char *opacity, char *bump, char *env);

	//New in Noesis 4.0781
	void				(*Image_DXT_RemoveFlatFractionBlocks)(BYTE *data, int dataSize, int blockSize, int noeTexFmt);

	//New in Noesis 4.0783
	//boneRefMapSize is the number of entries in the bone reference map (not the size in bytes)
	void				(*rpgSetBoneMapSafe)(int *boneRefMap, int boneRefMapSize);

	//New in Noesis 4.079
	noesisTex_t			*(*Noesis_TextureAllocEx)(char *filename, int w, int h, BYTE *data, int dataLen, int type, int flags, int mipCount);

	//New in Noesis 4.0799
	void				(*Image_InterpolatedSampleFloat)(const float *data, int dataW, int dataH, int dataComponents,
									   float fracX, float fracY, float *dst, bool clamped);

	//New in Noesis 4.081
	//operates on the same scale as Noesis_GetTimeMS, but useful as a rendering timer.
	//this timer only increments once per frame, and should be used for time-based rendering since your rendering code may be called
	//more than once per frame for stereoscopic rendering.
	unsigned long		(*Noesis_GetFrameTime)(void);
	void				(*Noesis_GetSplineSetBounds)(const noesisSplineSet_t *ss, float *mins, float *maxs);
	const float			*(*Noesis_SplineLastOut)(const noesisSpline_t *spline, int idx);
	const float			*(*Noesis_SplineLastPos)(const noesisSpline_t *spline, int idx);
	//creates a mesh around a spline for simple visualization
	//for every returned pointer (pos, nrm, etc.) you must free it with Noesis_UnpooledFree.
	void				(*Noesis_SplineToMeshBuffers)(const noesisSplineSet_t *ss, const modelMatrix_t *transform, float fStep, float size, int subDivs, int &numVerts, int &numIndices, float **posOut, float **nrmOut, float **uvOut, int **idxOut, void *resvA, void *resvB);
	//generates a normal map (rgba32) from a heightmap. (rgba32) r+b+g added together is used to specify height.
	//1.0 is a good default for both scale values. returned pointer must be Noesis_UnpooledFree'd.
	BYTE				*(*Image_NormalMapFromHeightMap)(const BYTE *heightRGBA, const int heightW, const int heightH, const float heightScale, const float texelScale);

	//New in Noesis 4.0828
	void				(*rpgUnifyBinormals)(bool flip);

	//New in Noesis 4.0836
	bool				(*rpgActiveContextIsValid)(void);

	//inflate/deflate with explicit window bits
	int					(*Decomp_Inflate2)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize, int windowBits);
	int					(*Compress_Deflate2)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize, int level, int windowBits);
	int					(*Noesis_GetInflatedSize2)(BYTE *srcBuf, DWORD srcSize, int windowBits);

	//New in Noesis 4.0843
	//this function operates on all vertices that have been committed from immediate mode or buffers (via rpgEnd or rpgCommitTriangles)
	//it does effectively the same thing as rpgSkinModel, but doesn't require a constructed shared model to operate.
	//param may be NULL.
	void				(*rpgSkinPreconstructedVertsToBones)(modelBone_t *bones, int numBones, skinVertsToBonesParam_t *param); //skins committed verts to bones

	//New in Noesis 4.0844
	//returns true if handler is being invoked for export instead of preview or instanced module data load
	bool				(*Noesis_IsExporting)(void);

	//New in Noesis 4.0863
	//rearranges image data in morton order
	void				(*Image_MortonOrder)(BYTE *src, BYTE *dst, int width, int height, int bytesPerPixel, bool toMorton);

	//New in Noesis 4.0866
	//same as rpgAnimFromBonesAndMatsFinish, except it generates from a keyframed animation instead of matrices
	noesisAnim_t		*(*Noesis_AnimFromBonesAndKeyFramedAnim)(modelBone_t *bones, int numBones, noeKeyFramedAnim_t *kfAnim, bool doPostTransforms);

	//New in Noesis 4.087
	//rearranges image data in morton order
	//flags:
	//1 - toMorton
	//2 - swapXY
	//resv must be 0.
	void				(*Image_MortonOrderEx)(BYTE *src, BYTE *dst, int width, int height, int bytesPerPixel, int flags, TResvInt resv);

	//New in Noesis 4.0875
	int					(*rpgGetVertexCount)(void);
	int					(*rpgGetTriangleCount)(void);

	//New in Noesis 4.0897
	int					(*rpgGetMorphBase)(void);
	void				(*rpgSetMorphBase)(int morphBase);

	//New in Noesis 4.092
	int					(*Noesis_StrPoolGetOfsIfInPool)(noeStringPool_t *pool, char *str);

	//New in Noesis 4.0955
	//swizzle 4-byte rgba pixel based on flags. resv must be 0 or no changes will occur to p.
	void				(*Noesis_SwizzleNormalPixEx)(BYTE *p, unsigned int flags, TResvInt resv);

	//New in Noesis 4.096
	//grabs a snapshot of internal mesh properties
	void				(*Noesis_GetMeshInternalProperties)(const void *internalMesh, sharedMeshInternalProperties_t *dst);

	//New in Noesis 4.0961
	void				(*Noesis_UntileImageRAWEx)(BYTE *dst, BYTE *src, int dstSize, int imgW, int imgH, int blockSize, int srcSize, TResvInt resvA, TResvInt resvB);
	void				(*Noesis_UntileImageDXTEx)(BYTE *dst, BYTE *src, int dstSize, int imgW, int imgH, int bytesPerPix, int srcSize, TResvInt resvA, TResvInt resvB);

	//New in Noesis 4.0962
	BYTE				*(*Noesis_ConvertDXTEx)(int w, int h, BYTE *data, DWORD dxtFmt, int dataSize, convertDxtExParams_t *params, TResvInt resvB);

	//New in Noesis 4.0965
	//provides user-named vertex data when using immediate mode.
	//if the data should be treated as per-instance (meaning it's set only once for a whole stream of verts),
	//use the RPGVUFLAG_PERINSTANCE flag.
	void				(*rpgVertUserData)(const char *name, const void *data, int dataElemSize, int flags);
	//binds a user-named vertex buffer. dataSize is the entire size of the buffer, and dataElemSize is the per-vertex
	//data size, and dataStride is the number of bytes between elements in the buffer.
	//you can use a dataStride of 0 if you want to have a single chunk of data used for every vertex. (per-instance mode)
	//to unbind a user-named buffer, call rpgBindUserDataBuffer(name, NULL, 0, 0, 0, 0). to unbind all user-named buffers,
	//call rpgBindUserDataBuffer(NULL, NULL, 0, 0, 0, 0) or use rpgClearBufferBinds.
	//flags is currently unnecessary, as RPGVUFLAG_PERINSTANCE is indicated by a stride of 0. if RPGVUFLAG_PERINSTANCE is passed as a flag,
	//stride will be forced to 0.
	void				(*rpgBindUserDataBuffer)(const char *name, const void *data, int dataSize, int dataElemSize, int dataStride, int flags);

	//New in Noesis 4.0968
	bool				(*Noesis_SimulateDragAndDrop)(const char *dragDropFile);

	//New in Noesis 4.0969
	//processes export commands
	void				(*Noesis_ProcessCommands)(const char *commands);

	//New in Noesis 4.0974
	//under construction
	void				(*rpgWeldVerts)(float threshold, int flags, void *resv);

	//New in Noesis 4.0977
	//returned pointer must be freed with Noesis_UnpooledFree. when freed, all data and stream pointers therein will also be invalidated.
	decompDrawSegList_t	*(*Noesis_DecompDefaultDrawSegs)(const BYTE *data, const int dataSize, const int drawSegCount, const int drawSegOfs, const unsigned __int64 ver, const bool isBigEndian);
	bool				(*Noesis_WritePCMWaveFile)(wchar_t *fileName, const void *data, int dataSize, int bitRate, int sampleRate, int channelCount);

	//New in Noesis 4.098
	bool				(*Noesis_DecodeADPCMBlock)(const unsigned char *pSrc, unsigned char *pDst, int bitsPerSample, int samplesToDecode, int lshiftAmount,
									double &sampleOld, double &sampleOlder, double oldFactor, double olderFactor, int bitOfs, int bitStride,
									double sampleScale, int flags, void *resv);
	int					(*Noesis_GetExportingModelSetCount)(void);
	noesisModel_t		*(*Noesis_GetExportingModel)(int setIndex);

	void				(*rpgVertUniqueIndex)(int idx);

	//pass NOESIS_PLUGINAPI_VERSION for apiVersion. this will allow future API revisions to return NULL if the M68000 API has been changed in a way
	//which breaks binary compatibility. resv must be NULL.
	CNoeSharedM68000	*(*Noesis_CreateM68000)(int apiVersion, void *resv);
	void				(*Noesis_DestroyM68000)(CNoeSharedM68000 *pCpu);

	//New in Noesis 4.143
	//if return value is non-NULL, it should be freed via Noesis_UnpooledFree unless pDestData was provided.
	//if pDestData is provided, its size must be >= sourceElemCount * 3 or * 4 if wBits is non-0.
	//sourceElemStride specifies number of bytes between each source element (normal), sourceElemCount specifies total number of source elements.
	//each element's uint32 will be endian-swapped as it's processed if sourceIsBigEndian is true.
	//if nBits values are negative, source component will be treated as a signed fixed point value.
	//if nBits values are positive, source component will be treated as unsigned, then scale and biased (* 2 - 1) to -1..1.
	//returned data will be a 3 or 4 component array of normals of size sourceElemCount. 3 components if wBits is 0, otherwise 4.
	//xyz of returned values will be normalized. w value, if present, will be untouched.
	float *(*Noesis_DecodeNormals32)(void *pDestData, const void *pSourceData, const int sourceElemStride, const int sourceElemCount, const bool sourceIsBigEndian,
									const int xBits, const int yBits, const int zBits, const int wBits);

	//New in Noesis 4.144
	//same as Image_GetTexRGBA, but returns HDR color data in the form of rgbaF128. values sourced from non-HDR formats
	//will be in the range of 0..1. HDR pixel data may contain any value.
	//if the return values is non-NULL and shouldFree is true, YOU MUST FREE THE POINTER with Noesis_UnpooledFree.
	float				*(*Image_GetTexRGBAFloat)(noesisTex_t *tex, bool &shouldFree);

	//pResvA/pResvB must be NULL. returns a pool-allocated HDR tex data structure.
	SNoeHDRTexData		*(*Noesis_AllocHDRTexStructure)(void *pData, const int dataLen, const ENoeHdrTexFormat hdrTexFormat, void *pResvA, void *pResvB);

	//returns NULL outside of image export handlers, otherwise a pointer to the texture who owns the image data being written, if any.
	noesisTex_t			*(*Noesis_GetTextureBeingWritten)(void);

	//if return is non-NULL, must be freed with Noesis_UnpooledFree.
	//returned data will be in rgb48 instead of rgb24 form. it will be the caller's responsibility to scale/bias/interpret that data appropriately.
	unsigned short		*(*Image_JPEG_ReadDirect)(const unsigned char *pFileBuffer, const int bufferLen, const int dataPrecision,
													int *pWidthOut, int *pHeightOut, int *pComponentsOut,
													int *pPrecisionOut, void *pResv);


	//lzo 1y
	int					(*Decomp_LZO_1y)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);

	//lzo 1x with buffer overrun checks
	int					(*Decomp_LZOSafe_1x)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);
	//lzo 1y with buffer overrun checks
	int					(*Decomp_LZOSafe_1y)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);

	BYTE				*(*Image_DecodePVRTCEx)(BYTE *dataPtr, int sz, int srcW, int srcH, int bitsPP, int decodeFlags);

	bool				(*Noesis_FillOutPCMWaveHeader)(void *pDst, int *pDstSize, const int dataSize, const int bitRate, const int sampleRate, const int channelCount);

	int					(*Decomp_LZ4)(BYTE *srcBuf, BYTE *dstBuf, DWORD srcSize, DWORD dstSize);

	void				(*Image_InterpolatedSampleEx)(BYTE *img, int w, int h, float fracX, float fracY, float *dst, int flags);

	BYTE				*(*Image_CreateDDSFromDXTDataEx)(BYTE *data, int dataSize, int w, int h, int numMips, int dxtFmt, int cubeFlag, int otherFlags, int dxgiFmt, int *sizeOut, void *pResv);

	BYTE				*(*Image_GetTexRGBAOffset)(noesisTex_t *tex, bool &shouldFree, int offset);
	float				*(*Image_GetTexRGBAFloatOffset)(noesisTex_t *tex, bool &shouldFree, int offset, bool useHdr);

	int					(*Image_GetMipSize)(const noesisTex_t *tex, const int mipIndex, const bool useHdr);

	//checks all texture and relative paths for a given file, and returns the file contents in a buffer if found. if non-NULL is returned, free with Noesis_UnpooledFree.
	BYTE				*(*Noesis_LoadFileOnTexturePaths)(int *pSizeOut, const char *pFilename);

	void				(*Noesis_SetModelCustomData)(noesisModel_t *pMdl, CNoeCustomDataList &customDataList);

	//accepts one of NOE_ENCODEDXT_* values as encodeType. dataPixelStride is the number of bytes between each pixel. pResv must be NULL. pSizeOut may be NULL if return size is not needed.
	//the dxt buffer that's returned must be freed via Noesis_UnpooledFree. the buffer will also be padding out for dxt block alignment if the source image dimensions are not aligned to
	//block size.
	//for NOE_ENCODEDXT_BC4, only the first 2 (red/green) channels are used to encode the 2 "alpha" blocks.
	BYTE				*(*Noesis_EncodeDXT)(int w, int h, const BYTE *pData, int dataPixelStride, int encodeType, int *pSizeOut, void *pResv);

	//typical filtering for mip generation. assumes rgba32 src/dst. if dstW != srcW/2 || dstH != srcH/2, or either dimension is not aligned to 2, falls back to ResampleImageBilinear.
	void				(*Noesis_ResampleImageBox)(const unsigned char *pSrc, int srcW, int srcH, unsigned char *pDst, int dstW, int dstH);

	SMorphGroupInfo		*(*Noesis_GetMorphGroupInfoFromList)(const CArrayList<SMorphGroup> &morphGroups);
	void				(*rpgSetExData_MorphGroups)(SMorphGroupInfo *pGroupInfo);

	//will be ecb if pIV is NULL.
	int					(*Decrypt_AES)(const unsigned char *pSrc, unsigned char *pDst, int size, const unsigned char *pKey, const unsigned char *pIV, int keyBits);
	int					(*Encrypt_AES)(const unsigned char *pSrc, unsigned char *pDst, int size, const unsigned char *pKey, const unsigned char *pIV, int keyBits);

	//does not require a valid rpg context (static call)
	int					(*rpgCalculateGenus)(int vertexCount, int indexCount, const void *pIndexData, const rpgeoDataType_e indexDataType, const int triangleStride, const bool allowBoundaryEdges);

	//does not require a valid rpg context (static call)
	//destType must be RPGEODATA_DOUBLE or RPGEODATA_FLOAT. pParamOut must be large enough to store (4 or 8) * 2 * vertexCount. returns < 0 on failure.
	int					(*rpgParameterize2D)(void *pParamOut, rpgeoDataType_e destType, rpgeoParamertizationType_e paramType, int vertexCount, int indexCount,
											const void *pIndexData, rpgeoDataType_e indexDataType, int triangleStride,
											const void *pPosData, rpgeoDataType_e posDataType, int posStride);

	int					(*Noesis_GetLZNT1Size)(const unsigned char *pSrc, const unsigned int srcSize);
	int					(*Decomp_LZNT1)(const unsigned char *pSrc, unsigned char *pDst, const unsigned int srcSize, const unsigned int dstSize);

	bool				(*Noesis_LoadTexByHandlerMulti)(CArrayList<noesisTex_t *> &noeTex, BYTE *srcBuf, DWORD srcSize, char *ext);

	void				(*Noesis_ParseInstanceOptions)(const char *pOptions);

	void				(*rpgSkinPreconstructedVertsToTransforms)(modelMatrix_t *pTransforms, const int transformCount, skinVertsToBonesParam_t *param); //skins committed verts to bones

	//pPath should not include an extension, it will be selected by the target texture exporter
	bool				(*Noesis_ExportTextureInDesiredFormat)(wchar_t *pOutPath, const wchar_t *pPath, noesisTex_t *pTex);

	CArrayList<noesisTexRef_t>		&(*Noesis_LoadTexturesForModelEx)(sharedModel_t *pmdl, const int flags);

	void				(*Noesis_GetPreviewAngleTransform)(modelMatrix_t *pMat);

	void				(*Noesis_TurnAnimNameIntoAbsPath)(wchar_t *pPathOut, const char *pAnimName, const char *pExtension);

	bool				(*Noesis_GetLeftHandedPreference)(void *pResv);

	//this name is allowed to be duplicated between meshes, and is to be used as a guide by exporters for formats which support meshes with multiple materials.
	//in this case, the exporter may elect to re-group meshes sharing the same source name into a single mesh.
	void				(*rpgSetSourceName)(const char *pSourceName);

	//pResvA/pResvB must be NULL. returns a pool-allocated palette tex data structure.
	SNoePalData			*(*Noesis_AllocPalTexStructure)(void *pData, const int dataLen, const int colorCount, const ENoePalFormat palTexFormat, void *pResvA, void *pResvB);

	//returns NULL if there is no palette data attached to the texture.
	//Noesis_UnpooledFree must be used to free returned pointer if non-NULL.
	BYTE				*(*Noesis_GetTexPalRgba)(int *pColorCountOut, const noesisTex_t *pTex, const int minColors);

	//as above, but both ppPalOut and ppIndicesOut must be freed if function returns true;
	bool				(*Noesis_GetTexPalIndicesRgba)(BYTE **ppPalOut, BYTE **ppIndicesOut, int *pColorCountOut, const noesisTex_t *pTex, const int minColors);

	void				(*rpgSetUVXScaleBias)(float *scale, float *bias, int index);

	//uvIndex 0 = "rpgBindUV1Buffer"
	//uvIndex 1 = "rpgBindUV2Buffer"
	//uvIndex 2+ = uvx buffers
	//elemCount will generally be 2 for "uv" coordinates, but up to 4 is allowed.
	void				(*rpgBindUVXBuffer)(void *data, rpgeoDataType_e dataType, int stride, int uvIndex, int elemCount);
	void				(*rpgBindUVXBufferSafe)(void *data, rpgeoDataType_e dataType, int stride, int uvIndex, int elemCount, int bufferSize);

	//functions as Noesis_ExportTextureInDesiredFormat, but allows multiple textures for applicable formats. for non-applicable formats, only the first texture will be exported.
	//also supplies a pForceExt option, which if non-NULL, will override the export handler with a handler matching the given extension.
	bool				(*Noesis_ExportTextureInDesiredFormatMulti)(wchar_t *pOutPath, const wchar_t *pPath, noesisTex_t *pTex, const int texCount, const char *pForceExt);

	//pPalData and pPalIndices may each be NULL unless you want to write an 8-bit paletted png to memory.
	//return value >= 0 indicates success.
	int					(*Noesis_PNG_WriteToMemory)(unsigned char *pDest, const int destSize, const unsigned char *pRgba, const int width, const int height, const bool writeAlpha, const unsigned char *pPalData, const unsigned char *pPalIndices);

	//handle with care, doesn't respect refcount and doesn't check shouldFreeData
	void				(*Noesis_ForceFreeTextureData)(noesisTex_t *pTex);

	void				(*rpgFeedMorphName)(const char *pName);

	//meant to be used with "GData" stuff
	bool				(*Noesis_SetSelectedModel)(const int modelIndex);

	//can be safely used to preserve pointer in module instance data through successive plugin calls, e.g. check -> load.
	//note that if calls aren't directly successive, data may be stomped by another plugin.
	void				(*Noesis_SetPluginUserPtr)(void *pUserPtr);
	void				*(*Noesis_GetPluginUserPtr)();
	//pointer to a scratch buffer meant to be preserved between plugin calls under a given instance
	void				*(*Noesis_GetPluginUserScratchBuffer)(int size);

	//reserved, do not call.
	int					(*resvA)(void);
	int					(*resvB)(void);
	int					(*resvC)(void);
	int					(*resvD)(void);
	int					(*resvE)(void);
	int					(*resvF)(void);
	int					(*resvG)(void);
} noeRAPI_t;

typedef struct noePluginInfo_s
{
	char				pluginName[64];
	char				pluginDesc[512];

	BYTE				resv[512];
} noePluginInfo_t;

//optName, optDescr, and handler must not be NULL or NPAPI_AddTypeOption will fail.
//storeSize must also be > 0.
#define OPTFLAG_WANTARG		(1<<0) //if not set in flags, arg passed will always be NULL
typedef struct addOptParms_s
{
	char			*optName; //MUST START WITH -, e.g. "-maxbones"
	char			*optDescr; //a plain-text description of what this option does
	//your handler should return true if the supplied argument is handled, otherwise false
	bool			(*handler)(const char *arg, unsigned char *store, int storeSize);
	unsigned char	*shareStore; //allows you to provide a store buffer, otherwise one is allocated for you.
	int				storeSize; //the amount of space given for your persistent argument buffer. this memory will be zeroed along with all other model processing options.
	int				flags;
	void			(*storeReset)(unsigned char *store, int storeSize); //optional, allows you to dictate the default contents of your store memory
	TResvInt		resv[16];
} addOptParms_t;

typedef enum
{
	NOEUSERVAL_NONE = 0,
	NOEUSERVAL_STRING,
	NOEUSERVAL_FLOAT,
	NOEUSERVAL_INT,
	NOEUSERVAL_BOOL,
	NOEUSERVAL_FILEPATH,
	NOEUSERVAL_FOLDERPATH,
	NOEUSERVAL_SAVEFILEPATH,
	NOEUSERVAL_RESV1,
	NOEUSERVAL_RESV2,
	NOEUSERVAL_RESV3,
	NOEUSERVAL_RESV4,
	NOEUSERVAL_RESV5,
	NOEUSERVAL_RESV6,
	NOEUSERVAL_RESV7,
	NOEUSERVAL_RESV8,
	NUM_NOE_USER_VALS,
} noeUserValType_e;

#define MAX_USERINPUT_BUFFER_SIZE	32768
typedef struct noeUserPromptParam_s
{
	char				*titleStr; //dialog title string, leave NULL for default
	char				*promptStr; //user instruction string, leave NULL for default
	char				*defaultValue; //default value field (NULL for blank)
	noeUserValType_e	valType; //type of value the user is being prompted for
	char				*(*valHandler)(void *valIn, noeUserValType_e valInType); //custom handler, returns NULL if input is acceptable, otherwise error string. leave this NULL for no custom handling/validation.
									//valIn will vary based on valType in this struct. STRING = char pointer, FLOAT = float pointer, INT = int pointer, BOOL = bool pointer
	BYTE				valBuf[MAX_USERINPUT_BUFFER_SIZE];
	TResvInt			resv[16];
} noeUserPromptParam_t;

#define NOE_DIRENTRY_FLAG_NONE		0
#define NOE_DIRENTRY_FLAG_ISDIR		(1 << 0)
struct SNoeDirEntry
{
	wchar_t mPath[MAX_NOESIS_PATH];
	int mFlags;
	unsigned __int64 mFileSize;
};

typedef struct noePluginFn_s
{
	//=========================
	//engine-provided functions
	//=========================

	//note that rapi handles can be passed from multiple different and concurrent noesis core instances. you should always deal with the local rapi
	//function set.

	//returns a handle to a new type for this module.
	int			(*NPAPI_Register)(char *typeDesc, char *extList);
	//sets the callback for "is this data of this type?"
	void		(*NPAPI_SetTypeHandler_TypeCheck)(int th, bool (*dataCheck)(BYTE *fileBuffer, int bufferLen, noeRAPI_t *rapi));
	//sets the callback to load model data
	void		(*NPAPI_SetTypeHandler_LoadModel)(int th, noesisModel_t *(*loadModel)(BYTE *fileBuffer, int bufferLen, int &numMdl, noeRAPI_t *rapi));
	//sets the callback to write model data
	void		(*NPAPI_SetTypeHandler_WriteModel)(int th, bool (*writeModel)(noesisModel_t *mdl, RichBitStream *outStream, noeRAPI_t *rapi));
	//sets the callback to load pixel data
	void		(*NPAPI_SetTypeHandler_LoadRGBA)(int th, bool (*loadRGBA)(BYTE *fileBuffer, int bufferLen, CArrayList<noesisTex_t *> &noeTex, noeRAPI_t *rapi));
	//sets the callback to write pixel data
	void		(*NPAPI_SetTypeHandler_WriteRGBA)(int th, int (*writeRGBA)(char *fileName, BYTE *pix, int w, int h, noeRAPI_t *rapi));
	//sets the callback to write animation data
	void		(*NPAPI_SetTypeHandler_WriteAnim)(int th, void (*writeAnim)(noesisAnim_t *anim, noeRAPI_t *rapi));
	//sets the callback to extract an archive
	void		(*NPAPI_SetTypeHandler_ExtractArc)(int th, bool (*extractArc)(BYTE *fileBuffer, int bufferLen, bool justChecking, noeRAPI_t *rapi));

	//gets the handle for the main window. make sure to check if this is non-0, as it can be called before the main Noesis window is created.
	HWND		(*NPAPI_GetMainWnd)(void);

	//gets the noesis api version
	int			(*NPAPI_GetAPIVersion)(void);

	//sets the callback to extract an archive in direct-stream mode (new in Noesis 2.5)
	void		(*NPAPI_SetTypeHandler_ExtractArcStream)(int th, bool (*extractArcStream)(wchar_t *filename, __int64 len, bool justChecking, noeRAPI_t *rapi));

	//allows you to add advanced/commandline option hooks for your program (new in Noesis 3.22)
	//if successful, returns a pointer to the variable store for the option.
	void		*(*NPAPI_AddTypeOption)(int th, addOptParms_t *optParms);

	//New in Noesis 3.46
	//dst is assumed to be at least MAX_NOESIS_PATH wchars
	void		(*NPAPI_GetExecutablePath)(wchar_t *dst);
	//returns number of files found, calls fileCallback (if non-null) for each file found. example:
	//NPAPI_EnumerateFiles(noesisPathDir, L"plugins", L"*.dll", myPathHandler);
	int			(*NPAPI_EnumerateFiles)(const wchar_t *basePath, const wchar_t *searchPath, const wchar_t *searchType,
								bool (*fileCallback)(wchar_t *filePath));
	//plugins are allowed a single critical section to avoid processing-previewing conflicts. resv MUST BE 0 for these calls.
	void		(*NPAPI_EnterCritical)(TResvInt resv);
	void		(*NPAPI_LeaveCritical)(TResvInt resv);
	//allows a plugin to invoke the debug log. resv must be 0.
	void		(*NPAPI_PopupDebugLog)(TResvInt resv);
	//logs a debug string
	void		(*NPAPI_DebugLogStr)(char *str);

	//New in Noesis 3.84
	//this handler intercepts *all* image exports for the active export, and feeds them to the batch write handler. This can be useful for compiling animations from an array of textures.
	void		(*NPAPI_SetTypeHandler_WriteRGBABatch)(int th, bool (*writeRGBABatch)(char *fileName, noesisTex_t *textures, int numTex, noeRAPI_t *rapi));

	//New in Noesis 3.89
	//creates and displays a user prompt, returns true if the callback handler approved the input and the user hit ok, otherwise false
	//if the return value is true, the contents of the input are stored in the param structure's valBuf, to be casted to the appropriate type based on the value of valType.
	bool		(*NPAPI_UserPrompt)(noeUserPromptParam_t *params);
	//displays a message in a standard dialog prompt
	void		(*NPAPI_MessagePrompt)(wchar_t *msg);
	//returns a handle to a new tool for this module.
	int			(*NPAPI_RegisterTool)(char *toolDesc, int (*toolMethod)(int toolIdx, void *userData), void *userData);
	//returns the user data for a tool by index
	void		*(*NPAPI_GetToolUserData)(int toolIdx);

	//New in Noesis 3.9
	//dst is assumed to be at least MAX_NOESIS_PATH wchars
	void		(*NPAPI_GetScenesPath)(wchar_t *dst);
	//tells noesis to open a new file in the main preview view
	bool		(*NPAPI_OpenFile)(wchar_t *p);

	//New in Noesis 3.93
	//it is recommended that you try to use unique names (for example, prefix all setting names with your plugin name) when reading/writing settings to avoid conflicts with other plugins.
	//writes a chunk of binary for a user plugin
	bool		(*NPAPI_UserSettingWrite)(wchar_t *name, BYTE *src, int srcLen);
	//reads a chunk of binary for a user plugin
	bool		(*NPAPI_UserSettingRead)(wchar_t *name, BYTE *dst, int dstLen);
	//performs a task on a job thread. jobHandle may be NULL if you do not need to query for the completion of the job.
	void		(*NPAPI_Threads_DoJob)(void (*job)(void *userData), void *userData, jobHandle_t *jobHandle);
	//checks if a job is done yet
	bool		(*NPAPI_Threads_JobDone)(jobHandle_t *jobHandle, bool stall);
	//runs the windows message pump. should only be used by tools and when you know what you're doing.
	void		(*NPAPI_DoPump)(void);
	//returns internal afx app handle. again should only be used when you know what you're doing.
	void		*(*NPAPI_GetAfxWinApp)(void);

	//New in Noesis 3.97
	//will return null if memory maps are disabled in preferences
	NoeMappedFile	*(*NPAPI_AllocMappedFile)(unsigned __int64 size, wchar_t *name);
	void		(*NPAPI_DestroyMappedFile)(NoeMappedFile *mf);
	//returns engine-allocated rgba buffer
	BYTE		*(*NPAPI_LoadImageRGBA)(wchar_t *filename, int *wOut, int *hOut);
	//returns true if write was successful
	bool		(*NPAPI_SaveImageRGBA)(wchar_t *filename, BYTE *rgba, int w, int h, char *options);
	//engine-allocated pointers must be freed with NPAPI_EngineFree
	void		*(*NPAPI_EngineAlloc)(size_t size);
	void		(*NPAPI_EngineFree)(void *ptr);
	//module instantiation can be performed from tools. always remember to free the module when you're finished with it, or you will bog Noesis down.
	int			(*NPAPI_InstantiateModule)(void (*logFunction)(char *str));
	void		(*NPAPI_FreeModule)(int modIdx);
	noeRAPI_t	*(*NPAPI_GetModuleRAPI)(int modIdx);
	//fills in a buffer (expected to be MAX_NOESIS_PATH wchars) with the currently selected file in the viewer. "" is no selection is made.
	void		(*NPAPI_GetSelectedFile)(wchar_t *dst);

	//New in Noesis 3.994
	//sets implicit export options for the format
	void		(*NPAPI_SetTypeExportOptions)(int th, char *options);

	//New in Noesis 3.996
	//sets menu help text for a tool entry
	void		(*NPAPI_SetToolHelpText)(int toolIdx, char *helpText);
	//gets a pointer to the rapi interface for the active preview model. NULL if nothing is loaded.
	noeRAPI_t	*(*NPAPI_GetPreviewRAPI)(void);
	//register a visualizer
	int			(*NPAPI_RegisterVisualizer)(void);
	//set visualizer pre model render callback
	void		(*NPAPI_Visualizer_SetPreRender)(int vh, bool (*preRender)(int vh, void *resv, noeSharedGL_t *ngl));
	//set visualizer post model render callback
	void		(*NPAPI_Visualizer_SetPostRender)(int vh, void (*postRender)(int vh, modelMatrix_t *skinMats, int numSkinMats, float animFrame, void *resv, noeSharedGL_t *ngl));
	//set visualizer callback for after preview model is loaded (called after successful load)
	void		(*NPAPI_Visualizer_SetPreviewLoaded)(int vh, void (*previewLoaded)(int vh));
	//set visualizer callback for when preview is closed (called before close)
	void		(*NPAPI_Visualizer_SetPreviewClose)(int vh, void (*previewClose)(int vh));
	//set visualizer callback for when preview is reset (f12/middlemouse)
	void		(*NPAPI_Visualizer_SetPreviewReset)(int vh, void (*previewReset)(int vh));
	//set visualizer callback for new input
	void		(*NPAPI_Visualizer_SetInput)(int vh, bool (*input)(int vh, int x, int y, int buttonFlags, void *resv));
	//checks or unchecks a tool's menu item
	void		(*NPAPI_CheckToolMenuItem)(int toolIdx, bool checked);

	//New in Noesis 4.02
	//gets a program setting. returns NULL if setting doesn't exist.
	char		*(*NPAPI_GetProgramSetting)(char *name);
	//returns true if debug log is open
	bool		(*NPAPI_DebugLogIsOpen)(void);
	//reloads all plugins
	void		(*NPAPI_ReloadPlugins)(void);

	//New in Noesis 4.04
	//dst is assumed to be at least MAX_NOESIS_PATH wchars
	void		(*NPAPI_GetPluginsPath)(wchar_t *dst);
	//returns true if a forced reload is occurring
	bool		(*NPAPI_IsTriggeredPluginReload)(void);
	//dst is assumed to be at least MAX_NOESIS_PATH wchars, or dst may be NULL
	bool		(*NPAPI_GetOpenPreviewFile)(wchar_t *dst);

	//New in Noesis 4.061
	//fills in a buffer (expected to be MAX_NOESIS_PATH wchars) with the currently selected directory in the viewer. "" is no selection is made.
	void		(*NPAPI_GetSelectedDirectory)(wchar_t *dst);

	//New in Noesis 4.066
	cntArray_t	*(*Array_Alloc)(int elementSize, int initialNum);
	void		(*Array_Free)(cntArray_t *ar);
	void		(*Array_SetGrowth)(cntArray_t *ar, bool exponential);
	void		(*Array_QSort)(cntArray_t *ar, int (__cdecl * compareFunc)(const void *a, const void *b));
	void		*(*Array_GetElement)(cntArray_t *ar, int index);
	void		*(*Array_GetElementGrow)(cntArray_t *ar, int index);
	void		(*Array_Append)(cntArray_t *ar, const void *element);
	void		(*Array_RemoveLast)(cntArray_t *ar);
	void		(*Array_Insert)(cntArray_t *ar, const void *element, int index);
	void		(*Array_Remove)(cntArray_t *ar, int index);
	int			(*Array_GetCount)(cntArray_t *ar);
	void		(*Array_Reset)(cntArray_t *ar);
	void		(*Array_Tighten)(cntArray_t *ar);

	cntStream_t	*(*Stream_Alloc)(void *buffer, int size);
	cntStream_t	*(*Stream_AllocFixed)(int size);
	void		(*Stream_Free)(cntStream_t *st);
	void		(*Stream_WriteBits)(cntStream_t *st, const void *buf, int size);
	void		(*Stream_WriteBytes)(cntStream_t *st, const void *buf, int size);
	bool		(*Stream_ReadBits)(cntStream_t *st, void *buf, int size);
	bool		(*Stream_ReadBytes)(cntStream_t *st, void *buf, int size);
	void		(*Stream_WriteBool)(cntStream_t *st, bool val);
	void		(*Stream_WriteInt)(cntStream_t *st, int val);
	void		(*Stream_WriteFloat)(cntStream_t *st, float val);
	void		(*Stream_WriteString)(cntStream_t *st, const char *str, bool noTerminate);
	void		(*Stream_WriteWString)(cntStream_t *st, const wchar_t *str, bool noTerminate);
	bool		(*Stream_ReadBool)(cntStream_t *st);
	int			(*Stream_ReadInt)(cntStream_t *st);
	float		(*Stream_ReadFloat)(cntStream_t *st);
	void		(*Stream_ReadString)(cntStream_t *st, char *str, int maxSize);
	void		*(*Stream_Buffer)(cntStream_t *st);
	int			(*Stream_Size)(cntStream_t *st);
	void		(*Stream_SetOffset)(cntStream_t *st, int offset);
	int			(*Stream_GetOffset)(cntStream_t *st);
	void		(*Stream_SetFlags)(cntStream_t *st, int flags);
	int			(*Stream_GetFlags)(cntStream_t *st);
	void		(*Steam_WriteToFile)(cntStream_t *st, FILE *f);

	//New in Noesis 4.081
	const noesisSplineSet_t	*(*Noesis_GetCharSplineSet)(const char c);

	//New in Noesis 4.084
	//should be some combination of NTOOLFLAG values
	void		(*NPAPI_SetToolFlags)(int toolIdx, int flags);
	int			(*NPAPI_GetToolFlags)(int toolIdx);
	//sets visibility callback. if it's a context item and a file is selected, focusFileName will be the full path to the selected file.
	//otherwise, the value will be NULL.
	//the callback should return 1 if the menu is visible, otherwise 0.
	void		(*NPAPI_SetToolVisibleCallback)(int toolIdx, int (*visibleCallback)(int toolIdx, const wchar_t *focusFileName, void *resvA, void *resvB));
	//returns some combination of NFORMATFLAG values.
	//ext should be the file extension including the dot, e.g. ".png"
	//if the extension is used by multiple format handlers, expect to get flags from all applicable formats.
	//(numHandlers will also be set appropriately if you pass a non-NULL value)
	int			(*NPAPI_GetFormatExtensionFlags)(wchar_t *ext, int *numHandlers);
	//tells noesis to open a new file in the main preview view, then delete it
	bool		(*NPAPI_OpenAndRemoveTempFile)(wchar_t *p);

	//selects a mesh in the data viewer, may be used by tool plugins.
	void		(*NPAPI_SelectDataViewerMesh)(void *internalMdl, void *internalMesh);

	//selects a mesh's material in the data viewer, may be used by tool plugins.
	void		(*NPAPI_SelectDataViewerMeshMaterial)(void *internalMdl, void *internalMesh);

	//selects a bone in the data viewer, may be used by tool plugins.
	void		(*NPAPI_SelectDataViewerBone)(void *internalMdl, modelBone_t *bone);

	void		(*NPAPI_Visualizer_SetRawKeyDownHook)(int vh, bool (*rawKeyDownHook)(int vh, WPARAM wParam, LPARAM lParam));
	void		(*NPAPI_Visualizer_SetRawKeyUpHook)(int vh, bool (*rawKeyUpHook)(int vh, WPARAM wParam, LPARAM lParam));
	void		(*NPAPI_Visualizer_SetOverrideRenders)(int vh, bool (*shouldOverrideRender)(int vh), void (*doOverrideRender)(int vh, noeSharedGL_t *ngl));

	bool		(*NPAPI_FileIsLoadable)(wchar_t *pFilename);

	void		(*NPAPI_DisableFormatByDescription)(char *typeDesc);

	double		(*NPAPI_HighPrecisionTime)(void); //in seconds

	void			(*NPAPI_AddUserExtProc)(const char *pExtName, NOEXFUNCTION pFunction);
	NOEXFUNCTION	(*NPAPI_GetUserExtProc)(const char *pExtName);

	void (*Stream_SetBitOffset)(const int byteOffset, const int bitOffset, cntStream_t *st);
	void (*Stream_GetBitOffset)(int *pByteOffset, int *pBitOffset, cntStream_t *st);

	void (*NPAPI_PumpModalStatus)(const char *pMsg, const float duration);
	void (*NPAPI_ClearModalStatus)(void);

	unsigned int (*NPAPI_GetResourceHandle)(const unsigned int resourceIndex);

	bool (*NPAPI_GetDirectoryList)(CArrayList<SNoeDirEntry> &entries, const wchar_t *pPath);

	//returns 1 for file, 2 for directory, otherwise
	int (*NPAPI_PathExists)(const wchar_t *pPath);
	void (*NPAPI_PathNormalize)(wchar_t *pPath);

	bool		(*Stream_ReadRevBits)(cntStream_t *st, void *buf, int size);

	void (*NPAPI_SetGlobalMemForRAPI)(noeRAPI_t *pRapi);

	void (*NPAPI_SetToolSubMenuName)(int toolIdx, const char *pSubMenuName);

	int (*NPAPI_RegisterVRMenuItem)(const char *pName, void (*pCallback)(int menuItemIndex));
	void (*NPAPI_EnterCustomVRMenuState)(int (*pGetItemCount)(), const char *(*pGetItemText)(const int itemIndex), void (*pItemUsed)(const int itemIndex), void *pResv);
	void(*NPAPI_SetCustomVRMenuItem)(const int itemIndex, void *pResv);

	bool (*NPAPI_IsSupportedFileExtension)(const wchar_t *pPath);

	bool (*NPAPI_OpenDataViewer)();
	void (*NPAPI_CloseDataViewer)();
	bool (*NPAPI_GetDataViewerSetting)(char *pOut, const char *pName);
	bool (*NPAPI_SetDataViewerSetting)(const char *pName, const char *pValue);

	//reserved, do not call.
	int			(*resvA)(void);
	int			(*resvB)(void);
	int			(*resvC)(void);
	int			(*resvD)(void);
	int			(*resvE)(void);
	int			(*resvF)(void);
} noePluginFn_t;

extern noePluginFn_t *g_nfn;
extern mathImpFn_t *g_mfn;


class CNoeCustomData
{
	friend class CNoeCustomDataList;
	friend class CModelEditDlg;
public:
	void SetData(const void *pData, size_t dataSize)
	{
		if (mpData)
		{
			if (!mAllocPooled)
			{
				mpRapi->Noesis_UnpooledFree(mpData);
			}
			mpData = NULL;
		}
		mDataSize = 0;

		if (pData)
		{
			mpData = (mAllocPooled) ?
						mpRapi->Noesis_PooledAlloc(dataSize) :
						mpRapi->Noesis_UnpooledAlloc(dataSize);
			memcpy(mpData, pData, dataSize);
			mDataSize = dataSize;
		}
	}

	const char *GetName() const { return mpName; }
	const char *GetType() const { return mpType; }

	void *GetData() const { return mpData; }
	size_t GetDataSize() const { return mDataSize; }

	CNoeCustomData *GetNext() const { return mpNext; }
	CNoeCustomData *GetPrev() const { return mpPrev; }

private:
	CNoeCustomData(const char *pName, const char *pType, noeRAPI_t *pRapi, bool allocPooled)
		: mpPrev(NULL)
		, mpNext(NULL)
		, mpData(NULL)
		, mDataSize(0)
		, mpRapi(pRapi)
		, mAllocPooled(allocPooled)
	{
		const size_t nameAllocSize = strlen(pName) + 1;
		const size_t typeAllocSize = strlen(pType) + 1;
		mpName = (mAllocPooled) ?
					(char *)mpRapi->Noesis_PooledAlloc(nameAllocSize + typeAllocSize) :
					(char *)mpRapi->Noesis_UnpooledAlloc(nameAllocSize + typeAllocSize);
		mpType = mpName + nameAllocSize;
		strcpy_s(mpName, nameAllocSize, pName);
		strcpy_s(mpType, typeAllocSize, pType);
	}

	~CNoeCustomData()
	{
		if (!mAllocPooled)
		{
			mpRapi->Noesis_UnpooledFree(mpName);
			//don't free mpType, just pointing off the end of mpName
			if (mpData)
			{
				mpRapi->Noesis_UnpooledFree(mpData);
			}
		}
	}

	CNoeCustomData *mpPrev;
	CNoeCustomData *mpNext;

	char *mpName;
	char *mpType;
	void *mpData;
	size_t mDataSize;

	noeRAPI_t *mpRapi;
	bool mAllocPooled;
};

NoeInline CNoeCustomData *CNoeCustomDataList::CreateCustomData(const char *pName, const char *pType, noeRAPI_t *pRapi, bool allocPooled)
{
	void *pDataMem = (allocPooled) ?
						pRapi->Noesis_PooledAlloc(sizeof(CNoeCustomData)) :
						pRapi->Noesis_UnpooledAlloc(sizeof(CNoeCustomData));
	CNoeCustomData *pData = new(pDataMem) CNoeCustomData(pName, pType, pRapi, allocPooled);
	pData->mpNext = mpDataHead;
	if (mpDataHead)
	{
		NoeAssert(!mpDataHead->mpPrev);
		mpDataHead->mpPrev = pData;
	}
	mpDataHead = pData;
	return pData;
}

NoeInline void CNoeCustomDataList::DestroyCustomData(CNoeCustomData *pData)
{
	if (pData->mpNext)
	{
		pData->mpNext->mpPrev = pData->mpPrev;
	}
	if (pData->mpPrev)
	{
		pData->mpPrev->mpNext = pData->mpNext;
	}
	noeRAPI_t *pRapi = pData->mpRapi;
	bool allocPooled = pData->mAllocPooled;
	pData->~CNoeCustomData();
	if (!allocPooled)
	{
		pRapi->Noesis_UnpooledFree(pData);
	}
}

NoeInline bool CNoeCustomDataList::DestroyCustomDataByName(const char *pName)
{
	if (CNoeCustomData *pData = FindCustomDataByName(pName))
	{
		DestroyCustomData(pData);
		return true;
	}
	return false;
}

NoeInline CNoeCustomData *CNoeCustomDataList::FindCustomDataByName(const char *pName) const
{
	for (CNoeCustomData *pData = mpDataHead; pData; pData = pData->mpNext)
	{
		if (!_stricmp(pName, pData->mpName))
		{
			return pData;
		}
	}
	return NULL;
}

NoeInline CNoeCustomData *CNoeCustomDataList::FindCustomDataByType(const char *pType) const
{
	for (CNoeCustomData *pData = mpDataHead; pData; pData = pData->mpNext)
	{
		if (!_stricmp(pType, pData->mpType))
		{
			return pData;
		}
	}
	return NULL;
}

NoeInline void CNoeCustomDataList::DuplicateListData(CNoeCustomDataList &otherList, noeRAPI_t *pRapi, bool allocPooled)
{
	DestroyList();
	for (CNoeCustomData *pOtherData = otherList.mpDataHead; pOtherData; pOtherData = pOtherData->mpNext)
	{
		CNoeCustomData *pDupData = CreateCustomData(pOtherData->mpName, pOtherData->mpType, pRapi, allocPooled);
		pDupData->SetData(pOtherData->GetData(), pOtherData->GetDataSize());
	}
}

NoeInline void CNoeCustomDataList::AssumeOwnership(CNoeCustomDataList &otherList)
{
	NoeAssert(!mpDataHead);
	mpDataHead = otherList.mpDataHead;
	otherList.mpDataHead = NULL;
}

NoeInline void CNoeCustomDataList::DestroyList()
{
	CNoeCustomData *pData = mpDataHead;
	while (pData)
	{
		CNoeCustomData *pNext = pData->mpNext;
		DestroyCustomData(pData);
		pData = pNext;
	}
	mpDataHead = NULL;
}

NoeInline CNoeCustomDataList &CNoeCustomDataList::operator=(CNoeCustomDataList &otherList)
{
	AssumeOwnership(otherList);
	return *this;
}

#define NPLUGIN_API extern "C" __declspec(dllexport)

#pragma pack(pop)

#endif
