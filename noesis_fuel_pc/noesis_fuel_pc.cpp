#include "pluginshare.h"

#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstdint>

const char* g_pPluginName = "noesis_fuel_pc";
const char* g_pPluginDesc = "FUEL PC, by widberg.";

std::unordered_map<std::uint32_t, const char*> classFileExtensions = {
	{ 549480509, "TOMNI" },
	{ 705810152, "TRTC" },
	{ 838505646, "TGENWORLD" },
	{ 848525546, "TLIGHTDATA" },
	{ 849267944, "TSOUND" },
	{ 849861735, "TOTEMBITMAP" },
	{ 866453734, "TROTSHAPE" },
	{ 954499543, "TPARTICLESDATA" },
	{ 968261323, "TWORLD" },
	{ 1114947943, "TWARP" },
	{ 1135194223, "TSPLINE" },
	{ 1175485833, "TANIM" },
	{ 1387343541, "TMESH" },
	{ 1391959958, "TUSERDEFINE" },
	{ 1396791303, "TSKIN" },
	{ 1471281566, "TBITMAP" },
	{ 1536002910, "TFONT" },
	{ 1625945536, "TROTSHAPEDATA" },
	{ 1706265229, "TSURFACE" },
	{ 1910554652, "TSPLINEGRAPH" },
	{ 1943824915, "TLOD" },
	{ 2204276779, "TMATERIAL" },
	{ 2245010728, "TNODE" },
	{ 2259852416, "TBINARY" },
	{ 2398393906, "TVOLUME" },
	{ 2906362741, "TWORLDREF" },
	{ 3312018398, "TPARTICLES" },
	{ 3412401859, "TLODDATA" },
	{ 3611002348, "TSKEL" },
	{ 3626109572, "TMESHDATA" },
	{ 3747817665, "TSURFACEDATAS" },
	{ 3834418854, "TMATERIALANIM" },
	{ 3845834591, "TGWROAD" },
	{ 4096629181, "TGAMEOBJ" },
	{ 4240844041, "TCAMERA" },
};

struct ResourceHeader
{
	std::uint32_t dataSize;
	std::uint32_t classObjectSize;
	std::uint32_t decompressedSize;
	std::uint32_t compressedSize;
	std::uint32_t classCRC32;
	std::uint32_t crc32;
};

std::uint32_t calculatePaddedSize(std::uint32_t unpaddedSize)
{
	return (unpaddedSize + 0x7ff) & 0xfffff800;
}

std::uint32_t calculatePaddingSize(std::uint32_t unpaddedSize)
{
	return calculatePaddedSize(unpaddedSize) - unpaddedSize;
}

constexpr std::uint32_t WINDOW_LOG = 14;
constexpr std::uint32_t WINDOW_SIZE = (1 << WINDOW_LOG);
constexpr std::uint32_t WINDOW_MASK = (1 << WINDOW_LOG) - 1;

struct LZHeader
{
	std::uint32_t decompressedSize;
	std::uint32_t compressedSize;
};

