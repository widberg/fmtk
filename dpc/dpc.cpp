#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cstdint>
#include <iostream>
#include <cstring>
#include <unordered_set>
#include <map>
#include <set>

std::unordered_set<std::uint32_t> classCRC32s;
std::unordered_set<std::uint32_t> heights;
std::unordered_set<std::uint32_t> objectCRC32s;

std::unordered_map<std::uint32_t, std::string> crc32_reverse_lookup;

std::unordered_map<std::uint32_t, const char*> classFileExtensions = {
    { 549480509, "Omni_Z" },
    { 705810152, "Rtc_Z" },
    { 838505646, "GenWorld_Z" },
    { 848525546, "LightData_Z" },
    { 849267944, "Sound_Z" },
    { 849861735, "MaterialObj_Z" },
    { 866453734, "RotShape_Z" },
    { 954499543, "ParticlesData_Z" },
    { 968261323, "World_Z" },
    { 1114947943, "Warp_Z" },
    { 1135194223, "Spline_Z" },
    { 1175485833, "Anim_Z" },
    { 1387343541, "Mesh_Z" },
    { 1391959958, "UserDefine_Z" },
    { 1396791303, "Skin_Z" },
    { 1471281566, "Bitmap_Z" },
    { 1536002910, "Font_Z" },
    { 1625945536, "RotShapeData_Z" },
    { 1706265229, "Surface_Z" },
    { 1910554652, "SplineGraph_Z" },
    { 1943824915, "Lod_Z" },
    { 2204276779, "Material_Z" },
    { 2245010728, "Node_Z" },
    { 2259852416, "Binary_Z" },
    { 2398393906, "CollisionVol_Z" },
    { 2906362741, "WorldRef_Z" },
    { 3312018398, "Particles_Z" },
    { 3412401859, "LodData_Z" },
    { 3611002348, "Skel_Z" },
    { 3626109572, "MeshData_Z" },
    { 3747817665, "SurfaceDatas_Z" },
    { 3834418854, "MaterialAnim_Z" },
    { 3845834591, "GwRoad_Z" },
    { 4096629181, "GameObj_Z" },
    { 4240844041, "Camera_Z" },
};

struct AAA
{
    std::string className;
    std::set<std::string> dpcNames;
    std::int32_t signedCRC32;
	std::string text;
};

std::map<std::uint32_t, AAA> crc32s;

#define FIELD(name) { #name, name }

enum
{
    winlog = 14,
    winsize = (1 << winlog),
    winmask = (1 << winlog) - 1
};

std::vector<std::uint8_t> unLZ(const std::vector<std::uint8_t>& data)
{
    std::uint8_t win[winsize];
    std::uint32_t c, d, r, l, i, j, flag, flagmask, flagbit, pos, lenbits = 2;


    const std::uint8_t* dp = data.data() + 4;

    std::vector<std::uint8_t> decompressed;

    for (i = 0; i < winsize; i++)
        win[i] = 0;

    l = *(std::uint32_t*)dp;
    dp += sizeof(l);

    std::uint32_t len = *(std::uint32_t*)(data.data() + 4);
    std::uint32_t decompLen = *(std::uint32_t*)(data.data());

    flagbit = 0;

    for (pos = 0;;)
    {

        if (flagbit <= 1)
        {
            flagmask = *(dp++) << 24;
            flagmask |= *(dp++) << 16;
            flagmask |= *(dp++) << 8;
            flagmask |= *(dp++);
            flagbit = 32 - 1;
            lenbits = winlog - (flagmask & 3);
        }

        flag = (flagmask >> flagbit) & 1;
        flagbit--;

        c = *(dp++);
        if (decompLen == decompressed.size())
            break;

        if (flag == 0)
        {
            // literal

            win[(pos++) & winmask] = c;
            decompressed.push_back(c);
        }
        else
        {
            // match

            d = *(dp++);
            j = (c << 8) + d;

            l = (j >> lenbits) + 3;
            d = (j & ((1 << lenbits) - 1)) + 1;

            for (j = 0; j < l; j++)
            {
                c = win[(pos - d) & winmask];
                win[(pos++) & winmask] = c;
                decompressed.push_back(c);
            }
        }
    }

    return decompressed;
}

std::uint32_t calculatePaddedSize(std::uint32_t unpaddedSize)
{
    return (unpaddedSize + 0x7ff) & 0xfffff800;
}

std::uint32_t calculatePaddingSize(std::uint32_t unpaddedSize)
{
    return calculatePaddedSize(unpaddedSize) - unpaddedSize;
}

struct RelationalTableUnknownStrcuture
{
    std::uint32_t unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;
    std::uint16_t unknown3;
    std::uint16_t unknown4;
    std::uint32_t placeholder0;
    std::uint32_t placeholder1;
    std::uint32_t placeholder2;

    nlohmann::json to_json() const
    {
        return
        {
            FIELD(unknown0),
            FIELD(unknown1),
            FIELD(unknown2),
            FIELD(unknown3),
            FIELD(unknown4),
        };
    }
};

template <class T>
struct PascalArray
{
    std::uint32_t count;
    std::vector<T> data;

    nlohmann::json to_json() const
    {
        std::vector<nlohmann::json> jsonTs;

        for (const T& t : data)
        {
            jsonTs.push_back(t);
        }

        return
        {
            FIELD(count),
            { "data", jsonTs },
        };
    }
};

struct RelationalTableHeader
{
    std::uint32_t unused0;
    std::uint32_t unused1;
    std::uint32_t unused2;

    nlohmann::json to_json() const
    {
        return
        {
            FIELD(unused0),
            FIELD(unused1),
            FIELD(unused2),
        };
    }
};

struct RelationalTable
{
    RelationalTableHeader relationalTableHeader;
    PascalArray<std::uint32_t> resourcesCRC32s;
    PascalArray<std::uint32_t> crc32s;
    PascalArray<std::uint32_t> referenceCounts;
    PascalArray<std::uint32_t> resourcePaddedSize;
    PascalArray<std::uint32_t> unknown4;
    PascalArray<RelationalTableUnknownStrcuture> unknown5;
    RelationalTableUnknownStrcuture terminal;
    std::vector<std::uint8_t> padding;

    nlohmann::json to_json() const
    {
        return
        {
            { "relationalTableHeader", relationalTableHeader.to_json() },
            { "resourcesCRC32s", resourcesCRC32s.to_json() },
            { "crc32s", crc32s.to_json() },
            { "referenceCounts", referenceCounts.to_json() },
            { "resourcePaddedSize", resourcePaddedSize.to_json() },
            { "unknown4", unknown4.to_json() },
            { "unknown5", unknown5.to_json() },
        };
    }
};

struct ResourceHeader
{
    std::uint32_t dataSize;
    std::uint32_t classObjectSize;
    std::uint32_t decompressedSize;
    std::uint32_t compressedSize;
    std::uint32_t classCRC32;
    std::uint32_t crc32;

    nlohmann::json to_json() const
    {
        return
        {
            FIELD(dataSize),
            FIELD(classObjectSize),
            FIELD(decompressedSize),
            FIELD(compressedSize),
            FIELD(classCRC32),
            FIELD(crc32),
        };
    }
};

struct Resource
{
    ResourceHeader resourceHeader;
    std::vector<std::uint8_t> data;
    std::vector<std::uint8_t> padding;

