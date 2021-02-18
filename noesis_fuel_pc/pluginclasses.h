#ifndef _NOESIS_PLUGIN_CLASSES_H
#define _NOESIS_PLUGIN_CLASSES_H

#pragma pack(push, 1)

//=========================================
//RichMath classes
//=========================================

class RichAngles;
class RichVec3;
class RichVec4;
class RichMat43;
class RichMat44;
class RichQuat;

class RichAngles
{
public:
	RichAngles(void);
	RichAngles(const float pitch, const float yaw, const float roll);
	RichAngles(const float *angles);
	RichAngles(const float *radians, bool scaleToDegrees);

	//access
	float					&operator[](int idx);
	float					operator[](int idx) const;
	//set/compare
	RichAngles				&operator=(const RichAngles &angles);
	bool					operator==(const RichAngles &angles) const;
	bool					operator!=(const RichAngles &angles) const;
	//add
	RichAngles				operator+(const RichAngles &angles) const;
	RichAngles				&operator+=(const RichAngles &angles);
	//subtract
	RichAngles				operator-(void) const;
	RichAngles				operator-(const RichAngles &angles) const;
	RichAngles				&operator-=(const RichAngles &angles);
	//multiply
	RichAngles				operator*(const RichAngles &angles) const;
	RichAngles				&operator*=(const RichAngles &angles);
	RichAngles				operator*(const float &f) const;
	RichAngles				&operator*=(const float &f);
	//divide
	RichAngles				operator/(const RichAngles &angles) const;
	RichAngles				&operator/=(const RichAngles &angles);

	void					ChangeEndian(void);

	//general operations
	void					Mod(float f);
	void					Normalize360(void);
	void					Normalize180(void);
	void					AngleVectors(RichVec3 *fwd, RichVec3 *right, RichVec3 *up) const;
	void					Lerp(const RichAngles &angles, const float frac);
	void					Lerp(const RichAngles &anglesA, const RichAngles &anglesB, const float frac);
	void					ALerp(const RichAngles &angles, const float degrees);
	void					ALerp(const RichAngles &anglesA, const RichAngles &anglesB, const float degrees);

	//conversion
	RichVec3				ToVec3(void) const;
	RichMat43				ToMat43(void) const;
	RichMat43				ToMat43_XYZ(bool yFlip = true) const; //convert using xyz convention
	RichQuat				ToQuat(void) const;

	float					a[3];
};

class RichMat43
{
public:
	RichMat43(void);
	RichMat43(const RichVec3 &r0, const RichVec3 &r1, const RichVec3 &r2, const RichVec3 &trans);
	RichMat43(const modelMatrix_t &mat);
	RichMat43(const float *mat);

	//access
	RichVec3				&operator[](int idx);
	const RichVec3			&operator[](int idx) const;
	//set/compare
	RichMat43				&operator=(const RichMat43 &mat);
	RichMat43				&operator=(const modelMatrix_t &mat);
	bool					operator==(const RichMat43 &mat) const;
	bool					operator!=(const RichMat43 &mat) const;
	//add
	RichMat43				operator+(const RichMat43 &mat) const;
	RichMat43				&operator+=(const RichMat43 &mat);
	//subtract
	RichMat43				operator-(void) const;
	RichMat43				operator-(const RichMat43 &mat) const;
	RichMat43				&operator-=(const RichMat43 &mat);
	//matrix multiply/transform
	RichMat43				operator*(const RichMat43 &mat) const;
	RichMat43				&operator*=(const RichMat43 &mat);
	RichVec3				operator*(const RichVec3 &vec) const;
	RichVec4				operator*(const RichVec4 &vec) const;

	void					ChangeEndian(void);