std::vector<std::uint8_t> decompress(const std::vector<std::uint8_t>& data)
{
	static std::uint8_t windowBuffer[WINDOW_SIZE];
	std::memset(windowBuffer, 0, sizeof(windowBuffer));

	std::uint32_t d, r, i, j, flag, flagmask, flagbit, pos, lenbits = 2;
	std::uint8_t currentByte;

	const LZHeader lzHeader = *reinterpret_cast<const LZHeader*>(data.data());

	std::vector<std::uint8_t> decompressed;
	decompressed.reserve(lzHeader.decompressedSize);

	const std::uint8_t* dataPointer = data.data() + sizeof(lzHeader);

	flagbit = 0;

	for (pos = 0; decompressed.size() < lzHeader.decompressedSize;)
	{

		if (flagbit <= 1)
		{
			flagmask = *(dataPointer++) << 24;
			flagmask |= *(dataPointer++) << 16;
			flagmask |= *(dataPointer++) << 8;
			flagmask |= *(dataPointer++);
			flagbit = 32 - 1;
			lenbits = WINDOW_LOG - (flagmask & 3);
		}

		flag = (flagmask >> flagbit) & 1;
		flagbit--;

		currentByte = *(dataPointer++);

		if (flag == 0)
		{
			// literal

			windowBuffer[(pos++) & WINDOW_MASK] = currentByte;
			decompressed.push_back(currentByte);
		}
		else
		{
			// match

			d = *(dataPointer++);
			j = (currentByte << 8) + d;

			std::uint32_t length = (j >> lenbits) + 3;
			d = (j & ((1 << lenbits) - 1)) + 1;

			for (j = 0; j < length; j++)
			{
				currentByte = windowBuffer[(pos - d) & WINDOW_MASK];
				windowBuffer[(pos++) & WINDOW_MASK] = currentByte;
				decompressed.push_back(currentByte);
			}
		}
	}

	return decompressed;
}