    nlohmann::json to_json() const
    {
        return
        {
            { "resourceHeader", resourceHeader.to_json() },
            //FIELD(data),
        };
    }
};

struct UnitResource
{
    ResourceHeader resourceHeader;
    std::vector<std::uint8_t> classObject;
    std::vector<std::uint8_t> data;

    nlohmann::json to_json() const
    {
        /*classCRC32s.insert(resourceHeader.classCRC32);*/

        if (resourceHeader.classCRC32 == 4096629181)
        {
            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            nlohmann::json gameObj;

            if (resourceHeader.classObjectSize == 4)
            {
                gameObj["nameCRC32"] = *pData;
            }
            else if (resourceHeader.classObjectSize > 4)
            {
                gameObj["nameCRC32"] = *pData;
                pData++;
                
                std::uint32_t crc32Count = *pData;
                gameObj["crc32Count"] = crc32Count;
                pData++;

                gameObj["crc32s"] = nlohmann::json::array();

                for (std::uint32_t i = 0; i < crc32Count; ++i)
                {
                    gameObj["crc32s"].push_back(*pData);
                    pData++;
                }
            }

            if (data.size())
            {
                pData = reinterpret_cast<const std::uint32_t*>(data.data());
                std::uint32_t childCount = *pData;
                gameObj["childCount"] = childCount;
                pData++;

                gameObj["children"] = nlohmann::json::array();

                for (std::uint32_t i = 0; i < childCount; ++i)
                {
                    nlohmann::json child;
                    std::uint32_t strLen = *pData;
                    child["strLen"] = strLen;
                    pData++;
                    child["str"] = (char*)pData;
                    pData = (std::uint32_t*)((char*)pData + strLen);

                    child["u0"] = *pData;
                    pData++;

                    std::uint32_t crc32Count = *pData;
                    child["crc32Count"] = crc32Count;
                    pData++;

                    child["crc32s"] = nlohmann::json::array();

                    for (std::uint32_t j = 0; j < crc32Count; ++j)
                    {
                        child["crc32s"].push_back(*pData);
                        pData++;
                    }

                    gameObj["children"].push_back(child);
                }
            }

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$GameObj_Z", gameObj },
            };
        }
        if (resourceHeader.classCRC32 == 2245010728)
        {
            nlohmann::json node;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                node["parentNodeCRC32"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                node["parentNodeCRC32"] = *pData;
                pData++;
                node["zero"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 12)
            {
                node["parentNodeCRC32"] = *pData;
                pData++;
                node["u0"] = *pData;
                pData++;
                node["zero"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            if (data.size())
            {
                pData = reinterpret_cast<const std::uint32_t*>(data.data());

                node["u00"] = *pData;
                pData++;
                node["u01"] = *pData;
                pData++;
                node["u02"] = *pData;
                pData++;
                node["u03"] = *pData;
                pData++;
                node["u04"] = *pData;
                pData++;
                node["u05"] = *pData;
                pData++;
                node["u06"] = *pData;
                pData++;
                node["u07"] = *pData;
                pData++;
                node["u08"] = *pData;
                pData++;
                node["u09"] = *pData;
                pData++;
            }

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Node_Z", node },
            };
        }
        if (resourceHeader.classCRC32 == 1471281566)
        {
            nlohmann::json bitmap;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 32)
            {
                bitmap["crc32"] = *pData;
                pData++;

                std::uint8_t* p8 = (std::uint8_t*)pData;

                bitmap["u00"] = *p8;
                p8++;
                bitmap["u01"] = *p8;
                p8++;

                pData = (std::uint32_t*)p8;

                std::uint32_t x = *pData;
                bitmap["width"] = x;
                pData++;
                std::uint32_t y = *pData;
                bitmap["height"] = y;
                pData++;

                p8 = (std::uint8_t*)pData;

                bitmap["u02"] = *p8;
                p8++;
                bitmap["u03"] = *p8;
                p8++;
                bitmap["u04"] = *p8;
                p8++;
                bitmap["u05"] = *p8;
                p8++;
                bitmap["u06"] = *p8;
                p8++;
                bitmap["bitmapType"] = *p8;
                p8++;
                bitmap["u08"] = *p8;
                p8++;
                bitmap["u09"] = *p8;
                p8++;
                bitmap["u10"] = *p8;
                p8++;
                bitmap["u11"] = *p8;
                p8++;
                bitmap["u12"] = *p8;
                p8++;
                bitmap["u13"] = *p8;
                p8++;
                bitmap["u14"] = *p8;
                p8++;
                bitmap["mipMapCount"] = *p8;
                p8++;
                bitmap["u16"] = *p8;
                p8++;
                bitmap["u17"] = *p8;
                p8++;
                bitmap["u18"] = *p8;
                p8++;
                bitmap["u19"] = *p8;
                p8++;
            }
            else if (resourceHeader.classObjectSize == 13)
            {
                bitmap["_crc32"] = *pData;
                pData++;

                std::uint8_t* p8 = (std::uint8_t*)pData;

                bitmap["u00"] = *p8;
                p8++;
                bitmap["u01"] = *p8;
                p8++;
                bitmap["u02"] = *p8;
                p8++;
                bitmap["u03"] = *p8;
                p8++;
                bitmap["u04"] = *p8;
                p8++;
                bitmap["u05"] = *p8;
                p8++;
                bitmap["u06"] = *p8;
                p8++;
                bitmap["u07"] = *p8;
                p8++;
                bitmap["u08"] = *p8;
                p8++;
            }
            else
            {
                assert(false);
            }

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Bitmap_Z", bitmap },
            };
        }
        /*else if (resourceHeader.classCRC32 == 1387343541 && !resourceHeader.compressedSize)
        {
            nlohmann::json mesh;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            mesh["nameCRC32"] = *pData;
            pData++;
            mesh["meshDataCRC32"] = *pData;
            pData++;
            mesh["u0"] = *pData;
            pData++;
            mesh["u1"] = *pData;
            pData++;
            mesh["u2"] = *pData;
            pData++;
            mesh["u3"] = *pData;
            pData++;
            mesh["u4"] = *pData;
            pData++;
            mesh["u5"] = *pData;
            pData++;
            mesh["u6"] = *pData;
            pData++;
            mesh["u7"] = *pData;
            pData++;
            mesh["u8"] = *pData;
            pData++;
            mesh["u9"] = *pData;
            pData++;
            mesh["u10"] = *pData;
            pData++;
            mesh["u11"] = *pData;
            pData++;
            mesh["u12"] = *pData;
            pData++;
            mesh["u13"] = *pData;
            pData++;
            mesh["u14"] = *pData;
            pData++;
            mesh["u15"] = *pData;
            pData++;
            mesh["u16"] = *pData;
            pData++;
            mesh["u17"] = *pData;
            pData++;
            mesh["u18"] = *pData;
            pData++;
            mesh["u19"] = *pData;
            pData++;
            mesh["u20"] = *pData;
            pData++;
            mesh["u21"] = *pData;
            pData++;
            mesh["u22"] = *pData;
            pData++;
            mesh["u23"] = *pData;
            pData++;
            mesh["u24"] = *pData;
            pData++;
            mesh["u25"] = *pData;
            pData++;
            mesh["u26"] = *pData;
            pData++;
            mesh["u27"] = *pData;
            pData++;
            mesh["u28"] = *pData;
            pData++;
            mesh["u29"] = *pData;
            pData++;
            mesh["u30"] = *pData;
            pData++;
            mesh["u31"] = *pData;
            pData++;
            mesh["u32"] = *pData;
            pData++;
            mesh["u33"] = *pData;
            pData++;

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Mesh_Z", mesh },
            };
        }*/
        if (resourceHeader.classCRC32 == 848525546)
        {
            nlohmann::json lightData;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                lightData["u0"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                lightData["u0"] = *pData;
                pData++;
                lightData["u1"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            assert(resourceHeader.decompressedSize == 56);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$LightData_Z", lightData },
            };
        }
        /*else if (resourceHeader.classCRC32 == 3626109572 && !resourceHeader.compressedSize)
        {
            nlohmann::json meshData;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                meshData["u0"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                meshData["u0"] = *pData;
                pData++;
                meshData["u1"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            assert(resourceHeader.decompressedSize == 20);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$MeshData_Z", meshData },
            };
        }
        else if (resourceHeader.classCRC32 == 954499543 && !resourceHeader.compressedSize)
        {
            nlohmann::json particleData;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                particleData["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            assert(resourceHeader.decompressedSize == 38 || resourceHeader.decompressedSize == 40 || resourceHeader.decompressedSize == 42 || resourceHeader.decompressedSize == 44);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$ParticlesData_Z", particleData },
            };
        }*/
        if (resourceHeader.classCRC32 == 3747817665)
        {
            nlohmann::json surfaceDatas;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                surfaceDatas["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            if (data.size())
            {
                pData = reinterpret_cast<const std::uint32_t*>(data.data());

                surfaceDatas["u1"] = *pData;
            }

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$SurfaceDatas_Z", surfaceDatas },
            };
        }
        /*else if (resourceHeader.classCRC32 == 3412401859 && !resourceHeader.compressedSize)
        {
            nlohmann::json lodData;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                lodData["u0"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                lodData["u0"] = *pData;
                pData++;
                lodData["u1"] = *pData;
                pData++;
            }
            else if (resourceHeader.classObjectSize == 12)
            {
                lodData["u0"] = *pData;
                pData++;
                lodData["u1"] = *pData;
                pData++;
                lodData["u2"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$LodData_Z", lodData },
            };
        }*/
        if (resourceHeader.classCRC32 == 1391959958)
        {
            nlohmann::json userDefine;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                userDefine["nameCRC32"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                userDefine["nameCRC32"] = *pData;
                pData++;
                userDefine["zero"] = *pData;
            }
            else
            {
                assert(false);
            }

            if (data.size())
            {
                pData = reinterpret_cast<const std::uint32_t*>(data.data());
            
                std::uint32_t strLen = *pData;
                pData++;
                userDefine["strLen"] = strLen;
                std::string str;
                str.resize(strLen);
                memcpy_s(str.data(), str.size(), (char*)pData, strLen);
                userDefine["str"] = str;
            }

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$UserDefine_Z", userDefine },
            };
        }
        /*else if (resourceHeader.classCRC32 == 1175485833 && !resourceHeader.compressedSize)
        {
            nlohmann::json animation;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                animation["u0"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                animation["u0"] = *pData;
                pData++;
                animation["u1"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Animation_Z", animation },
            };
        }*/
        if (resourceHeader.classCRC32 == 705810152)
        {
            nlohmann::json rtc;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                rtc["u0"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                rtc["u0"] = *pData;
                pData++;
                rtc["u1"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data());

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Rtc_Z", rtc },
            };
        }
        /*else if (resourceHeader.classCRC32 == 968261323 && !resourceHeader.compressedSize)
        {
            nlohmann::json world;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                world["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$World_Z", world },
            };
        }
        else if (resourceHeader.classCRC32 == 1536002910 && !resourceHeader.compressedSize)
        {
            nlohmann::json fonts;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                fonts["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Fonts_Z", fonts },
            };
        }*/
        if (resourceHeader.classCRC32 == 1625945536)
        {
            nlohmann::json rotShapeData;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                rotShapeData["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data());

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$RotShapeData_Z", rotShapeData },
            };
        }
        if (resourceHeader.classCRC32 == 1114947943)
        {
            nlohmann::json warp;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                warp["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data());

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Warp_Z", warp },
            };
        }
        /*else if (resourceHeader.classCRC32 == 849861735 && !resourceHeader.compressedSize)
        {
            nlohmann::json materialObj;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                materialObj["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$MaterialObj_Z", materialObj },
            };
        }*/
        if (resourceHeader.classCRC32 == 849267944)
        {
            nlohmann::json sound;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 8)
            {
                sound["crc32"] = *pData;
                pData++;
                sound["zero"] = *pData;
                pData++;
            }
            else if (resourceHeader.classObjectSize == 14)
            {
                sound["crc32"] = *pData;
                pData++;
                sound["sampleRate"] = *pData;
                pData++;
                sound["dataSize"] = *pData;
                pData++;
                std::uint16_t* p16 = (std::uint16_t*)pData;
                sound["u0"] = *p16;
            }
            else
            {
                assert(false);
            }

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Sound_Z", sound },
            };
        }
        /*else if (resourceHeader.classCRC32 == 549480509 && !resourceHeader.compressedSize)
        {
            nlohmann::json omni;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                omni["u0"] = *pData;
                pData++;
                omni["u1"] = *pData;
                pData++;
                omni["u2"] = *pData;
                pData++;
                omni["u3"] = *pData;
                pData++;
                omni["u4"] = *pData;
                pData++;
                omni["u5"] = *pData;
                pData++;
                omni["u6"] = *pData;
                pData++;
                omni["u7"] = *pData;
                pData++;
                omni["u8"] = *pData;
                pData++;
                omni["u9"] = *pData;
                pData++;
                omni["u10"] = *pData;
                pData++;
                omni["u11"] = *pData;
                pData++;
                omni["u12"] = *pData;
                pData++;
                omni["u13"] = *pData;
                pData++;
                omni["u14"] = *pData;
                pData++;
                omni["u15"] = *pData;
                pData++;
                omni["u16"] = *pData;
                pData++;
                omni["u17"] = *pData;
                pData++;
                omni["u18"] = *pData;
                pData++;
                omni["u19"] = *pData;
                pData++;
                omni["u20"] = *pData;
                pData++;
                omni["u21"] = *pData;
                pData++;
                omni["u22"] = *pData;
                pData++;
                omni["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Omni_Z", omni },
            };
        }
        else if (resourceHeader.classCRC32 == 838505646 && !resourceHeader.compressedSize)
        {
            nlohmann::json genWorld;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                genWorld["u0"] = *pData;
                pData++;
                genWorld["u1"] = *pData;
                pData++;
                genWorld["u2"] = *pData;
                pData++;
                genWorld["u3"] = *pData;
                pData++;
                genWorld["u4"] = *pData;
                pData++;
                genWorld["u5"] = *pData;
                pData++;
                genWorld["u6"] = *pData;
                pData++;
                genWorld["u7"] = *pData;
                pData++;
                genWorld["u8"] = *pData;
                pData++;
                genWorld["u9"] = *pData;
                pData++;
                genWorld["u10"] = *pData;
                pData++;
                genWorld["u11"] = *pData;
                pData++;
                genWorld["u12"] = *pData;
                pData++;
                genWorld["u13"] = *pData;
                pData++;
                genWorld["u14"] = *pData;
                pData++;
                genWorld["u15"] = *pData;
                pData++;
                genWorld["u16"] = *pData;
                pData++;
                genWorld["u17"] = *pData;
                pData++;
                genWorld["u18"] = *pData;
                pData++;
                genWorld["u19"] = *pData;
                pData++;
                genWorld["u20"] = *pData;
                pData++;
                genWorld["u21"] = *pData;
                pData++;
                genWorld["u22"] = *pData;
                pData++;
                genWorld["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$GenWorld_Z", genWorld },
            };
        }
        else if (resourceHeader.classCRC32 == 866453734 && !resourceHeader.compressedSize)
        {
            nlohmann::json rotShape;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                rotShape["u0"] = *pData;
                pData++;
                rotShape["u1"] = *pData;
                pData++;
                rotShape["u2"] = *pData;
                pData++;
                rotShape["u3"] = *pData;
                pData++;
                rotShape["u4"] = *pData;
                pData++;
                rotShape["u5"] = *pData;
                pData++;
                rotShape["u6"] = *pData;
                pData++;
                rotShape["u7"] = *pData;
                pData++;
                rotShape["u8"] = *pData;
                pData++;
                rotShape["u9"] = *pData;
                pData++;
                rotShape["u10"] = *pData;
                pData++;
                rotShape["u11"] = *pData;
                pData++;
                rotShape["u12"] = *pData;
                pData++;
                rotShape["u13"] = *pData;
                pData++;
                rotShape["u14"] = *pData;
                pData++;
                rotShape["u15"] = *pData;
                pData++;
                rotShape["u16"] = *pData;
                pData++;
                rotShape["u17"] = *pData;
                pData++;
                rotShape["u18"] = *pData;
                pData++;
                rotShape["u19"] = *pData;
                pData++;
                rotShape["u20"] = *pData;
                pData++;
                rotShape["u21"] = *pData;
                pData++;
                rotShape["u22"] = *pData;
                pData++;
                rotShape["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$RotShape_Z", rotShape },
            };
        }
        else if (resourceHeader.classCRC32 == 1396791303 && !resourceHeader.compressedSize)
        {
            nlohmann::json skin;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                skin["u0"] = *pData;
                pData++;
                skin["u1"] = *pData;
                pData++;
                skin["u2"] = *pData;
                pData++;
                skin["u3"] = *pData;
                pData++;
                skin["u4"] = *pData;
                pData++;
                skin["u5"] = *pData;
                pData++;
                skin["u6"] = *pData;
                pData++;
                skin["u7"] = *pData;
                pData++;
                skin["u8"] = *pData;
                pData++;
                skin["u9"] = *pData;
                pData++;
                skin["u10"] = *pData;
                pData++;
                skin["u11"] = *pData;
                pData++;
                skin["u12"] = *pData;
                pData++;
                skin["u13"] = *pData;
                pData++;
                skin["u14"] = *pData;
                pData++;
                skin["u15"] = *pData;
                pData++;
                skin["u16"] = *pData;
                pData++;
                skin["u17"] = *pData;
                pData++;
                skin["u18"] = *pData;
                pData++;
                skin["u19"] = *pData;
                pData++;
                skin["u20"] = *pData;
                pData++;
                skin["u21"] = *pData;
                pData++;
                skin["u22"] = *pData;
                pData++;
                skin["u23"] = *pData;
                pData++;
            }
            else if (resourceHeader.classObjectSize == 106)
            {
                skin["u0"] = *pData;
                pData++;
                skin["u1"] = *pData;
                pData++;
                skin["u2"] = *pData;
                pData++;
                skin["u3"] = *pData;
                pData++;
                skin["u4"] = *pData;
                pData++;
                skin["u5"] = *pData;
                pData++;
                skin["u6"] = *pData;
                pData++;
                skin["u7"] = *pData;
                pData++;
                skin["u8"] = *pData;
                pData++;
                skin["u9"] = *pData;
                pData++;
                skin["u10"] = *pData;
                pData++;
                skin["u11"] = *pData;
                pData++;
                skin["u12"] = *pData;
                pData++;
                skin["u13"] = *pData;
                pData++;
                skin["u14"] = *pData;
                pData++;
                skin["u15"] = *pData;
                pData++;
                skin["u16"] = *pData;
                pData++;
                skin["u17"] = *pData;
                pData++;
                skin["u18"] = *pData;
                pData++;
                skin["u19"] = *pData;
                pData++;
                skin["u20"] = *pData;
                pData++;
                skin["u21"] = *pData;
                pData++;
                skin["u22"] = *pData;
                pData++;
                skin["u23"] = *pData;
                pData++;
                skin["u24"] = *pData;
                pData++;
                skin["u25"] = *pData;
                pData++;
            }
            else if (resourceHeader.classObjectSize == 102)
            {
                skin["u0"] = *pData;
                pData++;
                skin["u1"] = *pData;
                pData++;
                skin["u2"] = *pData;
                pData++;
                skin["u3"] = *pData;
                pData++;
                skin["u4"] = *pData;
                pData++;
                skin["u5"] = *pData;
                pData++;
                skin["u6"] = *pData;
                pData++;
                skin["u7"] = *pData;
                pData++;
                skin["u8"] = *pData;
                pData++;
                skin["u9"] = *pData;
                pData++;
                skin["u10"] = *pData;
                pData++;
                skin["u11"] = *pData;
                pData++;
                skin["u12"] = *pData;
                pData++;
                skin["u13"] = *pData;
                pData++;
                skin["u14"] = *pData;
                pData++;
                skin["u15"] = *pData;
                pData++;
                skin["u16"] = *pData;
                pData++;
                skin["u17"] = *pData;
                pData++;
                skin["u18"] = *pData;
                pData++;
                skin["u19"] = *pData;
                pData++;
                skin["u20"] = *pData;
                pData++;
                skin["u21"] = *pData;
                pData++;
                skin["u22"] = *pData;
                pData++;
                skin["u23"] = *pData;
                pData++;
                skin["u24"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Skin_Z", skin },
            };
        }
        else if (resourceHeader.classCRC32 == 1706265229 && !resourceHeader.compressedSize)
        {
            nlohmann::json surface;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                surface["u0"] = *pData;
                pData++;
                surface["u1"] = *pData;
                pData++;
                surface["u2"] = *pData;
                pData++;
                surface["u3"] = *pData;
                pData++;
                surface["u4"] = *pData;
                pData++;
                surface["u5"] = *pData;
                pData++;
                surface["u6"] = *pData;
                pData++;
                surface["u7"] = *pData;
                pData++;
                surface["u8"] = *pData;
                pData++;
                surface["u9"] = *pData;
                pData++;
                surface["u10"] = *pData;
                pData++;
                surface["u11"] = *pData;
                pData++;
                surface["u12"] = *pData;
                pData++;
                surface["u13"] = *pData;
                pData++;
                surface["u14"] = *pData;
                pData++;
                surface["u15"] = *pData;
                pData++;
                surface["u16"] = *pData;
                pData++;
                surface["u17"] = *pData;
                pData++;
                surface["u18"] = *pData;
                pData++;
                surface["u19"] = *pData;
                pData++;
                surface["u20"] = *pData;
                pData++;
                surface["u21"] = *pData;
                pData++;
                surface["u22"] = *pData;
                pData++;
                surface["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Surface_Z", surface },
            };
        }
        else if (resourceHeader.classCRC32 == 3312018398 && !resourceHeader.compressedSize)
        {
            nlohmann::json particles;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                particles["u0"] = *pData;
                pData++;
                particles["u1"] = *pData;
                pData++;
                particles["u2"] = *pData;
                pData++;
                particles["u3"] = *pData;
                pData++;
                particles["u4"] = *pData;
                pData++;
                particles["u5"] = *pData;
                pData++;
                particles["u6"] = *pData;
                pData++;
                particles["u7"] = *pData;
                pData++;
                particles["u8"] = *pData;
                pData++;
                particles["u9"] = *pData;
                pData++;
                particles["u10"] = *pData;
                pData++;
                particles["u11"] = *pData;
                pData++;
                particles["u12"] = *pData;
                pData++;
                particles["u13"] = *pData;
                pData++;
                particles["u14"] = *pData;
                pData++;
                particles["u15"] = *pData;
                pData++;
                particles["u16"] = *pData;
                pData++;
                particles["u17"] = *pData;
                pData++;
                particles["u18"] = *pData;
                pData++;
                particles["u19"] = *pData;
                pData++;
                particles["u20"] = *pData;
                pData++;
                particles["u21"] = *pData;
                pData++;
                particles["u22"] = *pData;
                pData++;
                particles["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Particles_Z", particles },
            };
        }
        else if (resourceHeader.classCRC32 == 2906362741 && !resourceHeader.compressedSize)
        {
            nlohmann::json worldRef;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                worldRef["u0"] = *pData;
                pData++;
                worldRef["u1"] = *pData;
                pData++;
                worldRef["u2"] = *pData;
                pData++;
                worldRef["u3"] = *pData;
                pData++;
                worldRef["u4"] = *pData;
                pData++;
                worldRef["u5"] = *pData;
                pData++;
                worldRef["u6"] = *pData;
                pData++;
                worldRef["u7"] = *pData;
                pData++;
                worldRef["u8"] = *pData;
                pData++;
                worldRef["u9"] = *pData;
                pData++;
                worldRef["u10"] = *pData;
                pData++;
                worldRef["u11"] = *pData;
                pData++;
                worldRef["u12"] = *pData;
                pData++;
                worldRef["u13"] = *pData;
                pData++;
                worldRef["u14"] = *pData;
                pData++;
                worldRef["u15"] = *pData;
                pData++;
                worldRef["u16"] = *pData;
                pData++;
                worldRef["u17"] = *pData;
                pData++;
                worldRef["u18"] = *pData;
                pData++;
                worldRef["u19"] = *pData;
                pData++;
                worldRef["u20"] = *pData;
                pData++;
                worldRef["u21"] = *pData;
                pData++;
                worldRef["u22"] = *pData;
                pData++;
                worldRef["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$WorldRef_Z", worldRef },
            };
        }
        else if (resourceHeader.classCRC32 == 1910554652 && !resourceHeader.compressedSize)
        {
            nlohmann::json splineGraph;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                splineGraph["u0"] = *pData;
                pData++;
                splineGraph["u1"] = *pData;
                pData++;
                splineGraph["u2"] = *pData;
                pData++;
                splineGraph["u3"] = *pData;
                pData++;
                splineGraph["u4"] = *pData;
                pData++;
                splineGraph["u5"] = *pData;
                pData++;
                splineGraph["u6"] = *pData;
                pData++;
                splineGraph["u7"] = *pData;
                pData++;
                splineGraph["u8"] = *pData;
                pData++;
                splineGraph["u9"] = *pData;
                pData++;
                splineGraph["u10"] = *pData;
                pData++;
                splineGraph["u11"] = *pData;
                pData++;
                splineGraph["u12"] = *pData;
                pData++;
                splineGraph["u13"] = *pData;
                pData++;
                splineGraph["u14"] = *pData;
                pData++;
                splineGraph["u15"] = *pData;
                pData++;
                splineGraph["u16"] = *pData;
                pData++;
                splineGraph["u17"] = *pData;
                pData++;
                splineGraph["u18"] = *pData;
                pData++;
                splineGraph["u19"] = *pData;
                pData++;
                splineGraph["u20"] = *pData;
                pData++;
                splineGraph["u21"] = *pData;
                pData++;
                splineGraph["u22"] = *pData;
                pData++;
                splineGraph["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$SplineGraph_Z", splineGraph },
            };
        }
        else if (resourceHeader.classCRC32 == 2398393906 && !resourceHeader.compressedSize)
        {
            nlohmann::json collisionVol;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                collisionVol["u0"] = *pData;
                pData++;
                collisionVol["u1"] = *pData;
                pData++;
                collisionVol["u2"] = *pData;
                pData++;
                collisionVol["u3"] = *pData;
                pData++;
                collisionVol["u4"] = *pData;
                pData++;
                collisionVol["u5"] = *pData;
                pData++;
                collisionVol["u6"] = *pData;
                pData++;
                collisionVol["u7"] = *pData;
                pData++;
                collisionVol["u8"] = *pData;
                pData++;
                collisionVol["u9"] = *pData;
                pData++;
                collisionVol["u10"] = *pData;
                pData++;
                collisionVol["u11"] = *pData;
                pData++;
                collisionVol["u12"] = *pData;
                pData++;
                collisionVol["u13"] = *pData;
                pData++;
                collisionVol["u14"] = *pData;
                pData++;
                collisionVol["u15"] = *pData;
                pData++;
                collisionVol["u16"] = *pData;
                pData++;
                collisionVol["u17"] = *pData;
                pData++;
                collisionVol["u18"] = *pData;
                pData++;
                collisionVol["u19"] = *pData;
                pData++;
                collisionVol["u20"] = *pData;
                pData++;
                collisionVol["u21"] = *pData;
                pData++;
                collisionVol["u22"] = *pData;
                pData++;
                collisionVol["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$CollisionVol_Z", collisionVol },
            };
        }
        else if (resourceHeader.classCRC32 == 1943824915 && !resourceHeader.compressedSize)
        {
            nlohmann::json lod;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                lod["u0"] = *pData;
                pData++;
                lod["u1"] = *pData;
                pData++;
                lod["u2"] = *pData;
                pData++;
                lod["u3"] = *pData;
                pData++;
                lod["u4"] = *pData;
                pData++;
                lod["u5"] = *pData;
                pData++;
                lod["u6"] = *pData;
                pData++;
                lod["u7"] = *pData;
                pData++;
                lod["u8"] = *pData;
                pData++;
                lod["u9"] = *pData;
                pData++;
                lod["u10"] = *pData;
                pData++;
                lod["u11"] = *pData;
                pData++;
                lod["u12"] = *pData;
                pData++;
                lod["u13"] = *pData;
                pData++;
                lod["u14"] = *pData;
                pData++;
                lod["u15"] = *pData;
                pData++;
                lod["u16"] = *pData;
                pData++;
                lod["u17"] = *pData;
                pData++;
                lod["u18"] = *pData;
                pData++;
                lod["u19"] = *pData;
                pData++;
                lod["u20"] = *pData;
                pData++;
                lod["u21"] = *pData;
                pData++;
                lod["u22"] = *pData;
                pData++;
                lod["u23"] = *pData;
                pData++;
            }
            else if (resourceHeader.classObjectSize == 102)
            {
                lod["u0"] = *pData;
                pData++;
                lod["u1"] = *pData;
                pData++;
                lod["u2"] = *pData;
                pData++;
                lod["u3"] = *pData;
                pData++;
                lod["u4"] = *pData;
                pData++;
                lod["u5"] = *pData;
                pData++;
                lod["u6"] = *pData;
                pData++;
                lod["u7"] = *pData;
                pData++;
                lod["u8"] = *pData;
                pData++;
                lod["u9"] = *pData;
                pData++;
                lod["u10"] = *pData;
                pData++;
                lod["u11"] = *pData;
                pData++;
                lod["u12"] = *pData;
                pData++;
                lod["u13"] = *pData;
                pData++;
                lod["u14"] = *pData;
                pData++;
                lod["u15"] = *pData;
                pData++;
                lod["u16"] = *pData;
                pData++;
                lod["u17"] = *pData;
                pData++;
                lod["u18"] = *pData;
                pData++;
                lod["u19"] = *pData;
                pData++;
                lod["u20"] = *pData;
                pData++;
                lod["u21"] = *pData;
                pData++;
                lod["u22"] = *pData;
                pData++;
                lod["u23"] = *pData;
                pData++;
                lod["u24"] = *pData;
                pData++;
            }
            else if (resourceHeader.classObjectSize == 110)
            {
                lod["u0"] = *pData;
                pData++;
                lod["u1"] = *pData;
                pData++;
                lod["u2"] = *pData;
                pData++;
                lod["u3"] = *pData;
                pData++;
                lod["u4"] = *pData;
                pData++;
                lod["u5"] = *pData;
                pData++;
                lod["u6"] = *pData;
                pData++;
                lod["u7"] = *pData;
                pData++;
                lod["u8"] = *pData;
                pData++;
                lod["u9"] = *pData;
                pData++;
                lod["u10"] = *pData;
                pData++;
                lod["u11"] = *pData;
                pData++;
                lod["u12"] = *pData;
                pData++;
                lod["u13"] = *pData;
                pData++;
                lod["u14"] = *pData;
                pData++;
                lod["u15"] = *pData;
                pData++;
                lod["u16"] = *pData;
                pData++;
                lod["u17"] = *pData;
                pData++;
                lod["u18"] = *pData;
                pData++;
                lod["u19"] = *pData;
                pData++;
                lod["u20"] = *pData;
                pData++;
                lod["u21"] = *pData;
                pData++;
                lod["u22"] = *pData;
                pData++;
                lod["u23"] = *pData;
                pData++;
                lod["u24"] = *pData;
                pData++;
                lod["u25"] = *pData;
                pData++;
                lod["u26"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Lod_Z", lod },
            };
        }
        else if (resourceHeader.classCRC32 == 4240844041 && !resourceHeader.compressedSize)
        {
            nlohmann::json camera;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                camera["u0"] = *pData;
                pData++;
                camera["u1"] = *pData;
                pData++;
                camera["u2"] = *pData;
                pData++;
                camera["u3"] = *pData;
                pData++;
                camera["u4"] = *pData;
                pData++;
                camera["u5"] = *pData;
                pData++;
                camera["u6"] = *pData;
                pData++;
                camera["u7"] = *pData;
                pData++;
                camera["u8"] = *pData;
                pData++;
                camera["u9"] = *pData;
                pData++;
                camera["u10"] = *pData;
                pData++;
                camera["u11"] = *pData;
                pData++;
                camera["u12"] = *pData;
                pData++;
                camera["u13"] = *pData;
                pData++;
                camera["u14"] = *pData;
                pData++;
                camera["u15"] = *pData;
                pData++;
                camera["u16"] = *pData;
                pData++;
                camera["u17"] = *pData;
                pData++;
                camera["u18"] = *pData;
                pData++;
                camera["u19"] = *pData;
                pData++;
                camera["u20"] = *pData;
                pData++;
                camera["u21"] = *pData;
                pData++;
                camera["u22"] = *pData;
                pData++;
                camera["u23"] = *pData;
                pData++;
            }
            else if (resourceHeader.classObjectSize == 102)
            {
                camera["u0"] = *pData;
                pData++;
                camera["u1"] = *pData;
                pData++;
                camera["u2"] = *pData;
                pData++;
                camera["u3"] = *pData;
                pData++;
                camera["u4"] = *pData;
                pData++;
                camera["u5"] = *pData;
                pData++;
                camera["u6"] = *pData;
                pData++;
                camera["u7"] = *pData;
                pData++;
                camera["u8"] = *pData;
                pData++;
                camera["u9"] = *pData;
                pData++;
                camera["u10"] = *pData;
                pData++;
                camera["u11"] = *pData;
                pData++;
                camera["u12"] = *pData;
                pData++;
                camera["u13"] = *pData;
                pData++;
                camera["u14"] = *pData;
                pData++;
                camera["u15"] = *pData;
                pData++;
                camera["u16"] = *pData;
                pData++;
                camera["u17"] = *pData;
                pData++;
                camera["u18"] = *pData;
                pData++;
                camera["u19"] = *pData;
                pData++;
                camera["u20"] = *pData;
                pData++;
                camera["u21"] = *pData;
                pData++;
                camera["u22"] = *pData;
                pData++;
                camera["u23"] = *pData;
                pData++;
                camera["u24"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Camera_Z", camera },
            };
        }
        else if (resourceHeader.classCRC32 == 2259852416 && !resourceHeader.compressedSize)
        {
            nlohmann::json binary;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                binary["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Binary_Z", binary },
            };
        }*/
        if (resourceHeader.classCRC32 == 3845834591)
        {
            nlohmann::json gwRoad;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(classObject.data());

            if (resourceHeader.classObjectSize == 4)
            {
                gwRoad["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data());

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$GwRoad_Z", gwRoad },
            };
        }
        /*else if (resourceHeader.classCRC32 == 3834418854 && !resourceHeader.compressedSize)
        {
            nlohmann::json materialAnim;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                materialAnim["u0"] = *pData;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$MaterialAnim_Z", materialAnim },
            };
        }
        else if (resourceHeader.classCRC32 == 2204276779 && !resourceHeader.compressedSize)
        {
            nlohmann::json material;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                material["u0"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                material["u0"] = *pData;
                pData++;
                material["u1"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Material_Z", material },
            };
        }
        else if (resourceHeader.classCRC32 == 3611002348 && !resourceHeader.compressedSize)
        {
            nlohmann::json skel;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 4)
            {
                skel["u0"] = *pData;
            }
            else if (resourceHeader.classObjectSize == 8)
            {
                skel["u0"] = *pData;
                pData++;
                skel["u1"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Skel_Z", skel },
            };
        }
        else if (resourceHeader.classCRC32 == 1135194223 && !resourceHeader.compressedSize)
        {
            nlohmann::json spline;

            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

            if (resourceHeader.classObjectSize == 98)
            {
                spline["u0"] = *pData;
                pData++;
                spline["u1"] = *pData;
                pData++;
                spline["u2"] = *pData;
                pData++;
                spline["u3"] = *pData;
                pData++;
                spline["u4"] = *pData;
                pData++;
                spline["u5"] = *pData;
                pData++;
                spline["u6"] = *pData;
                pData++;
                spline["u7"] = *pData;
                pData++;
                spline["u8"] = *pData;
                pData++;
                spline["u9"] = *pData;
                pData++;
                spline["u10"] = *pData;
                pData++;
                spline["u11"] = *pData;
                pData++;
                spline["u12"] = *pData;
                pData++;
                spline["u13"] = *pData;
                pData++;
                spline["u14"] = *pData;
                pData++;
                spline["u15"] = *pData;
                pData++;
                spline["u16"] = *pData;
                pData++;
                spline["u17"] = *pData;
                pData++;
                spline["u18"] = *pData;
                pData++;
                spline["u19"] = *pData;
                pData++;
                spline["u20"] = *pData;
                pData++;
                spline["u21"] = *pData;
                pData++;
                spline["u22"] = *pData;
                pData++;
                spline["u23"] = *pData;
                pData++;
            }
            else
            {
                assert(false);
            }

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$Spline_Z", spline },
            };
        }*/

        return
        {
            { "resourceHeader", resourceHeader.to_json() },
            //FIELD(data),
        };
    }
};

