#include "pluginshare.h"
#include <math.h>
#include <algorithm>

#ifdef _NOESIS_INTERNAL
	#include "../maintypes.h"
	#define MATH_PREFIX //math functions are local internally
#else
	#define MATH_PREFIX g_mfn->
#endif

//======================================================
//Math utility classes
//======================================================

//===========================================
//RichAngles implementation
//===========================================
RichAngles::RichAngles(void)
{
	a[0] = 0.0f;
	a[1] = 0.0f;
	a[2] = 0.0f;
}
RichAngles::RichAngles(const float pitch, const float yaw, const float roll)
{
	a[0] = pitch;
	a[1] = yaw;
	a[2] = roll;
}
RichAngles::RichAngles(const float *angles)
{
	a[0] = angles[0];
	a[1] = angles[1];
	a[2] = angles[2];
}
RichAngles::RichAngles(const float *radians, bool scaleToDegrees)
{
	a[0] = radians[0];
	a[1] = radians[1];
	a[2] = radians[2];
	if (scaleToDegrees)
	{
		a[0] *= g_flRadToDeg;
		a[1] *= g_flRadToDeg;
		a[2] *= g_flRadToDeg;
	}
}
float &RichAngles::operator[](int idx)
{
	assert(idx >= 0 && idx < 3);
	return a[idx];
}
float RichAngles::operator[](int idx) const
{
	assert(idx >= 0 && idx < 3);
	return a[idx];
}
RichAngles &RichAngles::operator=(const RichAngles &angles)
{
	a[0] = angles.a[0];
	a[1] = angles.a[1];
	a[2] = angles.a[2];
	return *this;
}
bool RichAngles::operator==(const RichAngles &angles) const
{
	return (a[0] == angles.a[0] && a[1] == angles.a[1] && a[2] == angles.a[2]);
}
bool RichAngles::operator!=(const RichAngles &angles) const
{
	return (a[0] != angles.a[0] || a[1] != angles.a[1] || a[2] != angles.a[2]);
}
RichAngles RichAngles::operator+(const RichAngles &angles) const
{
	return RichAngles(a[0]+angles.a[0], a[1]+angles.a[1], a[2]+angles.a[2]);
}
RichAngles &RichAngles::operator+=(const RichAngles &angles)
{
	a[0] += angles.a[0];
	a[1] += angles.a[1];
	a[2] += angles.a[2];
	return *this;
}
RichAngles RichAngles::operator-(void) const
{
	return RichAngles(-a[0], -a[1], -a[2]);
}
RichAngles RichAngles::operator-(const RichAngles &angles) const
{
	return RichAngles(a[0]-angles.a[0], a[1]-angles.a[1], a[2]-angles.a[2]);
}
RichAngles &RichAngles::operator-=(const RichAngles &angles)
{
	a[0] -= angles.a[0];
	a[1] -= angles.a[1];
	a[2] -= angles.a[2];
	return *this;
}
RichAngles RichAngles::operator*(const RichAngles &angles) const
{
	return RichAngles(a[0]*angles.a[0], a[1]*angles.a[1], a[2]*angles.a[2]);
}
RichAngles &RichAngles::operator*=(const RichAngles &angles)
{
	a[0] *= angles.a[0];
	a[1] *= angles.a[1];
	a[2] *= angles.a[2];
	return *this;
}
RichAngles RichAngles::operator*(const float &f) const
{
	return RichAngles(a[0]*f, a[1]*f, a[2]*f);
}
RichAngles &RichAngles::operator*=(const float &f)
{
	a[0] *= f;
	a[1] *= f;
	a[2] *= f;
	return *this;
}
RichAngles RichAngles::operator/(const RichAngles &angles) const
{
	return RichAngles(a[0]/angles.a[0], a[1]/angles.a[1], a[2]/angles.a[2]);
}
RichAngles &RichAngles::operator/=(const RichAngles &angles)
{
	a[0] /= angles.a[0];
	a[1] /= angles.a[1];
	a[2] /= angles.a[2];
	return *this;
}

void RichAngles::ChangeEndian(void)
{
	LITTLE_BIG_SWAP(a[0]);
	LITTLE_BIG_SWAP(a[1]);
	LITTLE_BIG_SWAP(a[2]);
}

void RichAngles::Mod(float f)
{
	a[0] = fmodf(a[0], f);
	a[1] = fmodf(a[1], f);
	a[2] = fmodf(a[2], f);
}
void RichAngles::Normalize360(void)
{
	Mod(360.0f);
	for (int i = 0; i < 3; i++)
	{
		if (a[i] < 0.0f)
		{
			a[i] += 360.0f;
		}
	}
}
void RichAngles::Normalize180(void)
{
	Normalize360();
	for (int i = 0; i < 3; i++)
	{
		if (a[i] > 180.0f)
		{
			a[i] -= 360.0f;
		}
	}
}

void RichAngles::AngleVectors(RichVec3 *fwd, RichVec3 *right, RichVec3 *up) const
{
	float *v1 = (fwd) ? fwd->v : NULL;
	float *v2 = (right) ? right->v : NULL;
	float *v3 = (up) ? up->v : NULL;
	MATH_PREFIX Math_AngleVectors((float *)a, v1, v2, v3);
}
void RichAngles::Lerp(const RichAngles &angles, const float frac)
{
	a[0] = MATH_PREFIX Math_LinearLerp(a[0], angles.a[0], frac);
	a[1] = MATH_PREFIX Math_LinearLerp(a[1], angles.a[1], frac);
	a[2] = MATH_PREFIX Math_LinearLerp(a[2], angles.a[2], frac);
}
void RichAngles::Lerp(const RichAngles &anglesA, const RichAngles &anglesB, const float frac)
{
	a[0] = MATH_PREFIX Math_LinearLerp(anglesA.a[0], anglesB.a[0], frac);
	a[1] = MATH_PREFIX Math_LinearLerp(anglesA.a[1], anglesB.a[1], frac);
	a[2] = MATH_PREFIX Math_LinearLerp(anglesA.a[2], anglesB.a[2], frac);
}
void RichAngles::ALerp(const RichAngles &angles, const float degrees)
{
	a[0] = MATH_PREFIX Math_BlendAngleLinear(a[0], angles.a[0], degrees);
	a[1] = MATH_PREFIX Math_BlendAngleLinear(a[1], angles.a[1], degrees);
	a[2] = MATH_PREFIX Math_BlendAngleLinear(a[2], angles.a[2], degrees);
}
void RichAngles::ALerp(const RichAngles &anglesA, const RichAngles &anglesB, const float degrees)
{
	a[0] = MATH_PREFIX Math_BlendAngleLinear(anglesA.a[0], anglesB.a[0], degrees);
	a[1] = MATH_PREFIX Math_BlendAngleLinear(anglesA.a[1], anglesB.a[1], degrees);
	a[2] = MATH_PREFIX Math_BlendAngleLinear(anglesA.a[2], anglesB.a[2], degrees);
}

RichVec3 RichAngles::ToVec3(void) const
{
	float t[3];
	MATH_PREFIX Math_AngleVectors((float *)a, t, NULL, NULL);
	return RichVec3(t);
}
RichMat43 RichAngles::ToMat43(void) const
{
	modelMatrix_t m;
	MATH_PREFIX Math_AnglesToMat(a, &m);
	return RichMat43(m);
}
RichMat43 RichAngles::ToMat43_XYZ(bool yFlip) const
{
	RichMat43 m;
	m.Rotate(a[2], 0.0f, 0.0f, 1.0f);
	float y = (yFlip) ? -a[1] : a[1];
	m.Rotate(y, 0.0f, 1.0f, 0.0f);
	m.Rotate(a[0], 1.0f, 0.0f, 0.0f);
	return m;
}
RichQuat RichAngles::ToQuat(void) const
{
	return ToMat43().ToQuat();
}

//===========================================
//RichMat43 implementation
//===========================================
RichMat43::RichMat43(void)
{
	m = g_identityMatrix;
}
RichMat43::RichMat43(const RichVec3 &r0, const RichVec3 &r1, const RichVec3 &r2, const RichVec3 &trans)
{
	RichVec3 *mv = (RichVec3 *)&m;
	mv[0] = r0;
	mv[1] = r1;
	mv[2] = r2;
	mv[3] = trans;
}
RichMat43::RichMat43(const modelMatrix_t &mat)
{
	m = mat;
}
RichMat43::RichMat43(const float *mat)
{
	const modelMatrix_t *t = (const modelMatrix_t *)mat;
	m = *t;
}

RichVec3 &RichMat43::operator[](int idx)
{
	assert(idx >= 0 && idx < 4);
	RichVec3 *mv = (RichVec3 *)&m;
	return mv[idx];
}
const RichVec3 &RichMat43::operator[](int idx) const
{
	assert(idx >= 0 && idx < 4);
	const RichVec3 *mv = (const RichVec3 *)&m;
	return mv[idx];
}

RichMat43 &RichMat43::operator=(const RichMat43 &mat)
{
	m = mat.m;
	return *this;
}
RichMat43 &RichMat43::operator=(const modelMatrix_t &mat)
{
	m = mat;
	return *this;
}
bool RichMat43::operator==(const RichMat43 &mat) const
{
	const RichVec3 *mvA = (const RichVec3 *)&m;
	const RichVec3 *mvB = (const RichVec3 *)&mat.m;
	return (mvA[0] == mvB[0] && mvA[1] == mvB[1] && mvA[2] == mvB[2] && mvA[3] == mvB[3]);
}
bool RichMat43::operator!=(const RichMat43 &mat) const
{
	const RichVec3 *mvA = (const RichVec3 *)&m;
	const RichVec3 *mvB = (const RichVec3 *)&mat.m;
	return (mvA[0] != mvB[0] || mvA[1] != mvB[1] || mvA[2] != mvB[2] || mvA[3] != mvB[3]);
}
RichMat43 RichMat43::operator+(const RichMat43 &mat) const
{
	const RichVec3 *mvA = (const RichVec3 *)&m;
	const RichVec3 *mvB = (const RichVec3 *)&mat.m;

	return RichMat43(mvA[0]+mvB[0], mvA[1]+mvB[1], mvA[2]+mvB[2], mvA[3]+mvB[3]);
}
RichMat43 &RichMat43::operator+=(const RichMat43 &mat)
{
	RichVec3 *mvA = (RichVec3 *)&m;
	const RichVec3 *mvB = (const RichVec3 *)&mat.m;

	mvA[0] += mvB[0];
	mvA[1] += mvB[1];
	mvA[2] += mvB[2];
	mvA[3] += mvB[3];
	return *this;
}
RichMat43 RichMat43::operator-(void) const
{
	const RichVec3 *mv = (const RichVec3 *)&m;

	return RichMat43(-mv[0], -mv[1], -mv[2], -mv[3]);
}
RichMat43 RichMat43::operator-(const RichMat43 &mat) const
{
	const RichVec3 *mvA = (const RichVec3 *)&m;
	const RichVec3 *mvB = (const RichVec3 *)&mat.m;

	return RichMat43(mvA[0]-mvB[0], mvA[1]-mvB[1], mvA[2]-mvB[2], mvA[3]-mvB[3]);
}
RichMat43 &RichMat43::operator-=(const RichMat43 &mat)
{
	RichVec3 *mvA = (RichVec3 *)&m;
	const RichVec3 *mvB = (const RichVec3 *)&mat.m;

	mvA[0] -= mvB[0];
	mvA[1] -= mvB[1];
	mvA[2] -= mvB[2];
	mvA[3] -= mvB[3];
	return *this;
}
RichMat43 RichMat43::operator*(const RichMat43 &mat) const
{
	modelMatrix_t t;
	MATH_PREFIX Math_MatrixMultiply((modelMatrix_t *)&mat.m, (modelMatrix_t *)&m, &t);
	return RichMat43(t);
}
RichMat43 &RichMat43::operator*=(const RichMat43 &mat)
{
	modelMatrix_t t = m;
	MATH_PREFIX Math_MatrixMultiply((modelMatrix_t *)&mat.m, &t, &m);
	return *this;
}
RichVec3 RichMat43::operator*(const RichVec3 &vec) const
{
	return TransformPoint(vec);
}
RichVec4 RichMat43::operator*(const RichVec4 &vec) const
{
	return TransformVec4(vec);
}