	//general operations
	RichVec3				TransformPoint(const RichVec3 &vec) const;
	RichVec3				TransformNormal(const RichVec3 &vec) const;
	RichVec4				TransformVec4(const RichVec4 &vec) const;
	RichMat43				GetTranspose(void) const;
	void					Transpose(void);
	RichMat43				GetInverse(void) const;
	void					Inverse(void);
	RichMat43				GetOrthogonalize(bool keepScale = false, bool keepFlip = true, bool straightCross = false) const;
	void					Orthogonalize(bool keepScale = false, bool keepFlip = true, bool straightCross = false);
	bool					IsSkewed(void);
	void					Rotate(float degrees, float x, float y, float z, bool transposeRot = false);
	void					Rotate(float degrees, float *xyz, bool transposeRot = false);
	void					Translate(float x, float y, float z);
	void					Translate(float *xyz);
	void					Lerp(const RichMat43 &postMat, float lerpFrac, bool nonUniform = true, bool orthogonalize = false);
	void					Lerp(const RichMat43 &preMat, const RichMat43 &postMat, float lerpFrac, bool nonUniform = true, bool orthogonalize = false);
	void					SLerp(const RichMat43 &postMat, float lerpFrac, bool nonUniform = true);
	void					SLerp(const RichMat43 &preMat, const RichMat43 &postMat, float lerpFrac, bool nonUniform = true);
	void					TransformQST(const RichVec3 *pScalingCenter, const RichQuat *pScalingRotation,
											const RichVec3 *pScaling, const RichVec3 *pRotationCenter, const RichQuat *pRotation,
											const RichVec3 *pTranslation);

	//conversion
	RichQuat				ToQuat(void) const;
	RichAngles				ToAngles(void) const;
	RichAngles				ToAngles_Axis(int *axOrder) const;
	RichMat44				ToMat44(void) const;

	modelMatrix_t			m;
};

//4x4 matrices use opengl conventions (rotation is transposed from 4x3)
class RichMat44
{
public:
	RichMat44(void);
	RichMat44(const RichVec4 &r0, const RichVec4 &r1, const RichVec4 &r2, const RichVec4 &r3);
	RichMat44(const fourxMatrix_t &mat);
	RichMat44(const float *mat);

	//access
	RichVec4				&operator[](int idx);
	const RichVec4			&operator[](int idx) const;
	//set/compare
	RichMat44				&operator=(const RichMat44 &mat);
	RichMat44				&operator=(const fourxMatrix_t &mat);
	bool					operator==(const RichMat44 &mat) const;
	bool					operator!=(const RichMat44 &mat) const;
	//add
	RichMat44				operator+(const RichMat44 &mat) const;
	RichMat44				&operator+=(const RichMat44 &mat);
	//subtract
	RichMat44				operator-(void) const;
	RichMat44				operator-(const RichMat44 &mat) const;
	RichMat44				&operator-=(const RichMat44 &mat);
	//matrix multiply/transform
	RichMat44				operator*(const RichMat44 &mat) const;
	RichMat44				&operator*=(const RichMat44 &mat);
	RichVec4				operator*(const RichVec4 &vec) const;

	void					ChangeEndian(void);

	//general operations
	RichVec4				TransformVec4(const RichVec4 &vec) const;
	RichVec3				TransformNormal(const RichVec3 &vec) const;
	RichMat44				GetTranspose(void) const;
	void					Transpose(void);
	RichMat44				GetInverse(void) const;
	void					Inverse(void);
	void					Rotate(float degrees, float x, float y, float z);
	void					Rotate(float degrees, float *xyz);
	void					Translate(float x, float y, float z);
	void					Translate(float *xyz);

	//conversion
	RichMat43				ToMat43(void) const;

	fourxMatrix_t			m;
};

class RichQuat
{
public:
	RichQuat(void);
	RichQuat(const float x, const float y, const float z, const float w);
	RichQuat(const float *xyzw);
	RichQuat(const float *xyz, const bool noW);

	//access
	float					&operator[](int idx);
	float					operator[](int idx) const;
	//set/compare
	RichQuat				&operator=(const RichQuat &quat);
	bool					operator==(const RichQuat &quat) const;
	bool					operator!=(const RichQuat &quat) const;
	//add
	RichQuat				operator+(const RichQuat &quat) const;
	RichQuat				&operator+=(const RichQuat &quat);
	//subtract
	RichQuat				operator-(void) const;
	RichQuat				operator-(const RichQuat &quat) const;
	RichQuat				&operator-=(const RichQuat &quat);
	//multiply
	RichQuat				operator*(const RichQuat &quat) const;
	RichQuat				&operator*=(const RichQuat &quat);
	RichVec3				operator*(const RichVec3 &vec) const;
	RichQuat				operator*(const float &f) const;
	RichQuat				&operator*=(const float &f);