struct Unit
{
    std::vector<UnitResource> unitResources;
    std::vector<std::uint8_t> padding;

    nlohmann::json to_json() const
    {
        std::vector<nlohmann::json> jsonUnitResources;
        std::unordered_set<std::string> classes;

        static std::unordered_map<std::uint32_t, const char*> classFileExtensions = {
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

        std::uint32_t dataSize = 0;
        std::uint32_t compressedSize = 0;
        std::uint32_t decompressedSize = 0;
        std::uint32_t dataSizeSum = 0;
        std::uint32_t compressedSizeSum = 0;
        std::uint32_t decompressedSizeSum = 0;

        for (const UnitResource& unitResource : unitResources)
        {
            jsonUnitResources.push_back(unitResource.to_json());
            classes.insert(classFileExtensions[unitResource.resourceHeader.classCRC32]);
            dataSize = std::max(dataSize, unitResource.resourceHeader.dataSize);
            compressedSize = std::max(compressedSize, unitResource.resourceHeader.compressedSize);
            decompressedSize = std::max(decompressedSize, unitResource.resourceHeader.decompressedSize);
            dataSizeSum += unitResource.resourceHeader.dataSize;
            compressedSizeSum += unitResource.resourceHeader.compressedSize;
            decompressedSizeSum += unitResource.resourceHeader.decompressedSize;
        }

        return
        {
            { "classes", classes },
            { "dataSize", dataSize },
            { "compressedSize", compressedSize },
            { "decompressedSize", decompressedSize },
            { "dataSizeSum", dataSizeSum },
            { "compressedSizeSum", compressedSizeSum },
            { "decompressedSizeSum", decompressedSizeSum },
            { "unitResources", jsonUnitResources },
        };
    }
};

struct UnitDescription
{
    std::uint32_t unitType;
    std::uint32_t resourceCount;
    std::uint32_t paddedSize;
    std::uint32_t dataSize;
    std::uint32_t workingBufferOffset;
    std::uint32_t crc32;