void RichMat43::ChangeEndian(void)
{
	RichVec3 *mv = (RichVec3 *)&m;
	mv[0].ChangeEndian();
	mv[1].ChangeEndian();
	mv[2].ChangeEndian();
	mv[3].ChangeEndian();
}

RichVec3 RichMat43::TransformPoint(const RichVec3 &vec) const
{
	float t[3];
	MATH_PREFIX Math_TransformPointByMatrix((modelMatrix_t *)&m, (float *)vec.v, t);
	return RichVec3(t);
}
RichVec3 RichMat43::TransformNormal(const RichVec3 &vec) const
{
	float t[3];
	MATH_PREFIX Math_TransformPointByMatrixNoTrans((modelMatrix_t *)&m, (float *)vec.v, t);
	return RichVec3(t);
}
RichVec4 RichMat43::TransformVec4(const RichVec4 &vec) const
{
	return ToMat44().TransformVec4(vec);
}
RichMat43 RichMat43::GetTranspose(void) const
{
	modelMatrix_t t;
	MATH_PREFIX Math_TransposeMat((modelMatrix_t *)&m, &t);
	return RichMat43(t);
}
void RichMat43::Transpose(void)
{
	modelMatrix_t t;
	MATH_PREFIX Math_TransposeMat(&m, &t);
	m = t;
}
RichMat43 RichMat43::GetInverse(void) const
{
	modelMatrix_t t;
	MATH_PREFIX Math_MatrixInverse((modelMatrix_t *)&m, &t);
	return RichMat43(t);
}
void RichMat43::Inverse(void)
{
	modelMatrix_t t;
	MATH_PREFIX Math_MatrixInverse(&m, &t);
	m = t;
}
RichMat43 RichMat43::GetOrthogonalize(bool keepScale, bool keepFlip, bool straightCross) const
{
	modelMatrix_t t = m;
	MATH_PREFIX Math_OrthogonalizeMatrix(&t, keepScale, keepFlip, straightCross);
	return RichMat43(t);
}
void RichMat43::Orthogonalize(bool keepScale, bool keepFlip, bool straightCross)
{
	MATH_PREFIX Math_OrthogonalizeMatrix(&m, keepScale, keepFlip, straightCross);
}
bool RichMat43::IsSkewed(void)
{
	return MATH_PREFIX Math_MatrixIsSkewed(&m);
}
void RichMat43::Rotate(float degrees, float x, float y, float z, bool transposeRot)
{
	if (transposeRot)
	{
		MATH_PREFIX Math_RotateMatrix(&m, degrees, x, y, z);
	}
	else
	{
		MATH_PREFIX Math_RotateMatrixTP(&m, degrees, x, y, z);
	}
}
void RichMat43::Rotate(float degrees, float *xyz, bool transposeRot)
{
	if (transposeRot)
	{
		MATH_PREFIX Math_RotateMatrix(&m, degrees, xyz[0], xyz[1], xyz[2]);
	}
	else
	{
		MATH_PREFIX Math_RotateMatrixTP(&m, degrees, xyz[0], xyz[1], xyz[2]);
	}
}
void RichMat43::Translate(float x, float y, float z)
{
	float xyz[3] = {x, y, z};
	MATH_PREFIX Math_TranslateMatrix(&m, xyz);
}
void RichMat43::Translate(float *xyz)
{
	MATH_PREFIX Math_TranslateMatrix(&m, xyz);
}
void RichMat43::Lerp(const RichMat43 &postMat, float lerpFrac, bool nonUniform, bool orthogonalize)
{
	modelMatrix_t mout;
	MATH_PREFIX Math_LerpMatrices((modelMatrix_t &)m, (modelMatrix_t &)postMat.m, 1.0f-lerpFrac, mout, nonUniform, orthogonalize);
	m = mout;
}
void RichMat43::Lerp(const RichMat43 &preMat, const RichMat43 &postMat, float lerpFrac, bool nonUniform, bool orthogonalize)
{
	MATH_PREFIX Math_LerpMatrices((modelMatrix_t &)preMat.m, (modelMatrix_t &)postMat.m, 1.0f-lerpFrac, m, nonUniform, orthogonalize);
}
void RichMat43::SLerp(const RichMat43 &postMat, float lerpFrac, bool nonUniform)
{
	modelMatrix_t mout;
	MATH_PREFIX Math_LerpMatricesQ((modelMatrix_t &)m, (modelMatrix_t &)postMat.m, 1.0f-lerpFrac, mout, nonUniform);
	m = mout;
}
void RichMat43::SLerp(const RichMat43 &preMat, const RichMat43 &postMat, float lerpFrac, bool nonUniform)
{
	MATH_PREFIX Math_LerpMatricesQ((modelMatrix_t &)preMat.m, (modelMatrix_t &)postMat.m, 1.0f-lerpFrac, m, nonUniform);
}
void RichMat43::TransformQST(const RichVec3 *pScalingCenter, const RichQuat *pScalingRotation,
								const RichVec3 *pScaling, const RichVec3 *pRotationCenter, const RichQuat *pRotation,
								const RichVec3 *pTranslation)
{
	MATH_PREFIX Math_TransformQST(&m, (const float *)pScalingCenter, (const float *)pScalingRotation, (const float *)pScaling,
									(const float *)pRotationCenter, (const float *)pRotation, (const float *)pTranslation);
}

RichQuat RichMat43::ToQuat(void) const
{
	float t[4];
	MATH_PREFIX Math_MatToQuat((modelMatrix_t *)&m, t, false);
	return RichQuat(t);
}
RichAngles RichMat43::ToAngles(void) const
{
	float t[3];
	MATH_PREFIX Math_MatToAngles(t, &m);
	RichAngles ang(t);
	ang.Normalize360();
	return ang;
}
RichAngles RichMat43::ToAngles_Axis(int *axOrder) const
{
	RichAngles angles;
	MATH_PREFIX Math_AnglesToMatAxis(angles.a, &m, axOrder);
	return angles;
}
RichMat44 RichMat43::ToMat44(void) const
{
	fourxMatrix_t mat;
	MATH_PREFIX Math_ModelMatToGL((modelMatrix_t *)&m, (float *)&mat);
	return RichMat44(mat);
}


//===========================================
//RichMat44 implementation
//===========================================
RichMat44::RichMat44(void)
{
	m = g_identityMatrix4x4;
}
RichMat44::RichMat44(const RichVec4 &r0, const RichVec4 &r1, const RichVec4 &r2, const RichVec4 &r3)
{
	RichVec4 *mv = (RichVec4 *)&m;
	mv[0] = r0;
	mv[1] = r1;
	mv[2] = r2;
	mv[3] = r3;
}
RichMat44::RichMat44(const fourxMatrix_t &mat)
{
	m = mat;
}
RichMat44::RichMat44(const float *mat)
{
	const fourxMatrix_t *t = (const fourxMatrix_t *)mat;
	m = *t;
}

RichVec4 &RichMat44::operator[](int idx)
{
	assert(idx >= 0 && idx < 4);
	RichVec4 *mv = (RichVec4 *)&m;
	return mv[idx];
}
const RichVec4 &RichMat44::operator[](int idx) const
{
	assert(idx >= 0 && idx < 4);
	const RichVec4 *mv = (const RichVec4 *)&m;
	return mv[idx];
}

RichMat44 &RichMat44::operator=(const RichMat44 &mat)
{
	m = mat.m;
	return *this;
}
RichMat44 &RichMat44::operator=(const fourxMatrix_t &mat)
{
	m = mat;
	return *this;
}
bool RichMat44::operator==(const RichMat44 &mat) const
{
	const RichVec4 *mvA = (const RichVec4 *)&m;
	const RichVec4 *mvB = (const RichVec4 *)&mat.m;
	return (mvA[0] == mvB[0] && mvA[1] == mvB[1] && mvA[2] == mvB[2] && mvA[3] == mvB[3]);
}
bool RichMat44::operator!=(const RichMat44 &mat) const
{
	const RichVec4 *mvA = (const RichVec4 *)&m;
	const RichVec4 *mvB = (const RichVec4 *)&mat.m;
	return (mvA[0] != mvB[0] || mvA[1] != mvB[1] || mvA[2] != mvB[2] || mvA[3] != mvB[3]);
}
RichMat44 RichMat44::operator+(const RichMat44 &mat) const
{
	const RichVec4 *mvA = (const RichVec4 *)&m;
	const RichVec4 *mvB = (const RichVec4 *)&mat.m;

	return RichMat44(mvA[0]+mvB[0], mvA[1]+mvB[1], mvA[2]+mvB[2], mvA[3]+mvB[3]);
}
RichMat44 &RichMat44::operator+=(const RichMat44 &mat)
{
	RichVec4 *mvA = (RichVec4 *)&m;
	const RichVec4 *mvB = (const RichVec4 *)&mat.m;

	mvA[0] += mvB[0];
	mvA[1] += mvB[1];
	mvA[2] += mvB[2];
	mvA[3] += mvB[3];
	return *this;
}
RichMat44 RichMat44::operator-(void) const
{
	const RichVec4 *mv = (const RichVec4 *)&m;

	return RichMat44(-mv[0], -mv[1], -mv[2], -mv[3]);
}
RichMat44 RichMat44::operator-(const RichMat44 &mat) const
{
	const RichVec4 *mvA = (const RichVec4 *)&m;
	const RichVec4 *mvB = (const RichVec4 *)&mat.m;

	return RichMat44(mvA[0]-mvB[0], mvA[1]-mvB[1], mvA[2]-mvB[2], mvA[3]-mvB[3]);
}
RichMat44 &RichMat44::operator-=(const RichMat44 &mat)
{
	RichVec4 *mvA = (RichVec4 *)&m;
	const RichVec4 *mvB = (const RichVec4 *)&mat.m;

	mvA[0] -= mvB[0];
	mvA[1] -= mvB[1];
	mvA[2] -= mvB[2];
	mvA[3] -= mvB[3];
	return *this;
}
RichMat44 RichMat44::operator*(const RichMat44 &mat) const
{
	fourxMatrix_t t;
	MATH_PREFIX Math_MatrixMultiply4x4((fourxMatrix_t *)&m, (fourxMatrix_t *)&mat.m, &t);
	return RichMat44(t);
}
RichMat44 &RichMat44::operator*=(const RichMat44 &mat)
{
	fourxMatrix_t t = m;
	MATH_PREFIX Math_MatrixMultiply4x4(&t, (fourxMatrix_t *)&mat.m, &m);
	return *this;
}
RichVec4 RichMat44::operator*(const RichVec4 &vec) const
{
	return TransformVec4(vec);
}