static bool Arc_FUEL_ExtractDPC(wchar_t* filename, __int64 len, bool justChecking, noeRAPI_t* rapi)
{
	std::ifstream in(filename, std::ios::binary);

	if (justChecking)
	{
		static char versionString[256];
		in.read(versionString, sizeof(versionString));
		static std::unordered_set<std::string> validVersionStrings = {
			"v1.381.67.09 - Asobo Studio - Internal Cross Technology",
			"v1.381.66.09 - Asobo Studio - Internal Cross Technology",
			"v1.381.65.09 - Asobo Studio - Internal Cross Technology",
			"v1.381.64.09 - Asobo Studio - Internal Cross Technology",
			"v1.379.60.09 - Asobo Studio - Internal Cross Technology",
			"v1.325.50.07 - Asobo Studio - Internal Cross Technology",
			"v1.220.50.07 - Asobo Studio - Internal Cross Technology",
		};
		return validVersionStrings.count(versionString);
	}

	struct Resource
	{
		ResourceHeader resourceHeader;
		std::vector<std::uint8_t> classData;
		std::vector<std::uint8_t> data;
	};

	std::unordered_map<std::uint32_t, Resource> resources;

	struct UnitDescription
	{
		std::uint32_t unitType;
		std::uint32_t resourceCount;
		std::uint32_t paddedSize;
		std::uint32_t dataSize;
		std::uint32_t workingBufferOffset;
		std::uint32_t crc32;
	};

	struct PrimaryHeader
	{
		char versionString[256];
		std::uint32_t isNotRTC;
		std::uint32_t unitCount;
		std::uint32_t unitWorkingBufferCapacity;
		std::uint32_t resourceWorkingBufferCapacity;
		std::uint32_t paddedSize;
		std::uint32_t versionPatch;
		std::uint32_t versionMinor;
		UnitDescription unitDescriptions[64];
		std::uint32_t zero;
		std::uint32_t relationalTablePaddedSize;
		std::uint32_t relationalTableOffset;
		std::uint32_t relationalTableUnused0;
		std::uint32_t relationalTableUnused1;
		std::uint32_t relationalTableUnused2;
		std::uint32_t incrediBuilderUnused0;
		std::uint32_t incrediBuilderUnused1;
		std::uint32_t incrediBuilderUnused2;
		char incrediBuilderString[128];
		std::uint8_t padding[64];
	} primaryHeader;

	in.read(reinterpret_cast<char*>(&primaryHeader), sizeof(primaryHeader));

	ResourceHeader resourceHeader;

	for (std::uint32_t i = 0; i < primaryHeader.unitCount; ++i)
	{
		const std::uint32_t resourceCount = primaryHeader.unitDescriptions[i].resourceCount;

		for (std::uint32_t j = 0; j < resourceCount; ++j)
		{

			in.read(reinterpret_cast<char*>(&resourceHeader), sizeof(resourceHeader));
			resources[resourceHeader.crc32].resourceHeader = resourceHeader;
			if (classFileExtensions.count(resourceHeader.classCRC32))
			{
				rapi->LogOutput("Discovered %u.%s\n", resourceHeader.crc32, classFileExtensions[resourceHeader.classCRC32]);
			}
			else
			{
				rapi->LogOutput("Discovered %u.%u\n", resourceHeader.crc32, resourceHeader.classCRC32);
			}
			resources[resourceHeader.crc32].classData.resize(resourceHeader.classObjectSize);
			in.read(reinterpret_cast<char*>(resources[resourceHeader.crc32].classData.data()), resources[resourceHeader.crc32].classData.size());

			if (resourceHeader.dataSize != resourceHeader.classObjectSize)
			{
				resources[resourceHeader.crc32].data.resize(resourceHeader.dataSize - resourceHeader.classObjectSize);
				in.read(reinterpret_cast<char*>(resources[resourceHeader.crc32].data.data()), resources[resourceHeader.crc32].data.size());
			}
		}

		in.seekg(primaryHeader.unitDescriptions[i].paddedSize - primaryHeader.unitDescriptions[i].dataSize, std::ios::cur);
	}

	if (primaryHeader.relationalTableOffset)
	{
		in.seekg((primaryHeader.relationalTableOffset << 0xb) + (primaryHeader.relationalTablePaddedSize << 0xb), std::ios::beg);

		while (!in.read(reinterpret_cast<char*>(&resourceHeader), sizeof(resourceHeader)).eof())
		{
			if (classFileExtensions.count(resourceHeader.classCRC32))
			{
				rapi->LogOutput("Discovered %u.%s\n", resourceHeader.crc32, classFileExtensions[resourceHeader.classCRC32]);
			}
			else
			{
				rapi->LogOutput("Discovered %u.%u\n", resourceHeader.crc32, resourceHeader.classCRC32);
			}

			if (resources[resourceHeader.crc32].resourceHeader.dataSize == resources[resourceHeader.crc32].resourceHeader.classObjectSize)
			{
				resources[resourceHeader.crc32].data.resize(resourceHeader.dataSize);
				in.read(reinterpret_cast<char*>(resources[resourceHeader.crc32].data.data()), resources[resourceHeader.crc32].data.size());
				resources[resourceHeader.crc32].resourceHeader.dataSize += resourceHeader.dataSize;
				resources[resourceHeader.crc32].resourceHeader.compressedSize = resourceHeader.compressedSize;
				resources[resourceHeader.crc32].resourceHeader.decompressedSize = resourceHeader.decompressedSize;
				in.seekg(calculatePaddingSize(resourceHeader.dataSize + sizeof(resourceHeader)), std::ios::cur);
			}
			else
			{
				in.seekg(calculatePaddedSize(resourceHeader.dataSize + sizeof(resourceHeader)) - sizeof(resourceHeader), std::ios::cur);
			}
		}
	}

	for (const auto& e : resources)
	{
		const Resource& resource = e.second;

		char filename[MAX_NOESIS_PATH + 1];
		if (classFileExtensions.count(resource.resourceHeader.classCRC32))
		{
			sprintf_s(filename, sizeof(filename), "%u.%s", resource.resourceHeader.crc32, classFileExtensions[resource.resourceHeader.classCRC32]);
		}
		else
		{
			sprintf_s(filename, sizeof(filename), "%u.%u", resource.resourceHeader.crc32, resource.resourceHeader.classCRC32);
		}

		rapi->LogOutput("Exporting %s\n", filename);

		void* fs = rapi->Noesis_ExportArchiveFileOpen(filename);
		rapi->Noesis_FSWrite(&resource.resourceHeader, sizeof(resource.resourceHeader), fs);
		rapi->Noesis_FSWrite(resource.classData.data(), resource.classData.size(), fs);
		rapi->Noesis_FSWrite(resource.data.data(), resource.data.size(), fs);
		rapi->Noesis_FSClose(fs);
	}
	
	return true;
}