    nlohmann::json to_json() const
    {
        return
        {
            FIELD(unitType),
            FIELD(resourceCount),
            FIELD(paddedSize),
            FIELD(dataSize),
            FIELD(workingBufferOffset),
            FIELD(crc32),
        };
    }
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

    nlohmann::json to_json() const
    {
        std::vector<nlohmann::json> jsonUnitDescriptions;

        for (std::uint32_t i = 0; i < unitCount; ++i)
        {
            jsonUnitDescriptions.push_back(unitDescriptions[i].to_json());
        }

        return
        {
            FIELD(versionString),
            FIELD(isNotRTC),
            FIELD(unitCount),
            FIELD(unitWorkingBufferCapacity),
            FIELD(resourceWorkingBufferCapacity),
            FIELD(paddedSize),
            FIELD(versionPatch),
            FIELD(versionMinor),
            { "unitDescriptions", jsonUnitDescriptions },
            FIELD(relationalTablePaddedSize),
            FIELD(relationalTableOffset),
            FIELD(relationalTableUnused0),
            FIELD(relationalTableUnused1),
            FIELD(relationalTableUnused2),
            FIELD(incrediBuilderUnused0),
            FIELD(incrediBuilderUnused1),
            FIELD(incrediBuilderUnused2),
            { "incrediBuilderString", (incrediBuilderUnused0 != 0xFFFFFFFF) ? incrediBuilderString : "" },
        };
    }
};

std::ofstream allLLPC;

struct DPCFile
{
    DPCFile(const std::filesystem::path& dpcFilePath)
        : path(dpcFilePath)
    {
        std::filesystem::create_directory("unpack\\" + path.filename().stem().string());
        std::ofstream llpc(path.filename().stem().string() + ".LLPC");

        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.good())
        {
            assert(false);
        }

        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Primary Header
        file.read(reinterpret_cast<char*>(&primaryHeader), sizeof(primaryHeader));