void RichMat44::ChangeEndian(void)
{
	RichVec4 *mv = (RichVec4 *)&m;
	mv[0].ChangeEndian();
	mv[1].ChangeEndian();
	mv[2].ChangeEndian();
	mv[3].ChangeEndian();
}

RichVec4 RichMat44::TransformVec4(const RichVec4 &vec) const
{
	float out[4];
	const float *matrix = (float *)&m;
	const float *in = vec.v;
	out[0] = matrix[0]*in[0] + matrix[4+0]*in[1] + matrix[8+0]*in[2] + matrix[12+0]*in[3];
	out[1] = matrix[1]*in[0] + matrix[4+1]*in[1] + matrix[8+1]*in[2] + matrix[12+1]*in[3];
	out[2] = matrix[2]*in[0] + matrix[4+2]*in[1] + matrix[8+2]*in[2] + matrix[12+2]*in[3];
	out[3] = matrix[3]*in[0] + matrix[4+3]*in[1] + matrix[8+3]*in[2] + matrix[12+3]*in[3];
	return RichVec4(out);
}
RichVec3 RichMat44::TransformNormal(const RichVec3 &vec) const
{
	float out[3];
	const float *matrix = (float *)&m;
	const float *in = vec.v;
	out[0] = matrix[0]*in[0] + matrix[4+0]*in[1] + matrix[8+0]*in[2];
	out[1] = matrix[1]*in[0] + matrix[4+1]*in[1] + matrix[8+1]*in[2];
	out[2] = matrix[2]*in[0] + matrix[4+2]*in[1] + matrix[8+2]*in[2];
	return RichVec3(out);
}
RichMat44 RichMat44::GetTranspose(void) const
{
	RichMat44 dst;
	const RichMat44 &src = *this;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{ //swap rows/columns
			dst[i][j] = src[j][i];
		}
	}
	return dst;
}
void RichMat44::Transpose(void)
{
	*this = GetTranspose();
}
RichMat44 RichMat44::GetInverse(void) const
{
	fourxMatrix_t t;
	MATH_PREFIX Math_MatrixInverse4x4((float *)&m, (float *)&t);
	return RichMat44(t);
}
void RichMat44::Inverse(void)
{
	fourxMatrix_t t = m;
	MATH_PREFIX Math_MatrixInverse4x4((float *)&t, (float *)&m);
}
void RichMat44::Rotate(float degrees, float x, float y, float z)
{
	MATH_PREFIX Math_RotateMatrix4x4(&m, degrees, x, y, z);
}
void RichMat44::Rotate(float degrees, float *xyz)
{
	MATH_PREFIX Math_RotateMatrix4x4(&m, degrees, xyz[0], xyz[1], xyz[2]);
}
void RichMat44::Translate(float x, float y, float z)
{
	float xyz[3] = {x, y, z};
	MATH_PREFIX Math_TranslateMatrix4x4(&m, xyz);
}
void RichMat44::Translate(float *xyz)
{
	MATH_PREFIX Math_TranslateMatrix4x4(&m, xyz);
}

RichMat43 RichMat44::ToMat43(void) const
{
	modelMatrix_t mat;
	MATH_PREFIX Math_ModelMatFromGL(&mat, (float *)&m);
	return RichMat43(mat);
}

//===========================================
//RichQuat implementation
//===========================================
RichQuat::RichQuat(void)
{
	q[0] = 0.0f;
	q[1] = 0.0f;
	q[2] = 0.0f;
	q[3] = 0.0f;
}
RichQuat::RichQuat(const float x, const float y, const float z, const float w)
{
	q[0] = x;
	q[1] = y;
	q[2] = z;
	q[3] = w;
}
RichQuat::RichQuat(const float *xyzw)
{
	q[0] = xyzw[0];
	q[1] = xyzw[1];
	q[2] = xyzw[2];
	q[3] = xyzw[3];
}
RichQuat::RichQuat(const float *xyz, const bool noW)
{
	if (noW)
	{
		FromQuat3(xyz);
	}
	else
	{
		q[0] = xyz[0];
		q[1] = xyz[1];
		q[2] = xyz[2];
		q[3] = xyz[3];
	}
}

float &RichQuat::operator[](int idx)
{
	assert(idx >= 0 && idx < 4);
	return q[idx];
}
float RichQuat::operator[](int idx) const
{
	assert(idx >= 0 && idx < 4);
	return q[idx];
}
RichQuat &RichQuat::operator=(const RichQuat &quat)
{
	q[0] = quat.q[0];
	q[1] = quat.q[1];
	q[2] = quat.q[2];
	q[3] = quat.q[3];
	return *this;
}
bool RichQuat::operator==(const RichQuat &quat) const
{
	return (q[0] == quat.q[0] && q[1] == quat.q[1] && q[2] == quat.q[2] && q[3] == quat.q[3]);
}
bool RichQuat::operator!=(const RichQuat &quat) const
{
	return (q[0] != quat.q[0] || q[1] != quat.q[1] || q[2] != quat.q[2] || q[3] != quat.q[3]);
}
RichQuat RichQuat::operator+(const RichQuat &quat) const
{
	return RichQuat(q[0]+quat.q[0], q[1]+quat.q[1], q[2]+quat.q[2], q[3]+quat.q[3]);
}
RichQuat &RichQuat::operator+=(const RichQuat &quat)
{
	q[0] += quat.q[0];
	q[1] += quat.q[1];
	q[2] += quat.q[2];
	q[3] += quat.q[3];
	return *this;
}
RichQuat RichQuat::operator-(void) const
{
	return RichQuat(-q[0], -q[1], -q[2], -q[3]);
}
RichQuat RichQuat::operator-(const RichQuat &quat) const
{
	return RichQuat(q[0]-quat.q[0], q[1]-quat.q[1], q[2]-quat.q[2], q[3]-quat.q[3]);
}
RichQuat &RichQuat::operator-=(const RichQuat &quat)
{
	q[0] -= quat.q[0];
	q[1] -= quat.q[1];
	q[2] -= quat.q[2];
	q[3] -= quat.q[3];
	return *this;
}
RichQuat RichQuat::operator*(const RichQuat &quat) const
{
	float t[4];
	t[0] = q[3]*quat.q[0] + q[0]*quat.q[3] + q[1]*quat.q[2] - q[2]*quat.q[1];
	t[1] = q[3]*quat.q[1] + q[1]*quat.q[3] + q[2]*quat.q[0] - q[0]*quat.q[2];
	t[2] = q[3]*quat.q[2] + q[2]*quat.q[3] + q[0]*quat.q[1] - q[1]*quat.q[0];
	t[3] = q[3]*quat.q[3] - q[0]*quat.q[0] - q[1]*quat.q[1] - q[2]*quat.q[2];
	return RichQuat(t);
}
RichQuat &RichQuat::operator*=(const RichQuat &quat)
{
	float t[4];
	t[0] = q[3]*quat.q[0] + q[0]*quat.q[3] + q[1]*quat.q[2] - q[2]*quat.q[1];
	t[1] = q[3]*quat.q[1] + q[1]*quat.q[3] + q[2]*quat.q[0] - q[0]*quat.q[2];
	t[2] = q[3]*quat.q[2] + q[2]*quat.q[3] + q[0]*quat.q[1] - q[1]*quat.q[0];
	t[3] = q[3]*quat.q[3] - q[0]*quat.q[0] - q[1]*quat.q[1] - q[2]*quat.q[2];
	q[0] = t[0];
	q[1] = t[1];
	q[2] = t[2];
	q[3] = t[3];
	return *this;
}
RichVec3 RichQuat::operator*(const RichVec3 &vec) const
{
	return TransformPoint(vec);
}
RichQuat RichQuat::operator*(const float &f) const
{
	return RichQuat(q[0]*f, q[1]*f, q[2]*f, q[3]*f);
}
RichQuat &RichQuat::operator*=(const float &f)
{
	q[0] *= f;
	q[1] *= f;
	q[2] *= f;
	q[3] *= f;
	return *this;
}

void RichQuat::ChangeEndian(void)
{
	LITTLE_BIG_SWAP(q[0]);
	LITTLE_BIG_SWAP(q[1]);
	LITTLE_BIG_SWAP(q[2]);
	LITTLE_BIG_SWAP(q[3]);
}