	void					ChangeEndian(void);

	//general operations
	RichVec3				TransformPoint(const RichVec3 &vec) const;
	RichQuat				GetTranspose(void);
	void					Transpose(void);
	float					Length(void) const;
	float					Normalize(void);
	void					Lerp(const RichQuat &quat, const float frac);
	void					Lerp(const RichQuat &quatA, const RichQuat &quatB, const float frac);
	void					SLerp(const RichQuat &quat, const float frac);
	void					SLerp(const RichQuat &quatA, const RichQuat &quatB, const float frac);

	//conversion
	void					FromQuat3(const float *quat); //derive the w
	void					ToQuat3(float *quat) const;
	RichMat43				ToMat43(bool transposed = false) const;
	RichAngles				ToAngles(void) const;

	float					q[4];
};

class RichVec3
{
public:
	RichVec3(void);
	RichVec3(const float x, const float y, const float z);
	RichVec3(const float *xyz);

	//access
	float					&operator[](int idx);
	float					operator[](int idx) const;
	//set/compare
	RichVec3				&operator=(const RichVec3 &vec);
	bool					operator==(const RichVec3 &vec) const;
	bool					operator!=(const RichVec3 &vec) const;
	//add
	RichVec3				operator+(const RichVec3 &vec) const;
	RichVec3				&operator+=(const RichVec3 &vec);
	//subtract
	RichVec3				operator-(void) const;
	RichVec3				operator-(const RichVec3 &vec) const;
	RichVec3				&operator-=(const RichVec3 &vec);
	//multiply
	RichVec3				operator*(const RichVec3 &vec) const;
	RichVec3				&operator*=(const RichVec3 &vec);
	RichVec3				operator*(const float &f) const;
	RichVec3				&operator*=(const float &f);
	//divide
	RichVec3				operator/(const RichVec3 &vec) const;
	RichVec3				&operator/=(const RichVec3 &vec);

	void					ChangeEndian(void);

	//general operations
	float					Dot(const RichVec3 &vec) const;
	RichVec3				Cross(const RichVec3 &vec) const;
	void					Cross(const RichVec3 &vecA, const RichVec3 &vecB);
	float					Length(void) const;
	float					LengthSq(void) const;
	float					Normalize(void);
	RichVec3				Normalized(void) const;
	void					Lerp(const RichVec3 &vec, const float frac);
	void					Lerp(const RichVec3 &vecA, const RichVec3 &vecB, const float frac);
	void					BarycentricCoordinates(const RichVec3 &v0, const RichVec3 &v1, const RichVec3 &v2, const RichVec3 &point);
	void					SLerp(const RichVec3 &vec, const float frac);
	void					SLerp(const RichVec3 &vecA, const RichVec3 &vecB, const float frac);
	void					OrthoBasis(RichVec3 *pRightOut, RichVec3 *pUpOut) const;
	void					Min(const RichVec3 &vec);
	void					Min(const RichVec3 &vecA, const RichVec3 &vecB);
	void					Max(const RichVec3 &vec);
	void					Max(const RichVec3 &vecA, const RichVec3 &vecB);

	//conversion
	RichAngles				ToAngles(void) const;
	RichVec4				ToVec4(void) const;
	RichMat43				ToMat43(void) const;
	RichMat43				ToMat43Z(void) const;

	float					v[3];
};

class RichVecH3
{
public:
	RichVecH3(void);
	RichVecH3(const double x, const double y, const double z);
	RichVecH3(const double *xyz);