        // Units
        for (std::uint32_t i = 0; i < primaryHeader.unitCount; ++i)
        {
            const UnitDescription& unitDescription = primaryHeader.unitDescriptions[i];
            Unit unit;
            for (std::uint32_t j = 0; j < unitDescription.resourceCount; ++j)
            {
                UnitResource unitResource;
                file.read(reinterpret_cast<char*>(&unitResource.resourceHeader), sizeof(unitResource.resourceHeader));

                llpc << i << "/" << j << " " << unitResource.resourceHeader.crc32;
                if (crc32_reverse_lookup.count(unitResource.resourceHeader.crc32))
                {
                    llpc << " " << crc32_reverse_lookup[unitResource.resourceHeader.crc32];
                }
                llpc << "\n";

                allLLPC << path.filename().stem().string() << " " << i << " " << j << " " << unitResource.resourceHeader.crc32 << "\n";

                AAA& aaa = crc32s[unitResource.resourceHeader.crc32];
                aaa.className = classFileExtensions[unitResource.resourceHeader.classCRC32];
                aaa.dpcNames.insert(dpcFilePath.stem().string());
                aaa.signedCRC32 = unitResource.resourceHeader.crc32;
				if (crc32_reverse_lookup.count(unitResource.resourceHeader.crc32))
				{
					aaa.text = crc32_reverse_lookup[unitResource.resourceHeader.crc32];
				}

                objectCRC32s.insert(unitResource.resourceHeader.crc32);

                if (unitResource.resourceHeader.dataSize > 0)
                {
                }

                unitResource.classObject.resize(unitResource.resourceHeader.classObjectSize);
                file.read(reinterpret_cast<char*>(unitResource.classObject.data()), unitResource.classObject.size());

                std::ofstream headerFile("unpack\\" + path.filename().stem().string() + "\\" + std::to_string(unitResource.resourceHeader.crc32) + ".header." + std::to_string(unitResource.resourceHeader.classCRC32), std::ios::binary);

                if (!headerFile.good())
                {
                    assert(false);
                }

                headerFile.write(reinterpret_cast<char*>(unitResource.classObject.data()), unitResource.classObject.size());

                unitResource.data.resize(unitResource.resourceHeader.dataSize - unitResource.resourceHeader.classObjectSize);
                file.read(reinterpret_cast<char*>(unitResource.data.data()), unitResource.data.size());

                if (unitResource.resourceHeader.compressedSize || (unitResource.resourceHeader.dataSize != unitResource.resourceHeader.classObjectSize))
                {
                    if (unitResource.resourceHeader.compressedSize)
                    {
                        unitResource.data = unLZ(unitResource.data);
                    }
                    std::ofstream resFile("unpack\\" + path.filename().stem().string() + "\\" + std::to_string(unitResource.resourceHeader.crc32) + "." + std::to_string(unitResource.resourceHeader.classCRC32), std::ios::binary);

                    if (!resFile.good())
                    {
                        assert(false);
                    }

                    resFile.write(reinterpret_cast<char*>(unitResource.data.data()), unitResource.data.size());
                }

                unit.unitResources.push_back(unitResource);
            }

            unit.padding.resize(unitDescription.paddedSize - unitDescription.dataSize);
            file.read(reinterpret_cast<char*>(unit.padding.data()), unit.padding.size());

            units.push_back(unit);
        }