RichVec3 RichQuat::TransformPoint(const RichVec3 &vec) const
{
	return ToMat43()*vec;
}
RichQuat RichQuat::GetTranspose(void)
{
	RichMat43 m = ToMat43(true);
	return m.ToQuat();
}
void RichQuat::Transpose(void)
{
	RichMat43 m = ToMat43(true);
	*this = m.ToQuat();
}
float RichQuat::Length(void) const
{
	return sqrtf(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
}
float RichQuat::Normalize(void)
{
	float l = Length();
	if (l != 0.0f)
	{
		float lm = 1.0f/l;
		*this *= lm;
	}
	return l;
}
void RichQuat::Lerp(const RichQuat &quat, const float frac)
{
	q[0] = MATH_PREFIX Math_LinearLerp(q[0], quat.q[0], frac);
	q[1] = MATH_PREFIX Math_LinearLerp(q[1], quat.q[1], frac);
	q[2] = MATH_PREFIX Math_LinearLerp(q[2], quat.q[2], frac);
	q[3] = MATH_PREFIX Math_LinearLerp(q[3], quat.q[3], frac);
}
void RichQuat::Lerp(const RichQuat &quatA, const RichQuat &quatB, const float frac)
{
	q[0] = MATH_PREFIX Math_LinearLerp(quatA.q[0], quatB.q[0], frac);
	q[1] = MATH_PREFIX Math_LinearLerp(quatA.q[1], quatB.q[1], frac);
	q[2] = MATH_PREFIX Math_LinearLerp(quatA.q[2], quatB.q[2], frac);
	q[3] = MATH_PREFIX Math_LinearLerp(quatA.q[3], quatB.q[3], frac);
}
void RichQuat::SLerp(const RichQuat &quat, const float frac)
{
	float t[4];
	MATH_PREFIX Math_QuatSlerp(q, (float *)quat.q, frac, t);
	q[0] = t[0];
	q[1] = t[1];
	q[2] = t[2];
	q[3] = t[3];
}
void RichQuat::SLerp(const RichQuat &quatA, const RichQuat &quatB, const float frac)
{
	MATH_PREFIX Math_QuatSlerp((float *)quatA.q, (float *)quatB.q, frac, q);
}

void RichQuat::FromQuat3(const float *quat)
{
	q[0] = quat[0];
	q[1] = quat[1];
	q[2] = quat[2];
	q[3] = sqrtf(fabsf(1.0f - (quat[0]*quat[0] + quat[1]*quat[1] + quat[2]*quat[2])));
}
void RichQuat::ToQuat3(float *quat) const
{
	if (q[3] < 0.0f)
	{
		quat[0] = -q[0];
		quat[1] = -q[1];
		quat[2] = -q[2];
	}
	else
	{
		quat[0] = q[0];
		quat[1] = q[1];
		quat[2] = q[2];
	}
}
RichMat43 RichQuat::ToMat43(bool transposed) const
{
	modelMatrix_t m;
	MATH_PREFIX Math_QuatToMat((float *)q, &m, false, transposed);
	m.o[0] = 0.0f;
	m.o[1] = 0.0f;
	m.o[2] = 0.0f;
	return RichMat43(m);
}
RichAngles RichQuat::ToAngles(void) const
{
	return ToMat43().ToAngles();
}

//===========================================
//RichVec3 implementation
//===========================================
RichVec3::RichVec3(void)
{
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
}
RichVec3::RichVec3(const float x, const float y, const float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}
RichVec3::RichVec3(const float *xyz)
{
	v[0] = xyz[0];
	v[1] = xyz[1];
	v[2] = xyz[2];
}

float &RichVec3::operator[](int idx)
{
	assert(idx >= 0 && idx < 3);
	return v[idx];
}
float RichVec3::operator[](int idx) const
{
	assert(idx >= 0 && idx < 3);
	return v[idx];
}
RichVec3 &RichVec3::operator=(const RichVec3 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	return *this;
}
bool RichVec3::operator==(const RichVec3 &vec) const
{
	return (v[0] == vec.v[0] && v[1] == vec.v[1] && v[2] == vec.v[2]);
}
bool RichVec3::operator!=(const RichVec3 &vec) const
{
	return (v[0] != vec.v[0] || v[1] != vec.v[1] || v[2] != vec.v[2]);
}
RichVec3 RichVec3::operator+(const RichVec3 &vec) const
{
	return RichVec3(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2]);
}
RichVec3 &RichVec3::operator+=(const RichVec3 &vec)
{
	v[0] += vec.v[0];
	v[1] += vec.v[1];
	v[2] += vec.v[2];
	return *this;
}
RichVec3 RichVec3::operator-(void) const
{
	return RichVec3(-v[0], -v[1], -v[2]);
}
RichVec3 RichVec3::operator-(const RichVec3 &vec) const
{
	return RichVec3(v[0]-vec.v[0], v[1]-vec.v[1], v[2]-vec.v[2]);
}
RichVec3 &RichVec3::operator-=(const RichVec3 &vec)
{
	v[0] -= vec.v[0];
	v[1] -= vec.v[1];
	v[2] -= vec.v[2];
	return *this;
}
RichVec3 RichVec3::operator*(const RichVec3 &vec) const
{
	return RichVec3(v[0]*vec.v[0], v[1]*vec.v[1], v[2]*vec.v[2]);
}
RichVec3 &RichVec3::operator*=(const RichVec3 &vec)
{
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	v[2] *= vec.v[2];
	return *this;
}
RichVec3 RichVec3::operator*(const float &f) const
{
	return RichVec3(v[0]*f, v[1]*f, v[2]*f);
}
RichVec3 &RichVec3::operator*=(const float &f)
{
	v[0] *= f;
	v[1] *= f;
	v[2] *= f;
	return *this;
}
RichVec3 RichVec3::operator/(const RichVec3 &vec) const
{
	return RichVec3(v[0]/vec.v[0], v[1]/vec.v[1], v[2]/vec.v[2]);
}
RichVec3 &RichVec3::operator/=(const RichVec3 &vec)
{
	v[0] /= vec.v[0];
	v[1] /= vec.v[1];
	v[2] /= vec.v[2];
	return *this;
}

void RichVec3::ChangeEndian(void)
{
	LITTLE_BIG_SWAP(v[0]);
	LITTLE_BIG_SWAP(v[1]);
	LITTLE_BIG_SWAP(v[2]);
}