	//access
	double					&operator[](int idx);
	double					operator[](int idx) const;
	//set/compare
	RichVecH3				&operator=(const RichVecH3 &vec);
	bool					operator==(const RichVecH3 &vec) const;
	bool					operator!=(const RichVecH3 &vec) const;
	//add
	RichVecH3				operator+(const RichVecH3 &vec) const;
	RichVecH3				&operator+=(const RichVecH3 &vec);
	//subtract
	RichVecH3				operator-(void) const;
	RichVecH3				operator-(const RichVecH3 &vec) const;
	RichVecH3				&operator-=(const RichVecH3 &vec);
	//multiply
	RichVecH3				operator*(const RichVecH3 &vec) const;
	RichVecH3				&operator*=(const RichVecH3 &vec);
	RichVecH3				operator*(const double &f) const;
	RichVecH3				&operator*=(const double &f);
	//divide
	RichVecH3				operator/(const RichVecH3 &vec) const;
	RichVecH3				&operator/=(const RichVecH3 &vec);

	void					ChangeEndian(void);

	//general operations
	double					Dot(const RichVecH3 &vec) const;
	RichVecH3				Cross(const RichVecH3 &vec) const;
	void					Cross(const RichVecH3 &vecA, const RichVecH3 &vecB);
	double					Length(void) const;
	double					LengthSq(void) const;
	double					Normalize(void);
	RichVecH3				Normalized(void) const;
	void					BarycentricCoordinates(const RichVecH3 &v0, const RichVecH3 &v1, const RichVecH3 &v2, const RichVecH3 &point);
	void					SLerp(const RichVecH3 &vec, const double frac);
	void					SLerp(const RichVecH3 &vecA, const RichVecH3 &vecB, const double frac);
	void					OrthoBasis(RichVecH3 *pRightOut, RichVecH3 *pUpOut) const;
	void					Min(const RichVecH3 &vec);
	void					Min(const RichVecH3 &vecA, const RichVecH3 &vecB);
	void					Max(const RichVecH3 &vec);
	void					Max(const RichVecH3 &vecA, const RichVecH3 &vecB);

	double					v[3];
};


class RichVec4
{
public:
	RichVec4(void);
	RichVec4(const float x, const float y, const float z, const float w);
	RichVec4(const float *xyzw);
	RichVec4(const RichVec3 &xyz, const float w);

	//access
	float					&operator[](int idx);
	float					operator[](int idx) const;
	//set/compare
	RichVec4				&operator=(const RichVec4 &vec);
	bool					operator==(const RichVec4 &vec) const;
	bool					operator!=(const RichVec4 &vec) const;
	//add
	RichVec4				operator+(const RichVec4 &vec) const;
	RichVec4				&operator+=(const RichVec4 &vec);
	//subtract
	RichVec4				operator-(void) const;
	RichVec4				operator-(const RichVec4 &vec) const;
	RichVec4				&operator-=(const RichVec4 &vec);
	//multiply
	RichVec4				operator*(const RichVec4 &vec) const;
	RichVec4				&operator*=(const RichVec4 &vec);
	RichVec4				operator*(const float &f) const;
	RichVec4				&operator*=(const float &f);
	//divide
	RichVec4				operator/(const RichVec4 &vec) const;
	RichVec4				&operator/=(const RichVec4 &vec);

	void					ChangeEndian(void);

	//general operations
	float					Dot(const RichVec4 &vec) const;
	float					Length(void) const;
	float					LengthSq(void) const;
	float					Normalize(void);
	RichVec4				Normalized(void) const;
	void					Lerp(const RichVec4 &vec, const float frac);
	void					Lerp(const RichVec4 &vecA, const RichVec4 &vecB, const float frac);

	//conversion
	RichVec3				ToVec3(void) const;

	float					v[4];
};

class RichVec2
{
public:
	RichVec2(void);
	RichVec2(const float x, const float y);
	RichVec2(const float *xy);

	//access
	float					&operator[](int idx);
	float					operator[](int idx) const;
	//set/compare
	RichVec2				&operator=(const RichVec2 &vec);
	bool					operator==(const RichVec2 &vec) const;
	bool					operator!=(const RichVec2 &vec) const;
	//add
	RichVec2				operator+(const RichVec2 &vec) const;
	RichVec2				&operator+=(const RichVec2 &vec);
	//subtract
	RichVec2				operator-(void) const;
	RichVec2				operator-(const RichVec2 &vec) const;
	RichVec2				&operator-=(const RichVec2 &vec);
	//multiply
	RichVec2				operator*(const RichVec2 &vec) const;
	RichVec2				&operator*=(const RichVec2 &vec);
	RichVec2				operator*(const float &f) const;
	RichVec2				&operator*=(const float &f);
	//divide
	RichVec2				operator/(const RichVec2 &vec) const;
	RichVec2				&operator/=(const RichVec2 &vec);