        if (file.peek() == EOF)
        {
            return;
        }

        // Relational Table
        file.read(reinterpret_cast<char*>(&relationalTable.relationalTableHeader), sizeof(relationalTable.relationalTableHeader));

        file.read(reinterpret_cast<char*>(&relationalTable.resourcesCRC32s.count), sizeof(relationalTable.resourcesCRC32s.count));
        relationalTable.resourcesCRC32s.data.resize(relationalTable.resourcesCRC32s.count);
        file.read(reinterpret_cast<char*>(relationalTable.resourcesCRC32s.data.data()), relationalTable.resourcesCRC32s.data.size() * sizeof(std::uint32_t));

        file.read(reinterpret_cast<char*>(&relationalTable.crc32s.count), sizeof(relationalTable.crc32s.count));
        relationalTable.crc32s.data.resize(relationalTable.crc32s.count);
        file.read(reinterpret_cast<char*>(relationalTable.crc32s.data.data()), relationalTable.crc32s.data.size() * sizeof(std::uint32_t));

        file.read(reinterpret_cast<char*>(&relationalTable.referenceCounts.count), sizeof(relationalTable.referenceCounts.count));
        relationalTable.referenceCounts.data.resize(relationalTable.referenceCounts.count);
        file.read(reinterpret_cast<char*>(relationalTable.referenceCounts.data.data()), relationalTable.referenceCounts.data.size() * sizeof(std::uint32_t));