float RichVec3::Dot(const RichVec3 &vec) const
{
	return MATH_PREFIX Math_DotProduct(v, vec.v);
}
RichVec3 RichVec3::Cross(const RichVec3 &vec) const
{
	float r[3];
	MATH_PREFIX Math_CrossProduct(v, vec.v, r);
	return RichVec3(r);
}
void RichVec3::Cross(const RichVec3 &vecA, const RichVec3 &vecB)
{
	MATH_PREFIX Math_CrossProduct(vecA.v, vecB.v, v);
}
float RichVec3::Length(void) const
{
	return sqrtf(LengthSq());
}
float RichVec3::LengthSq(void) const
{
	return (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
float RichVec3::Normalize(void)
{
	return MATH_PREFIX Math_VecNorm(v);
}
RichVec3 RichVec3::Normalized(void) const
{
	float t[3];
	t[0] = v[0];
	t[1] = v[1];
	t[2] = v[2];
	MATH_PREFIX Math_VecNorm(t);
	return RichVec3(t);
}
void RichVec3::Lerp(const RichVec3 &vec, const float frac)
{
	v[0] = MATH_PREFIX Math_LinearLerp(v[0], vec.v[0], frac);
	v[1] = MATH_PREFIX Math_LinearLerp(v[1], vec.v[1], frac);
	v[2] = MATH_PREFIX Math_LinearLerp(v[2], vec.v[2], frac);
}
void RichVec3::Lerp(const RichVec3 &vecA, const RichVec3 &vecB, const float frac)
{
	v[0] = MATH_PREFIX Math_LinearLerp(vecA.v[0], vecB.v[0], frac);
	v[1] = MATH_PREFIX Math_LinearLerp(vecA.v[1], vecB.v[1], frac);
	v[2] = MATH_PREFIX Math_LinearLerp(vecA.v[2], vecB.v[2], frac);
}
void RichVec3::BarycentricCoordinates(const RichVec3 &v0, const RichVec3 &v1, const RichVec3 &v2, const RichVec3 &point)
{
	const RichVec3 p0 = v0-point;
	const RichVec3 p1 = v1-point;
	const RichVec3 p2 = v2-point;

	RichVec3 triArea;
	triArea.Cross(v0-v1, v0-v2);
	const float area = triArea.Length();
	if (area == 0.0f)
	{ //degenerate triangle
		v[0] = v[1] = v[2] = 0.0f;	
	}
	else
	{
		const float invArea = 1.0f / area;
		RichVec3 area0, area1, area2;
		area0.Cross(p1, p2);
		area1.Cross(p2, p0);
		area2.Cross(p0, p1);
		v[0] = area0.Length() * invArea * ( (triArea.Dot(area0) > 0.0) ? 1.0f : -1.0f );
		v[1] = area1.Length() * invArea * ( (triArea.Dot(area1) > 0.0) ? 1.0f : -1.0f );
		v[2] = area2.Length() * invArea * ( (triArea.Dot(area2) > 0.0) ? 1.0f : -1.0f );
	}
}

void RichVec3::SLerp(const RichVec3 &vec, const float frac)
{
	SLerp(*this, vec, frac);
}

//inputs are expected to be normalized
void RichVec3::SLerp(const RichVec3 &vecA, const RichVec3 &vecB, const float frac)
{
	if (vecA == vecB)
	{
		*this = vecA;
	}
	else
	{
		const float dp = vecA.Dot(vecB);
		const float thetaFrac = acosf(dp) * frac;
		RichVec3 const vecC = (vecB - vecA * dp).Normalized();

		const float cs = cosf(thetaFrac);
		const float sn = sinf(thetaFrac);

		*this = (vecA * cs + vecC * sn);
		//or:
		/*
		const float dp = vecA.Dot(vecB);
		const float theta = acosf(dp);

		const float st = sinf(theta);
		const float fA = sinf((1.0f - frac) * theta) / st;
		const float fB = sinf(frac * theta) / st;

		*this = vecA * fA + vecB * fB;
		*/
	}
}

void RichVec3::OrthoBasis(RichVec3 *pRightOut, RichVec3 *pUpOut) const
{
	if (v[0] > 0.5f || v[0] < -0.5f || v[1] > 0.5f || v[1] < -0.5f)
	{
		*pRightOut = RichVec3(v[1], -v[0], 0.0f);
	}
	else
	{
		*pRightOut = RichVec3(0.0f, v[2], -v[1]);
	}
	pRightOut->Normalize();
	pUpOut->Cross(*this, *pRightOut);
	pUpOut->Normalize();
}

void RichVec3::Min(const RichVec3 &vec)
{
	Min(*this, vec);
}

void RichVec3::Min(const RichVec3 &vecA, const RichVec3 &vecB)
{
	v[0] = (vecA[0] < vecB[0]) ? vecA[0] : vecB[0];
	v[1] = (vecA[1] < vecB[1]) ? vecA[1] : vecB[1];
	v[2] = (vecA[2] < vecB[2]) ? vecA[2] : vecB[2];
}

void RichVec3::Max(const RichVec3 &vec)
{
	Max(*this, vec);
}

void RichVec3::Max(const RichVec3 &vecA, const RichVec3 &vecB)
{
	v[0] = (vecA[0] > vecB[0]) ? vecA[0] : vecB[0];
	v[1] = (vecA[1] > vecB[1]) ? vecA[1] : vecB[1];
	v[2] = (vecA[2] > vecB[2]) ? vecA[2] : vecB[2];
}

RichAngles RichVec3::ToAngles(void) const
{
	float t[3];
	MATH_PREFIX Math_VecToAngles(v, t);
	return RichAngles(t);
}
RichVec4 RichVec3::ToVec4(void) const
{
	return RichVec4(v[0], v[1], v[2], 0.0f);
}
RichMat43 RichVec3::ToMat43(void) const
{
	RichVec3 trans;

	RichVec3 fwd = *this;
	RichVec3 right;
	float xy = v[0]*v[0] + v[1]*v[1];
	if (xy)
	{
		float ixyl = 1.0f/sqrtf(xy);
		right[0] = -v[1]*ixyl;
		right[1] = v[0]*ixyl;
	}
	else
	{
		right[0] = 1.0f;
	}
	RichVec3 up = fwd.Cross(right);
	return RichMat43(fwd, right, up, trans);
}
RichMat43 RichVec3::ToMat43Z(void) const
{
	RichVec3 trans;

	RichVec3 up = *this;
	RichVec3 right;
	float yz = v[1]*v[1] + v[2]*v[2];
	if (yz)
	{
		float iyzl = 1.0f/sqrtf(yz);
		right[1] = v[2]*iyzl;
		right[2] = -v[1]*iyzl;
	}
	else
	{
		right[0] = 1.0f;
	}
	RichVec3 fwd = right.Cross(up);
	return RichMat43(fwd, right, up, trans);
}


//===========================================
//RichVecH3 implementation
//===========================================
RichVecH3::RichVecH3(void)
{
	v[0] = 0.0;
	v[1] = 0.0;
	v[2] = 0.0;
}
RichVecH3::RichVecH3(const double x, const double y, const double z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}
RichVecH3::RichVecH3(const double *xyz)
{
	v[0] = xyz[0];
	v[1] = xyz[1];
	v[2] = xyz[2];
}

double &RichVecH3::operator[](int idx)
{
	assert(idx >= 0 && idx < 3);
	return v[idx];
}
double RichVecH3::operator[](int idx) const
{
	assert(idx >= 0 && idx < 3);
	return v[idx];
}
RichVecH3 &RichVecH3::operator=(const RichVecH3 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	return *this;
}
bool RichVecH3::operator==(const RichVecH3 &vec) const
{
	return (v[0] == vec.v[0] && v[1] == vec.v[1] && v[2] == vec.v[2]);
}
bool RichVecH3::operator!=(const RichVecH3 &vec) const
{
	return (v[0] != vec.v[0] || v[1] != vec.v[1] || v[2] != vec.v[2]);
}
RichVecH3 RichVecH3::operator+(const RichVecH3 &vec) const
{
	return RichVecH3(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2]);
}
RichVecH3 &RichVecH3::operator+=(const RichVecH3 &vec)
{
	v[0] += vec.v[0];
	v[1] += vec.v[1];
	v[2] += vec.v[2];
	return *this;
}
RichVecH3 RichVecH3::operator-(void) const
{
	return RichVecH3(-v[0], -v[1], -v[2]);
}
RichVecH3 RichVecH3::operator-(const RichVecH3 &vec) const
{
	return RichVecH3(v[0]-vec.v[0], v[1]-vec.v[1], v[2]-vec.v[2]);
}
RichVecH3 &RichVecH3::operator-=(const RichVecH3 &vec)
{
	v[0] -= vec.v[0];
	v[1] -= vec.v[1];
	v[2] -= vec.v[2];
	return *this;
}
RichVecH3 RichVecH3::operator*(const RichVecH3 &vec) const
{
	return RichVecH3(v[0]*vec.v[0], v[1]*vec.v[1], v[2]*vec.v[2]);
}
RichVecH3 &RichVecH3::operator*=(const RichVecH3 &vec)
{
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	v[2] *= vec.v[2];
	return *this;
}
RichVecH3 RichVecH3::operator*(const double &f) const
{
	return RichVecH3(v[0]*f, v[1]*f, v[2]*f);
}
RichVecH3 &RichVecH3::operator*=(const double &f)
{
	v[0] *= f;
	v[1] *= f;
	v[2] *= f;
	return *this;
}
RichVecH3 RichVecH3::operator/(const RichVecH3 &vec) const
{
	return RichVecH3(v[0]/vec.v[0], v[1]/vec.v[1], v[2]/vec.v[2]);
}
RichVecH3 &RichVecH3::operator/=(const RichVecH3 &vec)
{
	v[0] /= vec.v[0];
	v[1] /= vec.v[1];
	v[2] /= vec.v[2];
	return *this;
}

void RichVecH3::ChangeEndian(void)
{
	LITTLE_BIG_SWAP(v[0]);
	LITTLE_BIG_SWAP(v[1]);
	LITTLE_BIG_SWAP(v[2]);
}

double RichVecH3::Dot(const RichVecH3 &vec) const
{
	return v[0] * vec.v[0] + v[1] * vec.v[1] + v[2] * vec.v[2];
}
RichVecH3 RichVecH3::Cross(const RichVecH3 &vec) const
{
	double r[3];
	r[0] = v[1] * vec.v[2] - v[2] * vec.v[1];
	r[1] = v[2] * vec.v[0] - v[0] * vec.v[2];
	r[2] = v[0] * vec.v[1] - v[1] * vec.v[0];
	return RichVecH3(r);
}
void RichVecH3::Cross(const RichVecH3 &vecA, const RichVecH3 &vecB)
{
	v[0] = vecA.v[1] * vecB.v[2] - vecA.v[2] * vecB.v[1];
	v[1] = vecA.v[2] * vecB.v[0] - vecA.v[0] * vecB.v[2];
	v[2] = vecA.v[0] * vecB.v[1] - vecA.v[1] * vecB.v[0];
}
double RichVecH3::Length(void) const
{
	return sqrt(LengthSq());
}
double RichVecH3::LengthSq(void) const
{
	return (v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
double RichVecH3::Normalize(void)
{
	const double l = Length();
	if (l != 0.0)
	{
		const double invL = 1.0 / l;
		v[0] *= invL;
		v[1] *= invL;
		v[2] *= invL;
	}
	return l;
}
RichVecH3 RichVecH3::Normalized(void) const
{
	RichVecH3 t = *this;
	t.Normalize();
	return t;
}
void RichVecH3::BarycentricCoordinates(const RichVecH3 &v0, const RichVecH3 &v1, const RichVecH3 &v2, const RichVecH3 &point)
{
	const RichVecH3 p0 = v0-point;
	const RichVecH3 p1 = v1-point;
	const RichVecH3 p2 = v2-point;

	RichVecH3 triArea;
	triArea.Cross(v0-v1, v0-v2);
	const double area = triArea.Length();
	if (area == 0.0)
	{ //degenerate triangle
		v[0] = v[1] = v[2] = 0.0;	
	}
	else
	{
		const double invArea = 1.0 / area;
		RichVecH3 area0, area1, area2;
		area0.Cross(p1, p2);
		area1.Cross(p2, p0);
		area2.Cross(p0, p1);
		v[0] = area0.Length() * invArea * ( (triArea.Dot(area0) > 0.0) ? 1.0 : -1.0 );
		v[1] = area1.Length() * invArea * ( (triArea.Dot(area1) > 0.0) ? 1.0 : -1.0 );
		v[2] = area2.Length() * invArea * ( (triArea.Dot(area2) > 0.0) ? 1.0 : -1.0 );
	}
}

void RichVecH3::SLerp(const RichVecH3 &vec, const double frac)
{
	SLerp(*this, vec, frac);
}

//inputs are expected to be normalized
void RichVecH3::SLerp(const RichVecH3 &vecA, const RichVecH3 &vecB, const double frac)
{
	if (vecA == vecB)
	{
		*this = vecA;
	}
	else
	{
		const double dp = vecA.Dot(vecB);
		const double thetaFrac = acos(dp) * frac;
		RichVecH3 const vecC = (vecB - vecA * dp).Normalized();

		const double cs = cos(thetaFrac);
		const double sn = sin(thetaFrac);

		*this = (vecA * cs + vecC * sn);
		//or:
		/*
		const double dp = vecA.Dot(vecB);
		const double theta = acos(dp);

		const double st = sin(theta);
		const double fA = sin((1.0 - frac) * theta) / st;
		const double fB = sin(frac * theta) / st;

		*this = vecA * fA + vecB * fB;
		*/
	}
}

void RichVecH3::OrthoBasis(RichVecH3 *pRightOut, RichVecH3 *pUpOut) const
{
	if (v[0] > 0.5 || v[0] < -0.5 || v[1] > 0.5 || v[1] < -0.5)
	{
		*pRightOut = RichVecH3(v[1], -v[0], 0.0);
	}
	else
	{
		*pRightOut = RichVecH3(0.0, v[2], -v[1]);
	}
	pRightOut->Normalize();
	pUpOut->Cross(*this, *pRightOut);
	pUpOut->Normalize();
}

void RichVecH3::Min(const RichVecH3 &vec)
{
	Min(*this, vec);
}

void RichVecH3::Min(const RichVecH3 &vecA, const RichVecH3 &vecB)
{
	v[0] = (vecA[0] < vecB[0]) ? vecA[0] : vecB[0];
	v[1] = (vecA[1] < vecB[1]) ? vecA[1] : vecB[1];
	v[2] = (vecA[2] < vecB[2]) ? vecA[2] : vecB[2];
}

void RichVecH3::Max(const RichVecH3 &vec)
{
	Max(*this, vec);
}

void RichVecH3::Max(const RichVecH3 &vecA, const RichVecH3 &vecB)
{
	v[0] = (vecA[0] > vecB[0]) ? vecA[0] : vecB[0];
	v[1] = (vecA[1] > vecB[1]) ? vecA[1] : vecB[1];
	v[2] = (vecA[2] > vecB[2]) ? vecA[2] : vecB[2];
}


//===========================================
//RichVec4 implementation
//===========================================
RichVec4::RichVec4(void)
{
	v[0] = 0.0f;
	v[1] = 0.0f;
	v[2] = 0.0f;
	v[3] = 0.0f;
}
RichVec4::RichVec4(const float x, const float y, const float z, const float w)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}
RichVec4::RichVec4(const float *xyzw)
{
	v[0] = xyzw[0];
	v[1] = xyzw[1];
	v[2] = xyzw[2];
	v[3] = xyzw[3];
}
RichVec4::RichVec4(const RichVec3 &xyz, const float w)
{
	v[0] = xyz.v[0];
	v[1] = xyz.v[1];
	v[2] = xyz.v[2];
	v[3] = w;
}

float &RichVec4::operator[](int idx)
{
	assert(idx >= 0 && idx < 4);
	return v[idx];
}
float RichVec4::operator[](int idx) const
{
	assert(idx >= 0 && idx < 4);
	return v[idx];
}
RichVec4 &RichVec4::operator=(const RichVec4 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	v[2] = vec.v[2];
	v[3] = vec.v[3];
	return *this;
}
bool RichVec4::operator==(const RichVec4 &vec) const
{
	return (v[0] == vec.v[0] && v[1] == vec.v[1] && v[2] == vec.v[2] && v[3] == vec.v[3]);
}
bool RichVec4::operator!=(const RichVec4 &vec) const
{
	return (v[0] != vec.v[0] || v[1] != vec.v[1] || v[2] != vec.v[2] || v[3] != vec.v[3]);
}
RichVec4 RichVec4::operator+(const RichVec4 &vec) const
{
	return RichVec4(v[0]+vec.v[0], v[1]+vec.v[1], v[2]+vec.v[2], v[3]+vec.v[3]);
}
RichVec4 &RichVec4::operator+=(const RichVec4 &vec)
{
	v[0] += vec.v[0];
	v[1] += vec.v[1];
	v[2] += vec.v[2];
	v[3] += vec.v[3];
	return *this;
}
RichVec4 RichVec4::operator-(void) const
{
	return RichVec4(-v[0], -v[1], -v[2], -v[3]);
}
RichVec4 RichVec4::operator-(const RichVec4 &vec) const
{
	return RichVec4(v[0]-vec.v[0], v[1]-vec.v[1], v[2]-vec.v[2], v[3]-vec.v[3]);
}
RichVec4 &RichVec4::operator-=(const RichVec4 &vec)
{
	v[0] -= vec.v[0];
	v[1] -= vec.v[1];
	v[2] -= vec.v[2];
	v[3] -= vec.v[3];
	return *this;
}
RichVec4 RichVec4::operator*(const RichVec4 &vec) const
{
	return RichVec4(v[0]*vec.v[0], v[1]*vec.v[1], v[2]*vec.v[2], v[3]*vec.v[3]);
}
RichVec4 &RichVec4::operator*=(const RichVec4 &vec)
{
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	v[2] *= vec.v[2];
	v[3] *= vec.v[3];
	return *this;
}
RichVec4 RichVec4::operator*(const float &f) const
{
	return RichVec4(v[0]*f, v[1]*f, v[2]*f, v[3]*f);
}
RichVec4 &RichVec4::operator*=(const float &f)
{
	v[0] *= f;
	v[1] *= f;
	v[2] *= f;
	v[3] *= f;
	return *this;
}
RichVec4 RichVec4::operator/(const RichVec4 &vec) const
{
	return RichVec4(v[0]/vec.v[0], v[1]/vec.v[1], v[2]/vec.v[2], v[3]/vec.v[2]);
}
RichVec4 &RichVec4::operator/=(const RichVec4 &vec)
{
	v[0] /= vec.v[0];
	v[1] /= vec.v[1];
	v[2] /= vec.v[2];
	v[3] /= vec.v[3];
	return *this;
}

void RichVec4::ChangeEndian(void)
{
	LITTLE_BIG_SWAP(v[0]);
	LITTLE_BIG_SWAP(v[1]);
	LITTLE_BIG_SWAP(v[2]);
	LITTLE_BIG_SWAP(v[3]);
}

float RichVec4::Dot(const RichVec4 &vec) const
{
	const float *v1 = v;
	const float *v2 = vec.v;
	return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2] + v1[3]*v2[3];
}
float RichVec4::Length(void) const
{
	return sqrtf(LengthSq());
}
float RichVec4::LengthSq(void) const
{
	return (v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]);
}
float RichVec4::Normalize(void)
{
	float l = Length();
	if (l != 0.0f)
	{
		float lm = 1.0f/l;
		*this *= lm;
	}
	return l;
}
RichVec4 RichVec4::Normalized(void) const
{
	float l = Length();
	if (l != 0.0f)
	{
		float lm = 1.0f/l;
		return RichVec4(v[0]*lm, v[1]*lm, v[2]*lm, v[3]*lm);
	}
	return *this;
}
void RichVec4::Lerp(const RichVec4 &vec, const float frac)
{
	v[0] = MATH_PREFIX Math_LinearLerp(v[0], vec.v[0], frac);
	v[1] = MATH_PREFIX Math_LinearLerp(v[1], vec.v[1], frac);
	v[2] = MATH_PREFIX Math_LinearLerp(v[2], vec.v[2], frac);
	v[3] = MATH_PREFIX Math_LinearLerp(v[3], vec.v[3], frac);
}
void RichVec4::Lerp(const RichVec4 &vecA, const RichVec4 &vecB, const float frac)
{
	v[0] = MATH_PREFIX Math_LinearLerp(vecA.v[0], vecB.v[0], frac);
	v[1] = MATH_PREFIX Math_LinearLerp(vecA.v[1], vecB.v[1], frac);
	v[2] = MATH_PREFIX Math_LinearLerp(vecA.v[2], vecB.v[2], frac);
	v[3] = MATH_PREFIX Math_LinearLerp(vecA.v[3], vecB.v[3], frac);
}