static bool RGBA_FUEL_TypeCheckTBITMAP(BYTE* fileBuffer, int bufferLen, noeRAPI_t* rapi)
{
	if (bufferLen < sizeof(ResourceHeader))
	{
		return false;
	}

	ResourceHeader* resourceHeader = reinterpret_cast<ResourceHeader*>(fileBuffer);
	return resourceHeader->classCRC32 == 1471281566 && resourceHeader->classObjectSize == 32;
}

static bool RGBA_FUEL_LoadTBITMAP(BYTE* fileBuffer, int bufferLen, CArrayList<noesisTex_t*>& noeTex, noeRAPI_t* rapi)
{
	ResourceHeader* resourceHeader = reinterpret_cast<ResourceHeader*>(fileBuffer);
	fileBuffer += sizeof(ResourceHeader);

#pragma pack(push, 1)
	struct Bitmap_Z
	{
		std::uint32_t crc32;
		std::uint16_t u0;
		std::uint32_t width;
		std::uint32_t height;
		std::uint8_t u1;
		std::uint8_t u2;
		std::uint8_t u3;
		std::uint8_t u4;
		std::uint8_t u5;
		std::uint8_t bitmapType;
		std::uint8_t u6;
		std::uint8_t u7;
		std::uint8_t u8;
		std::uint8_t u9;
		std::uint8_t u10;
		std::uint8_t u11;
		std::uint8_t dxtType;
		std::uint8_t mipMapCount;
		std::uint8_t u13;
		std::uint8_t u14;
		std::uint8_t u15;
		std::uint8_t u16;
	};
#pragma pack(pop)

	Bitmap_Z *bitmap_z = reinterpret_cast<Bitmap_Z*>(fileBuffer);
	fileBuffer += sizeof(Bitmap_Z);

	BYTE* imgDst = (BYTE*)rapi->Noesis_UnpooledAlloc(resourceHeader->decompressedSize);

	if (resourceHeader->compressedSize)
	{
		std::vector<std::uint8_t> data;
		data.resize(resourceHeader->compressedSize);
		std::memcpy(data.data(), fileBuffer, resourceHeader->compressedSize);

		data = decompress(data);

		std::memcpy(imgDst, data.data(), resourceHeader->decompressedSize);
	}
	else
	{
		std::memcpy(imgDst, fileBuffer, resourceHeader->decompressedSize);
	}

	int texType = NOESISTEX_UNKNOWN;
	if (bitmap_z->dxtType == 14)
	{
		texType = NOESISTEX_DXT1;
	}
	else if (bitmap_z->dxtType == 16)
	{
		texType = NOESISTEX_DXT5;
	}

	noesisTex_t* nt = rapi->Noesis_TextureAllocEx("__fuel_tmp.dds", bitmap_z->width, bitmap_z->height, imgDst, resourceHeader->decompressedSize, texType, 0, bitmap_z->mipMapCount);
	nt->shouldFreeData = true;
	noeTex.Append(nt);

	return true;
}

static bool Model_FUEL_TypeCheckTMESH(BYTE* fileBuffer, int bufferLen, noeRAPI_t* rapi)
{
	if (bufferLen < sizeof(ResourceHeader))
	{
		return false;
	}

	ResourceHeader* resourceHeader = reinterpret_cast<ResourceHeader*>(fileBuffer);
	return resourceHeader->classCRC32 == 1387343541;
}