        file.read(reinterpret_cast<char*>(&relationalTable.resourcePaddedSize.count), sizeof(relationalTable.resourcePaddedSize.count));
        relationalTable.resourcePaddedSize.data.resize(relationalTable.resourcePaddedSize.count);
        file.read(reinterpret_cast<char*>(relationalTable.resourcePaddedSize.data.data()), relationalTable.resourcePaddedSize.data.size() * sizeof(std::uint32_t));

        file.read(reinterpret_cast<char*>(&relationalTable.unknown4.count), sizeof(relationalTable.unknown4.count));
        relationalTable.unknown4.data.resize(relationalTable.unknown4.count);
        file.read(reinterpret_cast<char*>(relationalTable.unknown4.data.data()), relationalTable.unknown4.data.size() * sizeof(std::uint32_t));

        file.read(reinterpret_cast<char*>(&relationalTable.unknown5.count), sizeof(relationalTable.unknown5.count));
        relationalTable.unknown5.data.resize(relationalTable.unknown5.count);
        file.read(reinterpret_cast<char*>(relationalTable.unknown5.data.data()), relationalTable.unknown5.data.size() * sizeof(RelationalTableUnknownStrcuture));

        RelationalTableUnknownStrcuture terminal;
        file.read(reinterpret_cast<char*>(&terminal), sizeof(terminal));