RichVec3 RichVec4::ToVec3(void) const
{
	return RichVec3(v[0], v[1], v[2]);
}



//===========================================
//RichVec2 implementation
//===========================================
RichVec2::RichVec2(void)
{
	v[0] = 0.0f;
	v[1] = 0.0f;
}
RichVec2::RichVec2(const float x, const float y)
{
	v[0] = x;
	v[1] = y;
}
RichVec2::RichVec2(const float *xy)
{
	v[0] = xy[0];
	v[1] = xy[1];
}

float &RichVec2::operator[](int idx)
{
	assert(idx >= 0 && idx < 2);
	return v[idx];
}
float RichVec2::operator[](int idx) const
{
	assert(idx >= 0 && idx < 2);
	return v[idx];
}
RichVec2 &RichVec2::operator=(const RichVec2 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	return *this;
}
bool RichVec2::operator==(const RichVec2 &vec) const
{
	return (v[0] == vec.v[0] && v[1] == vec.v[1]);
}
bool RichVec2::operator!=(const RichVec2 &vec) const
{
	return (v[0] != vec.v[0] || v[1] != vec.v[1]);
}
RichVec2 RichVec2::operator+(const RichVec2 &vec) const
{
	return RichVec2(v[0]+vec.v[0], v[1]+vec.v[1]);
}
RichVec2 &RichVec2::operator+=(const RichVec2 &vec)
{
	v[0] += vec.v[0];
	v[1] += vec.v[1];
	return *this;
}
RichVec2 RichVec2::operator-(void) const
{
	return RichVec2(-v[0], -v[1]);
}
RichVec2 RichVec2::operator-(const RichVec2 &vec) const
{
	return RichVec2(v[0]-vec.v[0], v[1]-vec.v[1]);
}
RichVec2 &RichVec2::operator-=(const RichVec2 &vec)
{
	v[0] -= vec.v[0];
	v[1] -= vec.v[1];
	return *this;
}
RichVec2 RichVec2::operator*(const RichVec2 &vec) const
{
	return RichVec2(v[0]*vec.v[0], v[1]*vec.v[1]);
}
RichVec2 &RichVec2::operator*=(const RichVec2 &vec)
{
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	return *this;
}
RichVec2 RichVec2::operator*(const float &f) const
{
	return RichVec2(v[0]*f, v[1]*f);
}
RichVec2 &RichVec2::operator*=(const float &f)
{
	v[0] *= f;
	v[1] *= f;
	return *this;
}
RichVec2 RichVec2::operator/(const RichVec2 &vec) const
{
	return RichVec2(v[0]/vec.v[0], v[1]/vec.v[1]);
}
RichVec2 &RichVec2::operator/=(const RichVec2 &vec)
{
	v[0] /= vec.v[0];
	v[1] /= vec.v[1];
	return *this;
}

void RichVec2::ChangeEndian(void)
{
	LITTLE_BIG_SWAP(v[0]);
	LITTLE_BIG_SWAP(v[1]);
}

float RichVec2::Dot(const RichVec2 &vec) const
{
	const float *v1 = v;
	const float *v2 = vec.v;
	return v1[0]*v2[0] + v1[1]*v2[1];
}
//returns a positive value if OAB makes a counter-clockwise turn, negative for clockwise turn,
//and zero if the points are collinear.
float RichVec2::Cross(const RichVec2 &vec, const RichVec2 &point) const
{
	return (vec[0] - v[0]) * (point[1] - v[1]) - (vec[1] - v[1]) * (point[0] - v[0]);
}
float RichVec2::Cross(const RichVec2 &vec) const
{
	return v[0]*vec[1] - v[1]*vec[0];
}
float RichVec2::Length(void) const
{
	return sqrtf(LengthSq());
}
float RichVec2::LengthSq(void) const
{
	return (v[0]*v[0] + v[1]*v[1]);
}
float RichVec2::Normalize(void)
{
	float l = Length();
	if (l != 0.0f)
	{
		float lm = 1.0f/l;
		*this *= lm;
	}
	return l;
}
RichVec2 RichVec2::Normalized(void) const
{
	float l = Length();
	if (l != 0.0f)
	{
		float lm = 1.0f/l;
		return RichVec2(v[0]*lm, v[1]*lm);
	}
	return *this;
}
RichVec2 RichVec2::InverseOrZero() const
{
	return RichVec2((v[0] != 0.0f) ? 1.0f / v[0] : 0.0f, (v[1] != 0.0f) ? 1.0f / v[1] : 0.0f);
}
void RichVec2::Lerp(const RichVec2 &vec, const float frac)
{
	v[0] = MATH_PREFIX Math_LinearLerp(v[0], vec.v[0], frac);
	v[1] = MATH_PREFIX Math_LinearLerp(v[1], vec.v[1], frac);
}
void RichVec2::Lerp(const RichVec2 &vecA, const RichVec2 &vecB, const float frac)
{
	v[0] = MATH_PREFIX Math_LinearLerp(vecA.v[0], vecB.v[0], frac);
	v[1] = MATH_PREFIX Math_LinearLerp(vecA.v[1], vecB.v[1], frac);
}


//===========================================
//RichVecH2 implementation
//===========================================
RichVecH2::RichVecH2(void)
{
	v[0] = 0.0;
	v[1] = 0.0;
}
RichVecH2::RichVecH2(const double x, const double y)
{
	v[0] = x;
	v[1] = y;
}
RichVecH2::RichVecH2(const double *xy)
{
	v[0] = xy[0];
	v[1] = xy[1];
}

