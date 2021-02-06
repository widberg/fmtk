#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cstdint>
#include <iostream>
#include <cstring>

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
        if (dp >= data.data() + len)
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
    std::uint32_t unknown3;
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
    std::vector<std::uint8_t> data;

    nlohmann::json to_json() const
    {
        if (resourceHeader.classCRC32 == 4096629181 && !resourceHeader.compressedSize)
        {
            const std::uint32_t* pData = reinterpret_cast<const std::uint32_t*>(data.data());

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

            pData = reinterpret_cast<const std::uint32_t*>(data.data() + resourceHeader.classObjectSize);
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

            return
            {
                { "resourceHeader", resourceHeader.to_json() },
                { "$GameObj_Z", gameObj },
            };
        }

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

        for (const UnitResource& unitResource : unitResources)
        {
            jsonUnitResources.push_back(unitResource.to_json());
        }

        return
        {
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

struct DPCFile
{
    DPCFile(const std::filesystem::path& dpcFilePath)
        : path(dpcFilePath)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
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
                unitResource.data.resize(unitResource.resourceHeader.dataSize);
                file.read(reinterpret_cast<char*>(unitResource.data.data()), unitResource.data.size());

                //if (unitResource.resourceHeader.compressedSize)
                //{
                //    std::vector<std::uint8_t> data(unitResource.data.begin() + unitResource.resourceHeader.classObjectSize, unitResource.data.end());
                //    std::uint32_t unLZSize = unLZ(data).size();

                //    if (unitResource.resourceHeader.decompressedSize != unLZSize)
                //    {
                //        std::cout << unitResource.resourceHeader.decompressedSize << " != " << unLZSize << std::endl;
                //    }
                //}

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
            file.read(reinterpret_cast<char*>(resource.data.data()), resource.data.size());

            //if (resource.resourceHeader.compressedSize)
            //{
            //    std::uint32_t unLZSize = unLZ(resource.data).size();

            //    if (resource.resourceHeader.decompressedSize != unLZSize)
            //    {
            //        std::cout << resource.resourceHeader.decompressedSize << " != " << unLZSize << std::endl;
            //    }
            //}

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
            { "relationalTable", relationalTable.to_json() },
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
    std::ofstream json("fmtk.json");
    nlohmann::json jsonDPCFiles;

    for (const auto& file : std::filesystem::recursive_directory_iterator("."))
    {
        if (file.path().extension() == ".DPC")
        {
            jsonDPCFiles.push_back(DPCFile(file.path()).to_json());
        }
    }

    //jsonDPCFiles.push_back(DPCFile("DATAS\\VEH.DPC").to_json());
    //jsonDPCFiles.push_back(DPCFile("DATAS\\BIKE.DPC").to_json());

    try
    {
        json << jsonDPCFiles.dump(4) << ",\n";
    }
    catch (const nlohmann::json::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