        relationalTable.padding.resize(calculatePaddingSize(file.tellg()));
        file.read(reinterpret_cast<char*>(relationalTable.padding.data()), relationalTable.padding.size());

        // Resources
        for (std::uint32_t i = 0; i < relationalTable.resourcesCRC32s.data.size(); ++i)
        {
            Resource resource;
            file.read(reinterpret_cast<char*>(&resource.resourceHeader), sizeof(resource.resourceHeader));
            resource.data.resize(resource.resourceHeader.dataSize);

            if (resource.resourceHeader.dataSize > 0)
            {
            }

            file.read(reinterpret_cast<char*>(resource.data.data()), resource.data.size());

            assert(resource.resourceHeader.classObjectSize == 0);

            if (resource.resourceHeader.compressedSize)
            {
                resource.data = unLZ(resource.data);
            }

            std::ofstream resFile("unpack\\" + path.filename().stem().string() + "\\" + std::to_string(resource.resourceHeader.crc32) + "." + std::to_string(resource.resourceHeader.classCRC32), std::ios::binary);

            if (!resFile.good())
            {
                assert(false);
            }

            resFile.write(reinterpret_cast<char*>(resource.data.data()), resource.data.size());

            resource.padding.resize(calculatePaddingSize(file.tellg()));
            file.read(reinterpret_cast<char*>(resource.padding.data()), resource.padding.size());

            resources.push_back(resource);
        }
    }

    nlohmann::json to_json() const
    {
        std::vector<nlohmann::json> jsonUnits;

        for (const Unit& unit : units)
        {
            jsonUnits.push_back(unit.to_json());
        }

        std::vector<nlohmann::json> jsonResources;

        for (const Resource& resource : resources)
        {
            jsonResources.push_back(resource.to_json());
        }

        return
        {
            { "$path", path.string() },
            { "$fileSize", fileSize },
            { "$fileSize-header", fileSize - sizeof(primaryHeader) },
            { "primaryHeader", primaryHeader.to_json() },
            { "relationalTable", primaryHeader.relationalTableOffset ? relationalTable.to_json() : nlohmann::json() },
            { "units", jsonUnits },
            { "resources", jsonResources },
        };
    }

    std::filesystem::path path;
    std::uint32_t fileSize;
    PrimaryHeader primaryHeader;
    RelationalTable relationalTable;
    std::vector<Unit> units;
    std::vector<Resource> resources;
};

template <class T>
void to_json(nlohmann::json& j, const T& p) {
    j = p.to_json();
}

int main(int argc, const char* argv[])
{
    std::ifstream nnpc("all.nnpc");

    allLLPC.open("ALL.LLPC");
    if (!allLLPC.good())
    {
        return -1;
    }

    std::uint32_t crc32;
    std::string text;

    while (nnpc >> crc32)
    {
        nnpc.get();
        std::getline(nnpc, text);
        crc32_reverse_lookup.insert({crc32, text});
    }

    std::ofstream json("fmtk.json");
    nlohmann::json jsonDPCFiles;

    std::filesystem::create_directory("unpack");

    for (const auto& file : std::filesystem::recursive_directory_iterator("."))
    {
        if (!file.is_directory() && file.path().extension() == ".DPC")
        {
            jsonDPCFiles.push_back(DPCFile(file.path()).to_json());
        }
    }

    //jsonDPCFiles.push_back(DPCFile("DATAS\\\VEH.DPC").to_json());
    //jsonDPCFiles.push_back(DPCFile("DATAS\\\BIKE.DPC").to_json());

    try
    {
        json << jsonDPCFiles.dump(4) << ",\n";
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    //for (std::uint32_t crc32 : classCRC32s)
    //{
    //    std::cout << crc32 << std::endl;
    //}

    //std::cout << std::endl;

    //for (std::uint32_t height : heights)
    //{
    //    std::cout << height << std::endl;
    //}

    std::ofstream crc32ss("crc32s.txt");
    for (auto& crc32 : crc32s)
    {
        crc32ss << crc32.first << "," << crc32.second.signedCRC32 << "," << crc32.second.className << ",";
        for (const std::string& str : crc32.second.dpcNames)
        {
            crc32ss << str << " ";
        }
        crc32ss.seekp(-1 + crc32ss.tellp(), std::ios::beg);
        crc32ss << "," << crc32.second.text << "\n";
    }
}