double &RichVecH2::operator[](int idx)
{
	assert(idx >= 0 && idx < 2);
	return v[idx];
}
double RichVecH2::operator[](int idx) const
{
	assert(idx >= 0 && idx < 2);
	return v[idx];
}
RichVecH2 &RichVecH2::operator=(const RichVecH2 &vec)
{
	v[0] = vec.v[0];
	v[1] = vec.v[1];
	return *this;
}
bool RichVecH2::operator==(const RichVecH2 &vec) const
{
	return (v[0] == vec.v[0] && v[1] == vec.v[1]);
}
bool RichVecH2::operator!=(const RichVecH2 &vec) const
{
	return (v[0] != vec.v[0] || v[1] != vec.v[1]);
}
RichVecH2 RichVecH2::operator+(const RichVecH2 &vec) const
{
	return RichVecH2(v[0]+vec.v[0], v[1]+vec.v[1]);
}
RichVecH2 &RichVecH2::operator+=(const RichVecH2 &vec)
{
	v[0] += vec.v[0];
	v[1] += vec.v[1];
	return *this;
}
RichVecH2 RichVecH2::operator-(void) const
{
	return RichVecH2(-v[0], -v[1]);
}
RichVecH2 RichVecH2::operator-(const RichVecH2 &vec) const
{
	return RichVecH2(v[0]-vec.v[0], v[1]-vec.v[1]);
}
RichVecH2 &RichVecH2::operator-=(const RichVecH2 &vec)
{
	v[0] -= vec.v[0];
	v[1] -= vec.v[1];
	return *this;
}
RichVecH2 RichVecH2::operator*(const RichVecH2 &vec) const
{
	return RichVecH2(v[0]*vec.v[0], v[1]*vec.v[1]);
}
RichVecH2 &RichVecH2::operator*=(const RichVecH2 &vec)
{
	v[0] *= vec.v[0];
	v[1] *= vec.v[1];
	return *this;
}
RichVecH2 RichVecH2::operator*(const double &f) const
{
	return RichVecH2(v[0]*f, v[1]*f);
}
RichVecH2 &RichVecH2::operator*=(const double &f)
{
	v[0] *= f;
	v[1] *= f;
	return *this;
}
RichVecH2 RichVecH2::operator/(const RichVecH2 &vec) const
{
	return RichVecH2(v[0]/vec.v[0], v[1]/vec.v[1]);
}
RichVecH2 &RichVecH2::operator/=(const RichVecH2 &vec)
{
	v[0] /= vec.v[0];
	v[1] /= vec.v[1];
	return *this;
}

void RichVecH2::ChangeEndian(void)
{
	LITTLE_BIG_SWAP(v[0]);
	LITTLE_BIG_SWAP(v[1]);
}

double RichVecH2::Dot(const RichVecH2 &vec) const
{
	const double *v1 = v;
	const double *v2 = vec.v;
	return v1[0]*v2[0] + v1[1]*v2[1];
}
//returns a positive value if OAB makes a counter-clockwise turn, negative for clockwise turn,
//and zero if the points are collinear.
double RichVecH2::Cross(const RichVecH2 &vec, const RichVecH2 &point) const
{
	return (vec[0] - v[0]) * (point[1] - v[1]) - (vec[1] - v[1]) * (point[0] - v[0]);
}
double RichVecH2::Cross(const RichVecH2 &vec) const
{
	return v[0]*vec[1] - v[1]*vec[0];
}
double RichVecH2::Length(void) const
{
	return sqrt(LengthSq());
}
double RichVecH2::LengthSq(void) const
{
	return (v[0]*v[0] + v[1]*v[1]);
}
double RichVecH2::Normalize(void)
{
	double l = Length();
	if (l != 0.0)
	{
		double lm = 1.0/l;
		*this *= lm;
	}
	return l;
}
RichVecH2 RichVecH2::Normalized(void) const
{
	double l = Length();
	if (l != 0.0)
	{
		double lm = 1.0/l;
		return RichVecH2(v[0]*lm, v[1]*lm);
	}
	return *this;
}
RichVecH2 RichVecH2::InverseOrZero() const
{
	return RichVecH2((v[0] != 0.0) ? 1.0 / v[0] : 0.0, (v[1] != 0.0) ? 1.0 / v[1] : 0.0);
}
RichVecH2 RichVecH2::PointOnSegment(const RichVecH2 &v0, const RichVecH2 &v1) const
{
	const RichVecH2 lineVec = v1 - v0;
	const double lineLenSq = lineVec.LengthSq();
	if (lineLenSq != 0.0)
	{
		const RichVecH2 pointVec = *this - v0;
		return v0 + lineVec * std::min<double>(std::max<double>((pointVec.Dot(lineVec) / lineLenSq), 0.0), 1.0);
	}

	return v0;
}

//=========================================
//Misc utility classes
//=========================================
RichFileWrap::RichFileWrap(const wchar_t *filename, noeFSMode_e mode, noeRAPI_t *rapi)
{
	assert(rapi);
	m_rapi = rapi;
	m_file = rapi->Noesis_FSOpen(filename, mode);
	m_close = true;
}
RichFileWrap::RichFileWrap(void *file, noeRAPI_t *rapi, bool close)
{
	assert(rapi && file);
	m_rapi = rapi;
	m_file = file;
	m_close = close;
}
RichFileWrap::~RichFileWrap()
{
	if (m_file && m_close)
	{
		m_rapi->Noesis_FSClose(m_file);
		m_file = NULL;
	}
}

bool RichFileWrap::IsValid(void)
{
	return (m_file && m_rapi);
}
void *RichFileWrap::GetFile(void)
{
	return m_file;
}
__int64 RichFileWrap::GetSize(void)
{
	assert(m_file && m_rapi);
	return m_rapi->Noesis_FSGetSize(m_file);
}
void RichFileWrap::Seek(__int64 pos, bool seekRelative)
{
	assert(m_file && m_rapi);
	m_rapi->Noesis_FSSeek(m_file, pos, seekRelative);
}
__int64 RichFileWrap::Tell(void)
{
	assert(m_file && m_rapi);
	return m_rapi->Noesis_FSTell(m_file);
}
bool RichFileWrap::CheckEOF(void)
{
	assert(m_file && m_rapi);
	return m_rapi->Noesis_FSEOF(m_file);
}
__int64 RichFileWrap::Read(void *dstBuf, __int64 size)
{
	assert(m_file && m_rapi);
	return m_rapi->Noesis_FSRead(dstBuf, size, m_file);
}
__int64 RichFileWrap::Write(const void *srcBuf, __int64 size)
{
	assert(m_file && m_rapi);
	return m_rapi->Noesis_FSWrite(srcBuf, size, m_file);
}


RichMemFileWrap::RichMemFileWrap(BYTE *buf, __int64 bufSize)
{
	m_buf = buf;
	m_bufSize = bufSize;
	m_bufPtr = 0;
}
RichMemFileWrap::~RichMemFileWrap()
{
}

bool RichMemFileWrap::IsValid(void)
{
	return (m_buf && m_bufSize > 0);
}
BYTE *RichMemFileWrap::GetBuffer(void)
{
	return m_buf;
}
__int64 RichMemFileWrap::GetSize(void)
{
	return m_bufSize;
}
void RichMemFileWrap::Seek(__int64 pos, bool seekRelative)
{
	if (seekRelative)
	{
		m_bufPtr += pos;
	}
	else
	{
		m_bufPtr = pos;
	}
	assert(m_bufPtr >= 0 && m_bufPtr <= m_bufSize);
}
__int64 RichMemFileWrap::Tell(void)
{
	return m_bufPtr;
}
bool RichMemFileWrap::CheckEOF(void)
{
	return (m_bufPtr >= m_bufSize);
}
__int64 RichMemFileWrap::Read(void *dstBuf, __int64 size)
{
	assert(m_buf);
	if (!m_buf || m_bufSize <= 0)
	{
		return 0;
	}
	__int64 r = size;
	if (m_bufPtr+r > m_bufSize)
	{
		r -= ((m_bufPtr+r)-m_bufSize);
	}
	if (r <= 0)
	{
		return 0;
	}
	memcpy(dstBuf, m_buf+m_bufPtr, (size_t)r);
	m_bufPtr += r;
	return r;
}
__int64 RichMemFileWrap::Write(const void *srcBuf, __int64 size)
{
	assert(m_buf);
	if (!m_buf || m_bufSize <= 0)
	{
		return 0;
	}
	__int64 r = size;
	if (m_bufPtr+r > m_bufSize)
	{
		r -= ((m_bufPtr+r)-m_bufSize);
	}
	if (r <= 0)
	{
		return 0;
	}
	memcpy(m_buf+m_bufPtr, srcBuf, (size_t)r);
	m_bufPtr += r;
	return r;
}

#ifdef _NOESIS_INTERNAL
#define STREAM_PREFIX

cntStream_t *Stream_Alloc(void *buffer, int size);
cntStream_t *Stream_AllocFixed(int size);
void Stream_Free(cntStream_t *st);
void Stream_WriteBits(cntStream_t *st, const void *buf, int size);
void Stream_WriteBytes(cntStream_t *st, const void *buf, int size);
bool Stream_ReadBits(cntStream_t *st, void *buf, int size);
bool Stream_ReadRevBits(cntStream_t *st, void *buf, int size);
bool Stream_ReadBytes(cntStream_t *st, void *buf, int size);
void Stream_WriteBool(cntStream_t *st, bool val);
void Stream_WriteInt(cntStream_t *st, int val);
void Stream_WriteFloat(cntStream_t *st, float val);
void Stream_WriteString(cntStream_t *st, const char *str, bool noTerminate = false);
void Stream_WriteWString(cntStream_t *st, const wchar_t *str, bool noTerminate = false);
bool Stream_ReadBool(cntStream_t *st);
int Stream_ReadInt(cntStream_t *st);
float Stream_ReadFloat(cntStream_t *st);
void Stream_ReadString(cntStream_t *st, char *str, int maxSize);
void *Stream_Buffer(cntStream_t *st);
int Stream_Size(cntStream_t *st);
void Stream_SetOffset(cntStream_t *st, int offset);
int Stream_GetOffset(cntStream_t *st);
void Stream_SetBitOffset(const int byteOffset, const int bitOffset, cntStream_t *st);
void Stream_GetBitOffset(int *pByteOffset, int *pBitOffset, cntStream_t *st);
void Stream_SetFlags(cntStream_t *st, int flags);
int Stream_GetFlags(cntStream_t *st);
void Steam_WriteToFile(cntStream_t *st, FILE *f);
#else
#define STREAM_PREFIX g_nfn->
#endif

RichBitStream::RichBitStream()
{
	NFN_CHECK
	m_stream = STREAM_PREFIX Stream_Alloc(NULL, 1048576);
}
RichBitStream::RichBitStream(void *data, int dataSize)
{
	NFN_CHECK
	m_stream = STREAM_PREFIX Stream_Alloc(data, dataSize);
}
RichBitStream::~RichBitStream()
{
	if (m_stream)
	{
		STREAM_PREFIX Stream_Free(m_stream);
		m_stream = NULL;
	}
}