	void					ChangeEndian(void);

	//general operations
	float					Dot(const RichVec2 &vec) const;
	float					Cross(const RichVec2 &vec, const RichVec2 &point) const;
	float					Cross(const RichVec2 &vec) const;
	float					Length(void) const;
	float					LengthSq(void) const;
	float					Normalize(void);
	RichVec2				Normalized(void) const;
	void					Lerp(const RichVec2 &vec, const float frac);
	void					Lerp(const RichVec2 &vecA, const RichVec2 &vecB, const float frac);
	RichVec2				InverseOrZero() const;

	float					v[2];
};

class RichVecH2
{
public:
	RichVecH2(void);
	RichVecH2(const double x, const double y);
	RichVecH2(const double *xy);

	//access
	double					&operator[](int idx);
	double					operator[](int idx) const;
	//set/compare
	RichVecH2				&operator=(const RichVecH2 &vec);
	bool					operator==(const RichVecH2 &vec) const;
	bool					operator!=(const RichVecH2 &vec) const;
	//add
	RichVecH2				operator+(const RichVecH2 &vec) const;
	RichVecH2				&operator+=(const RichVecH2 &vec);
	//subtract
	RichVecH2				operator-(void) const;
	RichVecH2				operator-(const RichVecH2 &vec) const;
	RichVecH2				&operator-=(const RichVecH2 &vec);
	//multiply
	RichVecH2				operator*(const RichVecH2 &vec) const;
	RichVecH2				&operator*=(const RichVecH2 &vec);
	RichVecH2				operator*(const double &f) const;
	RichVecH2				&operator*=(const double &f);
	//divide
	RichVecH2				operator/(const RichVecH2 &vec) const;
	RichVecH2				&operator/=(const RichVecH2 &vec);

	void					ChangeEndian(void);

	//general operations
	double					Dot(const RichVecH2 &vec) const;
	double					Cross(const RichVecH2 &vec, const RichVecH2 &point) const;
	double					Cross(const RichVecH2 &vec) const;
	double					Length(void) const;
	double					LengthSq(void) const;
	double					Normalize(void);
	RichVecH2				Normalized(void) const;
	RichVecH2				InverseOrZero() const;
	RichVecH2				PointOnSegment(const RichVecH2 &v0, const RichVecH2 &v1) const;

	double					v[2];
};




//=========================================
//Misc utility classes
//=========================================

class NoeMappedFile
{
public:
	virtual bool IsValid(void) = 0;

	virtual bool Read(BYTE *dst, unsigned __int64 ofs, int size) = 0;
	virtual bool Write(const BYTE *src, unsigned __int64 ofs, int size) = 0;
	virtual void *Lock(unsigned __int64 ofs, int size) = 0;
	virtual void Unlock(void) = 0;

	virtual void *GetInterface(int interType) = 0;
};

class RichMemFileBase
{
public:
	virtual bool IsValid(void) = 0;
	virtual  __int64 GetSize(void) = 0;
	virtual void Seek(__int64 pos, bool seekRelative) = 0;
	virtual __int64 Tell(void) = 0;
	virtual bool CheckEOF(void) = 0;
	virtual __int64 Read(void *dstBuf, __int64 size) = 0;
	virtual __int64 Write(const void *srcBuf, __int64 size) = 0;
};

//quick and dirty encapsulation class for plugins to use with the streaming file functions
class RichFileWrap : public RichMemFileBase
{
public:
	RichFileWrap(const wchar_t *filename, noeFSMode_e mode, noeRAPI_t *rapi);
	RichFileWrap(void *file, noeRAPI_t *rapi, bool close);
	~RichFileWrap();

