#include <iostream>
#include <filesystem>
#include <cstdint>
#include <fstream>
#include <string>
#include <functional>
#include <cassert>
#include <cstddef>
#include <bitset>
#include <vector>

unsigned indentationLevel = 0;

void tab(std::ostream& out)
{
    for (unsigned i = 0; i < indentationLevel; ++i)
    {
        out << "\t";
    }
}

void openBrace(std::ostream& out)
{
    tab(out);
    out << "{\n";
    indentationLevel++;
}

void closeBrace(std::ostream& out)
{
    indentationLevel--;
    tab(out);
    out << "},\n";
}

void pairString(std::ostream& out, const std::string& key, const std::string& value)
{
    tab(out);
    out << "\"" << key << "\":\"" << value << "\",\n";
}

void pairU32(std::ostream& out, const std::string& key, std::uint32_t value)
{
    tab(out);
    out << "\"" << key << "\":" << value << ",\n";
}

std::uint32_t calculatePaddedSize(std::uint32_t unpaddedSize)
{
    return (unpaddedSize + 0x7ff) & 0xfffff800;
}

std::uint32_t calculatePaddingSize(std::uint32_t unpaddedSize)
{
    return calculatePaddedSize(unpaddedSize) - unpaddedSize;
}

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


    const std::uint8_t* dp = data.data() + 28;

    std::vector<std::uint8_t> decompressed;

    for (i = 0; i < winsize; i++)
        win[i] = 0;

    l = *(std::uint32_t*)dp;
    dp += sizeof(l);

    std::uint32_t len = *(std::uint32_t*)data.data() + 24;

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

class Resource;
class ResourceHeader
{
public:
    friend std::istream& operator >> (std::istream& in, Resource& resource);
    friend std::ostream& operator << (std::ostream& out, const ResourceHeader& resource_header);
    friend std::istream& operator >> (std::istream& in, ResourceHeader& resource_header);
private:
    std::uint32_t dataSize;
    std::uint32_t unknown0;
    std::uint32_t decompressedSize;
    std::uint32_t compressedSize;
    std::uint32_t classCRC32;
    std::uint32_t crc32;
};

std::ostream& operator << (std::ostream& out, const ResourceHeader& resource_header)
{


    return out;
}

std::istream& operator >> (std::istream& in, ResourceHeader& resource_header)
{
    in.read(reinterpret_cast<char*>(&resource_header), sizeof(resource_header));

    return in;
}

class Resource
{
public:
    friend std::ostream& operator << (std::ostream& out, const Resource& resource);
    friend std::istream& operator >> (std::istream& in, Resource& resource);
private:
    ResourceHeader resourceHeader;
    std::vector<std::uint8_t> data;
    std::vector<std::uint8_t> padding;
};

std::ostream& operator << (std::ostream& out, const Resource& resource)
{


    return out;
}

std::istream& operator >> (std::istream& in, Resource& resource)
{
    in >> resource.resourceHeader;

    resource.data.resize(resource.resourceHeader.dataSize);
    in.read(reinterpret_cast<char*>(resource.data.data()), resource.data.size());

    resource.padding.resize(calculatePaddingSize(sizeof(ResourceHeader) + resource.resourceHeader.dataSize));
    in.read(reinterpret_cast<char*>(resource.padding.data()), resource.padding.size());

    return in;
}

class TailUnknownStruct
{
public:
    friend std::ostream& operator << (std::ostream& out, const TailUnknownStruct& tail_unknown_struct);
    friend std::istream& operator >> (std::istream& in, TailUnknownStruct& tail_unknown_struct);
private:
    std::uint32_t unknown0;
    std::uint32_t unknown1;
    std::uint32_t unknown2;
    std::uint32_t unknown3;
    std::uint8_t padding[12];
};

std::ostream& operator << (std::ostream& out, const TailUnknownStruct& tail_unknown_struct)
{


    return out;
}

std::istream& operator >> (std::istream& in, TailUnknownStruct& tail_unknown_struct)
{


    return in;
}

class TailUnknownStructPascalArray
{
public:
    friend std::ostream& operator << (std::ostream& out, const TailUnknownStructPascalArray& tail_unknown_struct_pascal_array);
    friend std::istream& operator >> (std::istream& in, TailUnknownStructPascalArray& tail_unknown_struct_pascal_array);
private:
    std::uint32_t count;
    std::vector<TailUnknownStruct> data;
};

std::ostream& operator << (std::ostream& out, const TailUnknownStructPascalArray& tail_unknown_struct_pascal_array)
{


    return out;
}

std::istream& operator >> (std::istream& in, TailUnknownStructPascalArray& tail_unknown_struct_pascal_array)
{


    return in;
}

class TailU32PascalArray
{
public:
    friend std::ostream& operator << (std::ostream& out, const TailU32PascalArray& tail_u32_pascal_array);
    friend std::istream& operator >> (std::istream& in, TailU32PascalArray& tail_u32_pascal_array);
private:
    std::uint32_t count;
    std::vector<std::uint32_t> data;
};

std::ostream& operator << (std::ostream& out, const TailU32PascalArray& tail_u32_pascal_array)
{


    return out;
}