void RichBitStream::WriteBits(const void *src, int numBits)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteBits(m_stream, src, numBits);
}
void RichBitStream::WriteBits(int val, int numBits)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteBits(m_stream, &val, numBits);
}
void RichBitStream::WriteBytes(const void *src, int size)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteBytes(m_stream, src, size);
}
bool RichBitStream::ReadBits(void *dst, int numBits)
{
	assert(m_stream);
	return STREAM_PREFIX Stream_ReadBits(m_stream, dst, numBits);
}
int RichBitStream::ReadBits(int numBits)
{
	assert(m_stream);
	int rv = 0;
	STREAM_PREFIX Stream_ReadBits(m_stream, &rv, numBits);
	return rv;
}
int RichBitStream::ReadRevBits(int numBits)
{
	assert(m_stream);
	int rv = 0;
	STREAM_PREFIX Stream_ReadRevBits(m_stream, &rv, numBits);
	return rv;
}
bool RichBitStream::ReadBytes(void *dst, int size)
{
	assert(m_stream);
	return STREAM_PREFIX Stream_ReadBytes(m_stream, dst, size);
}

void RichBitStream::WriteBool(bool val)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteBool(m_stream, val);
}
void RichBitStream::WriteByte(unsigned char b)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteBytes(m_stream, &b, 1);
}
void RichBitStream::WriteInt(int val)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteInt(m_stream, val);
}
void RichBitStream::WriteFloat(float val)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteFloat(m_stream, val);
}
void RichBitStream::WriteString(const char *str)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteString(m_stream, str, true);
}
void RichBitStream::WriteStringVA(const char *fmt, ...)
{
	assert(m_stream);
	char finalString[16384];
	va_list args;
	va_start(args, fmt);
	vsnprintf_s(finalString, 16384, 16383, fmt, args);
	va_end(args);
	STREAM_PREFIX Stream_WriteString(m_stream, finalString, true);
}
void RichBitStream::WriteWStringVA(const wchar_t *fmt, ...)
{
	assert(m_stream);
	wchar_t finalString[16384];
	va_list args;
	va_start(args, fmt);
	_vsnwprintf_s(finalString, 16384, 16383, fmt, args);
	va_end(args);
	STREAM_PREFIX Stream_WriteWString(m_stream, finalString, true);
}
void RichBitStream::WriteStringNulTerm(const char *str)
{
	assert(m_stream);
	STREAM_PREFIX Stream_WriteString(m_stream, str, false);
}

bool RichBitStream::ReadBool(void)
{
	assert(m_stream);
	return STREAM_PREFIX Stream_ReadBool(m_stream);
}
unsigned char RichBitStream::ReadByte(void)
{
	assert(m_stream);
	unsigned char b = 0;
	STREAM_PREFIX Stream_ReadBytes(m_stream, &b, 1);
	return b;
}
int RichBitStream::ReadInt(void)
{
	assert(m_stream);
	return STREAM_PREFIX Stream_ReadInt(m_stream);
}
float RichBitStream::ReadFloat(void)
{
	assert(m_stream);
	return STREAM_PREFIX Stream_ReadFloat(m_stream);
}
void RichBitStream::ReadString(char *str, int maxSize)
{
	assert(m_stream);
	return STREAM_PREFIX Stream_ReadString(m_stream, str, maxSize);
}

void *RichBitStream::GetBuffer(void)
{
	assert(m_stream);
	return STREAM_PREFIX Stream_Buffer(m_stream);
}
const void *RichBitStream::GetBuffer(void) const
{
	assert(m_stream);
	return STREAM_PREFIX Stream_Buffer(m_stream);
}
int RichBitStream::GetSize(void) const
{
	assert(m_stream);
	return STREAM_PREFIX Stream_Size(m_stream);
}

void RichBitStream::SetOffset(int ofs)
{
	assert(m_stream);
	STREAM_PREFIX Stream_SetOffset(m_stream, ofs);
}
int RichBitStream::GetOffset(void) const
{
	assert(m_stream);
	return STREAM_PREFIX Stream_GetOffset(m_stream);
}

void RichBitStream::SetBitOffset(int byteOffset, int bitOffset)
{
	assert(m_stream);
	STREAM_PREFIX Stream_SetBitOffset(byteOffset, bitOffset, m_stream);
}
void RichBitStream::GetBitOffset(int *pByteOffset, int *pBitOffset) const
{
	assert(m_stream);
	STREAM_PREFIX Stream_GetBitOffset(pByteOffset, pBitOffset, m_stream);
}

void RichBitStream::SetFlags(int flags)
{
	assert(m_stream);
	STREAM_PREFIX Stream_SetFlags(m_stream, flags);
}
int RichBitStream::GetFlags(void) const
{
	assert(m_stream);
	return STREAM_PREFIX Stream_GetFlags(m_stream);
}

bool RichBitStream::AllocWriteSpace(int size, bool tryNonFixed)
{
	cntStream_t *testStream = STREAM_PREFIX Stream_AllocFixed(size);
	if (!testStream && !tryNonFixed)
	{
		return false;
	}

	if (m_stream)
	{
		STREAM_PREFIX Stream_Free(m_stream);
	}
	m_stream = testStream;
	if (!m_stream)
	{
		m_stream = STREAM_PREFIX Stream_Alloc(NULL, size);
		if (!m_stream)
		{
			m_stream = STREAM_PREFIX Stream_Alloc(NULL, 1048576);
			return false;
		}
	}
	return true;
}

void RichBitStream::TakeOwnershipFrom(RichBitStream *other)
{
	//free our internal stream, and take ownership of the other stream's internal stream
	if (m_stream)
	{
		STREAM_PREFIX Stream_Free(m_stream);
		m_stream = NULL;
	}
	m_stream = other->m_stream;
	//the other stream is empty now
	other->m_stream = NULL;
}

#ifdef _NOESIS_INTERNAL //this function is intended only for internal use
void RichBitStream::WriteToFile(FILE *f)
{
	return Steam_WriteToFile(m_stream, f);
}
#endif

//======================================================
//Misc utility functions and data
//======================================================

//FIXME - this shit is terrible (in implementation too), and these are bad naming conventions. namespace or at least common prefix.

modelMatrix_t g_identityMatrix =
{
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, 0.0f, 0.0f}
};

fourxMatrix_t g_identityMatrix4x4 =
{
	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
};

double g_dbPI = 3.14159265358979323846;
float g_flPI = (float)g_dbPI;
float g_flDegToRad = g_flPI / 180.0f;
float g_flRadToDeg = 180.0f / g_flPI;

void SafeStrCopy(char *dst, int dstLen, const char *src)
{
	strncpy_s(dst, dstLen, src, dstLen-1);
	dst[dstLen-1] = 0;
}

int AlignInt(int val, int alignTo)
{
	int d = val%alignTo;
	if (d != 0)
	{
		val += (alignTo-d);		
	}
	return val;
}

//get signed bits
int SignedBits(int val, int bits)
{
	int signVal = (1<<(bits-1));
	bool isSign = !!(val & signVal);
	if (isSign)
	{
		val ^= signVal;
		val = -(signVal-val);
	}

	return val;
}

WORD GetBigWord(WORD w)
{
	WORD r = w;
	BYTE *m = (BYTE *)&r;
	BYTE t = m[0];
	m[0] = m[1];
	m[1] = t;
	return r;
}

WORD GetBigWordRaw(BYTE *b)
{
	return GetBigWord(*((WORD *)b));
}

int GetBigInt(int dw)
{
	int r = dw;
	BYTE *m = (BYTE *)&r;
	BYTE t[2] = {m[0], m[1]};
	m[0] = m[3];
	m[1] = m[2];
	m[2] = t[1];
	m[3] = t[0];
	return r;
}

int GetBigIntRaw(BYTE *b)
{
	return GetBigInt(*((int *)b));
}

void LittleBigSwap(void *in, int numBytes)
{
	BYTE *data = (BYTE *)in;
	if (numBytes == 8)
	{
		BYTE t[4] = {data[0], data[1], data[2], data[3]};
		data[0] = data[7];
		data[1] = data[6];
		data[2] = data[5];
		data[3] = data[4];
		data[4] = t[3];
		data[5] = t[2];
		data[6] = t[1];
		data[7] = t[0];
	}
	else if (numBytes == 4)
	{
		BYTE t[2] = {data[0], data[1]};
		data[0] = data[3];
		data[1] = data[2];
		data[2] = t[1];
		data[3] = t[0];
	}
	else if (numBytes == 2)
	{
		BYTE t = data[0];
		data[0] = data[1];
		data[1] = t;
	}
}

noeUDCommonMeshHierarchy_t *GetUDCommonMeshHierarchy(sharedMesh_t *mesh)
{
	//check for userstream hierarchy data
	if (mesh->userStreams)
	{
		for (int usIndex = 0; usIndex < mesh->numUserStreams; usIndex++)
		{
			modelUserStream_t *ustream = mesh->userStreams+usIndex;
			if (ustream->name && ustream->dataSize == sizeof(noeUDCommonMeshHierarchy_t) &&
				!_stricmp(ustream->name, NOESIS_UDCOMMON_MESH_HIERARCHY_STREAMNAME))
			{
				noeUDCommonMeshHierarchy_t *pHie = (noeUDCommonMeshHierarchy_t *)ustream->data;
				if (pHie->version == NOESIS_UDCOMMON_MESH_HIERARCHY_VERSION)
				{
					return pHie;
				}
			}
		}
	}
	return NULL;
}

size_t fseekread(__int64 ofs, void *dst, size_t elementSize, size_t count, FILE *file)
{
	_fseeki64(file, ofs, SEEK_SET);
	return fread(dst, elementSize, count, file);
}
int freadint(FILE *file, bool bigEnd)
{
	int i = 0;
	fread(&i, sizeof(int), 1, file);
	if (bigEnd)
	{
		LITTLE_BIG_SWAP(i);
	}
	return i;
}
int fseekreadint(__int64 ofs, FILE *file, bool bigEnd)
{
	_fseeki64(file, ofs, SEEK_SET);
	return freadint(file, bigEnd);
}
short freadshort(FILE *file, bool bigEnd)
{
	short i = 0;
	fread(&i, sizeof(short), 1, file);
	if (bigEnd)
	{
		LITTLE_BIG_SWAP(i);
	}
	return i;
}
short fseekreadshort(__int64 ofs, FILE *file, bool bigEnd)
{
	_fseeki64(file, ofs, SEEK_SET);
	return freadshort(file, bigEnd);
}