	virtual bool IsValid(void);
	void *GetFile(void);
	virtual __int64 GetSize(void);
	virtual void Seek(__int64 pos, bool seekRelative);
	virtual __int64 Tell(void);
	virtual bool CheckEOF(void);
	virtual __int64 Read(void *dstBuf, __int64 size);
	virtual __int64 Write(const void *srcBuf, __int64 size);

private:
	void		*m_file;
	noeRAPI_t	*m_rapi;
	bool		m_close;
};

//for treating pre-allocated buffers like files. use RichBitStream if you want something self-allocating.
class RichMemFileWrap : public RichMemFileBase
{
public:
	RichMemFileWrap(BYTE *buf, __int64 bufSize);
	~RichMemFileWrap();

	virtual bool IsValid(void);
	BYTE *GetBuffer(void);
	virtual __int64 GetSize(void);
	virtual void Seek(__int64 pos, bool seekRelative);
	virtual __int64 Tell(void);
	virtual bool CheckEOF(void);
	virtual __int64 Read(void *dstBuf, __int64 size);
	virtual __int64 Write(const void *srcBuf, __int64 size);

private:
	BYTE		*m_buf;
	__int64		m_bufSize;
	__int64		m_bufPtr;
};






//=========================================
//Container/data classes
//=========================================
#ifdef _NOESIS_INTERNAL
#define NFN_CHECK
#else
#define NFN_CHECK if (!g_nfn) { assert(0); return; }
#endif

#ifdef _NOESIS_INTERNAL
#define ARRAY_PREFIX
cntArray_t *Array_Alloc(int elementSize, int initialNum);
void Array_Free(cntArray_t *ar);
void Array_SetGrowth(cntArray_t *ar, bool exponential);
void Array_QSort(cntArray_t *ar, int (__cdecl * compareFunc)(const void *a, const void *b));
void *Array_GetElement(cntArray_t *ar, int index);
void *Array_GetElementGrow(cntArray_t *ar, int index);
void Array_Append(cntArray_t *ar, const void *element);
void Array_RemoveLast(cntArray_t *ar);
void Array_Insert(cntArray_t *ar, const void *element, int index);
void Array_Remove(cntArray_t *ar, int index);
int Array_GetCount(cntArray_t *ar);
void Array_Reset(cntArray_t *ar);
void Array_Tighten(cntArray_t *ar);
#else
#define ARRAY_PREFIX g_nfn->
#endif

template<class classType>
class CArrayList
{
public:
	CArrayList()
	{
		m_array = NULL;
	}

	~CArrayList()
	{
		NFN_CHECK
		Clear();
	}

	void CopyTo(const CArrayList<classType> &from)
	{
		Clear();
		if (!from.m_array || from.Num() <= 0)
		{
			return;
		}
		m_array = ARRAY_PREFIX Array_Alloc(sizeof(classType), from.Num());
		ARRAY_PREFIX Array_SetGrowth(m_array, true);
		for (int i = 0; i < from.Num(); i++)
		{
			Append(from[i]);
		}
	}

	inline int Num(void) const
	{
		if (!m_array)
		{
			return 0;
		}
		return ARRAY_PREFIX Array_GetCount(m_array);
	}

	inline void Clear(void)
	{
		if (m_array)
		{
			ARRAY_PREFIX Array_Free(m_array);
			m_array = 0;
		}
	}

	inline void Reset(void)
	{ //resets without freeing the array
		if (m_array)
		{
			ARRAY_PREFIX Array_Reset(m_array);
		}
	}

	inline void SetGrowth(bool exponential)
	{
		CheckAlloc();
		ARRAY_PREFIX Array_SetGrowth(m_array, exponential);
	}

	inline void QSort(int (__cdecl * compareFunc)(const void *a, const void *b))
	{
		if (m_array)
		{
			ARRAY_PREFIX Array_QSort(m_array, compareFunc);
		}
	}

	inline int Find(const classType &p, bool add = true)
	{
		CheckAlloc();
		int csize = sizeof(classType);
		int cnum = Num();
		for (int i = 0; i < cnum; i++)
		{
			const classType *a = (const classType *)ARRAY_PREFIX Array_GetElementGrow(m_array, i);
			const classType *b = &p;
			if (!memcmp(a, b, csize))
			{
				return i;
			}
		}
		if (!add)
		{
			return -1;
		}
		int r = Num();
		Append(p);
		return r;
	}