noesisModel_t* Model_FUEL_LoadTMESH(BYTE* fileBuffer, int bufferLen, int& numMdl, noeRAPI_t* rapi)
{
	ResourceHeader* resourceHeader = reinterpret_cast<ResourceHeader*>(fileBuffer);
	fileBuffer += sizeof(ResourceHeader);

//#pragma pack(push, 1)
//	struct Mesh_Z
//	{
//
//	};
//#pragma pack(pop)
//
//	Mesh_Z* mesh_z = reinterpret_cast<Mesh_Z*>(fileBuffer);
//	fileBuffer += sizeof(Mesh_Z);

	fileBuffer += resourceHeader->classObjectSize;

	std::vector<std::uint8_t> data;
	if (resourceHeader->compressedSize)
	{
		data.resize(resourceHeader->compressedSize);
		std::memcpy(data.data(), fileBuffer, resourceHeader->compressedSize);

		data = decompress(data);

		fileBuffer = data.data();
	}

	fileBuffer += 24;
	const std::uint32_t materialCRC32Count = *reinterpret_cast<std::uint32_t*>(fileBuffer);
	fileBuffer += sizeof(std::uint32_t);
	fileBuffer += materialCRC32Count * sizeof(std::uint32_t);
	fileBuffer += 12;
	const std::uint32_t subMeshCount = *reinterpret_cast<std::uint32_t*>(fileBuffer);
	fileBuffer += sizeof(std::uint32_t);

	BYTE* idxPtr = fileBuffer;
	for (std::uint32_t i = 0; i < subMeshCount; ++i)
	{
		struct SubMesh
		{
			std::uint32_t vertexCount;
			std::uint32_t vertexSize;
			std::uint32_t u0;
		} *subMesh = reinterpret_cast<SubMesh*>(idxPtr);
		idxPtr += sizeof(SubMesh) + subMesh->vertexCount * subMesh->vertexSize;
	}
	idxPtr += sizeof(std::uint32_t);

	void* pgctx = rapi->rpgCreateContext();

	for (std::uint32_t i = 0; i < subMeshCount; ++i)
	{
		static char name[255];
		sprintf_s(name, sizeof(name), "Mesh_%u", i);
		rapi->rpgSetName(name);
		rapi->rpgSetMaterial(NULL);

		struct SubMesh
		{
			std::uint32_t vertexCount;
			std::uint32_t vertexSize;
			std::uint32_t u0;
		} *subMesh = reinterpret_cast<SubMesh*>(fileBuffer);
		fileBuffer += sizeof(SubMesh);
		rapi->rpgBindPositionBufferSafe(fileBuffer, RPGEODATA_FLOAT, subMesh->vertexSize, subMesh->vertexCount * subMesh->vertexSize);
		fileBuffer += subMesh->vertexCount * subMesh->vertexSize;

		struct Indices
		{
			std::uint32_t indexCount;
			std::uint32_t u1;
		} *idx = reinterpret_cast<Indices*>(idxPtr);
		idxPtr += sizeof(Indices);
		rapi->rpgCommitTrianglesSafe(idxPtr, RPGEODATA_USHORT, idx->indexCount, RPGEO_TRIANGLE, false);
		idxPtr += idx->indexCount * sizeof(std::uint16_t);
	}

	rapi->rpgSetOption(RPGOPT_TRIWINDBACKWARD, true);
	rapi->rpgOptimize();
	noesisModel_t* mdl = rapi->rpgConstructModel();
	if (mdl)
	{
		numMdl = 1;
	}

	rapi->rpgDestroyContext(pgctx);

	return mdl;
}

static bool Model_FUEL_TypeCheckTSKEL(BYTE* fileBuffer, int bufferLen, noeRAPI_t* rapi)
{
	if (bufferLen < sizeof(ResourceHeader))
	{
		return false;
	}

	ResourceHeader* resourceHeader = reinterpret_cast<ResourceHeader*>(fileBuffer);
	return resourceHeader->classCRC32 == 3611002348;
}

