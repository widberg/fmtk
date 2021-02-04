#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

#define FIELD(name) { #name, name }

struct RelationalTableUnknownStrcuture
{
    std::uint32_t unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;
    std::uint32_t unknown3;
    std::uint32_t placeholder0;
    std::uint32_t placeholder1;
    std::uint32_t placeholder2;
};

template <class T>
struct PascalArray
{
    std::uint32_t count;
    std::vector<T> data;
};

struct RelationalTableHeader
{
    std::uint32_t unused0;
    std::uint32_t unused1;
    std::uint32_t unused2;
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

struct Resource
{
    ResourceHeader resourceHeader;
    std::vector<std::uint8_t> data;
};

struct UnitResource
{
    ResourceHeader resourceHeader;
    std::vector<std::uint8_t> data;
};

struct Unit
{
    std::vector<UnitResource> unitResources;
};

struct UnitDescription
{
    std::uint32_t unitType;
    std::uint32_t resourceCount;
    std::uint32_t paddedSize;
    std::uint32_t dataSize;
    std::uint32_t workingBufferOffset;
    std::uint32_t crc32;

    nlohmann::json json() const
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

    nlohmann::json json() const
    {
        std::vector<nlohmann::json> jsonUnitDescriptions;

        for (std::uint32_t i = 0; i < unitCount; ++i)
        {
            jsonUnitDescriptions.push_back(unitDescriptions[i].json());
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
            FIELD(incrediBuilderString),
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

        // Relational Table

        // Resources
    }

    nlohmann::json json() const
    {
        return
        {
            { "$path", path.string() },
            { "$fileSize", fileSize },
            { "$fileSize-header", fileSize - sizeof(primaryHeader) },
            { "primaryHeader", primaryHeader.json() },
        };
    }

    std::filesystem::path path;
    std::uint32_t fileSize;
    PrimaryHeader primaryHeader;
};

int main(int argc, const char* argv[])
{
    std::ofstream json("fmtk.json");

    DPCFile dpcFile("DATAS\\BIKE.DPC");

    json << dpcFile.json().dump(4);
}