std::istream& operator >> (std::istream& in, TailU32PascalArray& tail_u32_pascal_array)
{


    return in;
}

class TailHeader
{
public:
    friend std::ostream& operator << (std::ostream& out, const TailHeader& tail_header);
    friend std::istream& operator >> (std::istream& in, TailHeader& tail_header);
private:
    std::uint32_t unused0;
    std::uint32_t unused1;
    std::uint32_t unused2;
};

std::ostream& operator << (std::ostream& out, const TailHeader& tail_header)
{


    return out;
}

std::istream& operator >> (std::istream& in, TailHeader& tail_header)
{


    return in;
}

class UnitResourceHeader
{
public:
    friend std::ostream& operator << (std::ostream& out, const UnitResourceHeader& unit_resource_header);
    friend std::istream& operator >> (std::istream& in, UnitResourceHeader& unit_resource_header);
private:
    std::uint32_t dataSize;
    std::uint32_t unknown0;
    std::uint32_t decompressedSize;
    std::uint32_t compressedSize;
    std::uint32_t classCRC32;
    std::uint32_t crc32;
};

std::ostream& operator << (std::ostream& out, const UnitResourceHeader& unit_resource_header)
{


    return out;
}

std::istream& operator >> (std::istream& in, UnitResourceHeader& unit_resource_header)
{


    return in;
}

class UnitResource
{
public:
    friend std::ostream& operator << (std::ostream& out, const UnitResource& unit_resource);
    friend std::istream& operator >> (std::istream& in, UnitResource& unit_resource);
private:
    UnitResourceHeader resourceHeader;
    std::vector<std::uint8_t> data;
};

std::ostream& operator << (std::ostream& out, const UnitResource& unit_resource)
{


    return out;
}

std::istream& operator >> (std::istream& in, UnitResource& unit_resource)
{


    return in;
}

class Unit
{
public:
    friend std::ostream& operator << (std::ostream& out, const Unit& unit);
    friend std::istream& operator >> (std::istream& in, Unit& unit);
private:
    std::vector<UnitResource> unitResources;
};

std::ostream& operator << (std::ostream& out, const Unit& unit)
{


    return out;
}

std::istream& operator >> (std::istream& in, Unit& unit)
{
    in.read(reinterpret_cast<char*>(&unit_description), sizeof(unit_description));

    return in;
}

class UnitDescription
{
public:
    friend std::ostream& operator << (std::ostream& out, const UnitDescription& unit_description);
    friend std::istream& operator >> (std::istream& in, UnitDescription& unit_description);
private:
    std::uint32_t unitType;
    std::uint32_t resourceCount;
    std::uint32_t paddedSize;
    std::uint32_t dataSize;
    std::uint32_t unknown0;
    std::uint32_t crc32;
};

std::ostream& operator << (std::ostream& out, const UnitDescription& unit_description)
{


    return out;
}

std::istream& operator >> (std::istream& in, UnitDescription& unit_description)
{
    in.read(reinterpret_cast<char*>(&unit_description), sizeof(unit_description));

    return in;
}

class PrimaryHeader
{
public:
    friend std::ostream& operator << (std::ostream& out, const PrimaryHeader& primary_header);
    friend std::istream& operator >> (std::istream& in, PrimaryHeader& primary_header);
private:
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
    std::uint32_t tailPaddedSize;
    std::uint32_t tailOffset;
    std::uint32_t tailUnused0;
    std::uint32_t tailUnused1;
    std::uint32_t tailUnused2;
    std::uint32_t incrediBuilderUnused0;
    std::uint32_t incrediBuilderUnused1;
    std::uint32_t incrediBuilderUnused2;
    char incrediBuilderString[128];
    std::uint8_t padding[64];
};

std::ostream& operator << (std::ostream& out, const PrimaryHeader& primary_header)
{


    return out;
}

std::istream& operator >> (std::istream& in, PrimaryHeader& primary_header)
{
    in.read(reinterpret_cast<char*>(&primary_header), sizeof(primary_header));

    return in;
}

class DPCFile
{
public:
    DPCFile(const std::filesystem::path& path) : m_path(path)
    {
        std::ifstream file(m_path, std::ios::binary | std::ios::ate);
        m_file_size = file.tellg();
        file.seekg(0, std::ios::beg);

        file >> m_primary_header;

        for (;;)
        {

        }
    }

    friend std::ostream& operator << (std::ostream& out, const DPCFile& dpcFile);
private:
    std::filesystem::path m_path;
    std::uint32_t m_file_size;
    PrimaryHeader m_primary_header;
};

std::ostream& operator << (std::ostream& out, const DPCFile& dpcFile)
{
    openBrace(out);

    pairString(out, "path", dpcFile.m_path.string());
    pairU32(out, "fileSize", dpcFile.m_file_size);
    pairU32(out, "fileSize - headerSize", dpcFile.m_file_size - sizeof(dpcFile.m_primary_header));

    out << dpcFile.m_primary_header;

    closeBrace(out);

    return out;
}

int main()
{
    std::ofstream json("fmtk.json");

    for (const auto& file : std::filesystem::recursive_directory_iterator("."))
    {
        if (file.path().extension() == ".DPC")
        {
            json << DPCFile(file.path()) << "," << std::endl;
        }
    }
}