	inline void Append(const classType &p)
	{
		Push(p);
	}

	inline void Push(const classType &p)
	{
		CheckAlloc();
		ARRAY_PREFIX Array_Append(m_array, &p);
	}

	inline classType Pop(void)
	{
		CheckAlloc();
		int count = Num();
		classType r;
		if (count > 0)
		{
			r = *(const classType *)ARRAY_PREFIX Array_GetElement(m_array, count-1);
			ARRAY_PREFIX Array_RemoveLast(m_array);
		}
		return r;
	}

	inline void RemoveIndex(int index)
	{
		CheckAlloc();
		ARRAY_PREFIX Array_Remove(m_array, index);
	}

	inline classType &operator[](int index)
	{
		CheckAlloc();
		classType *t = (classType *)ARRAY_PREFIX Array_GetElementGrow(m_array, index);
		return *t;
	}

	inline const classType &operator[](int index) const
	{
		assert(m_array && index < Num());
		const classType *t = (const classType *)ARRAY_PREFIX Array_GetElement(m_array, index);
		return *t;
	}

private:
	inline void CheckAlloc(void)
	{
		if (m_array)
		{
			return;
		}
		m_array = ARRAY_PREFIX Array_Alloc(sizeof(classType), 4096);
		ARRAY_PREFIX Array_SetGrowth(m_array, true);
	}

	mutable cntArray_t		*m_array;
};

#define BITSTREAMFL_BIGENDIAN		(1<<16) //big endian for type-sensitive reads/writes (default is little)
#define BITSTREAMFL_DESCENDINGBITS	(1<<17) //accumulates n-bit values in descending order (default is ascending)
//user flags may be implemented on a per-plugin basis
#define BITSTREAMFL_USERFLAG1		(1<<24)
#define BITSTREAMFL_USERFLAG2		(1<<25)
#define BITSTREAMFL_USERFLAG3		(1<<26)
#define BITSTREAMFL_USERFLAG4		(1<<27)
#define BITSTREAMFL_USERFLAG5		(1<<28)
#define BITSTREAMFL_USERFLAG6		(1<<29)
#define BITSTREAMFL_USERFLAG7		(1<<30)
#define BITSTREAMFL_USERFLAG8		(1<<31)

class RichBitStream
{
public:
	RichBitStream();
	RichBitStream(void *data, int dataSize);
	~RichBitStream();

	void WriteBits(const void *src, int numBits);
	void WriteBits(int val, int numBits);
	void WriteBytes(const void *src, int size);
	bool ReadBits(void *dst, int numBits);
	int ReadBits(int numBits);
	bool ReadBytes(void *dst, int size);
	void WriteBool(bool val);
	void WriteByte(unsigned char b);
	void WriteInt(int val);
	void WriteFloat(float val);
	void WriteString(const char *str);
	void WriteStringVA(const char *fmt, ...);
	void WriteWStringVA(const wchar_t *fmt, ...);
	void WriteStringNulTerm(const char *str);
	bool ReadBool(void);
	unsigned char ReadByte(void);
	int ReadInt(void);
	float ReadFloat(void);
	void ReadString(char *str, int maxSize);

	void *GetBuffer(void);
	const void *GetBuffer(void) const;
	int GetSize(void) const;
	void SetOffset(int ofs);
	int GetOffset(void) const;
	void SetFlags(int flags);
	int GetFlags(void) const;
	bool AllocWriteSpace(int size, bool tryNonFixed = true);
	void TakeOwnershipFrom(RichBitStream *other);
	void WriteToFile(FILE *f);

	void SetBitOffset(int byteOffset, int bitOffset);
	void GetBitOffset(int *pByteOffset, int *pBitOffset) const;

	int ReadRevBits(int numBits);

protected:
	mutable cntStream_t		*m_stream;
};

#pragma pack(pop)

#endif //_NOESIS_PLUGIN_CLASSES_H