noesisModel_t* Model_FUEL_LoadTSKEL(BYTE* fileBuffer, int bufferLen, int& numMdl, noeRAPI_t* rapi)
{
	cntStream_t* s = g_nfn->Stream_Alloc(fileBuffer, bufferLen);

	ResourceHeader resourceHeader;
	g_nfn->Stream_ReadBytes(s, &resourceHeader, sizeof(resourceHeader));
	g_nfn->Stream_SetOffset(s, g_nfn->Stream_GetOffset(s) + resourceHeader.classObjectSize);
	

	void* pgctx = rapi->rpgCreateContext();

	g_nfn->Stream_SetOffset(s, g_nfn->Stream_GetOffset(s) + 20);
	std::uint32_t joinCount = g_nfn->Stream_ReadInt(s);
	g_nfn->Stream_SetOffset(s, g_nfn->Stream_GetOffset(s) + 24);

	modelBone_t* bones = rapi->Noesis_AllocBones(joinCount);

	for (std::uint32_t i = 0; i < joinCount; ++i)
	{
		g_nfn->Stream_SetOffset(s, g_nfn->Stream_GetOffset(s) + 140);
		fourxMatrix_t mat;
		g_nfn->Stream_ReadBytes(s, &mat, sizeof(mat));
		RichMat44 mat44(mat);
		RichMat43 mat43 = mat44.ToMat43();

		std::uint32_t unk = g_nfn->Stream_ReadInt(s);
		std::uint32_t parent = g_nfn->Stream_ReadInt(s) - 1;

		modelBone_t* bone = bones + i;
		bone->index = i;
		bone->mat = mat43.m;
		std::strcpy(bone->name, std::to_string(i).c_str());
		bone->eData.parent = (parent < joinCount && parent != i) ? bones + parent : NULL;

		g_nfn->Stream_SetOffset(s, g_nfn->Stream_GetOffset(s) + 36);
	}

	g_nfn->Stream_Free(s);

	rapi->rpgSetExData_Bones(bones, joinCount);

	noesisModel_t* mdl = rapi->rpgConstructModel();
	if (mdl)
	{
		numMdl = 1;
	}

	rapi->rpgDestroyContext(pgctx);

	return mdl;
}

//called by Noesis to init the plugin
bool NPAPI_InitLocal(void)
{
	if (g_nfn->NPAPI_GetAPIVersion() < NOESIS_PLUGINAPI_VERSION)
	{
		return false;
	}

	int fh;

	// DPC
	fh = g_nfn->NPAPI_Register("FUEL Archive", ".DPC");
	if (fh < 0)
	{
		return false;
	}

	g_nfn->NPAPI_SetTypeHandler_ExtractArcStream(fh, Arc_FUEL_ExtractDPC);

	// TBITMAP
	fh = g_nfn->NPAPI_Register("FUEL Bitmap", ".TBITMAP");
	if (fh < 0)
	{
		return false;
	}

	g_nfn->NPAPI_SetTypeHandler_TypeCheck(fh, RGBA_FUEL_TypeCheckTBITMAP);
	g_nfn->NPAPI_SetTypeHandler_LoadRGBA(fh, RGBA_FUEL_LoadTBITMAP);

	// TMESH
	fh = g_nfn->NPAPI_Register("FUEL Mesh", ".TMESH");
	if (fh < 0)
	{
		return false;
	}

	g_nfn->NPAPI_SetTypeHandler_TypeCheck(fh, Model_FUEL_TypeCheckTMESH);
	g_nfn->NPAPI_SetTypeHandler_LoadModel(fh, Model_FUEL_LoadTMESH);

	// TSKEL
	fh = g_nfn->NPAPI_Register("FUEL Skeleton", ".TSKEL");
	if (fh < 0)
	{
		return false;
	}

	g_nfn->NPAPI_SetTypeHandler_TypeCheck(fh, Model_FUEL_TypeCheckTSKEL);
	g_nfn->NPAPI_SetTypeHandler_LoadModel(fh, Model_FUEL_LoadTSKEL);

	return true;
}

//called by Noesis before the plugin is freed
void NPAPI_ShutdownLocal(void)
{
	//nothing to do here
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	return TRUE;
}
