#include <iostream>
#include <filesystem>
#include <cstdint>
#include <fstream>
#include <string>
#include <functional>
#include <cassert>
#include <cstddef>
#include <bitset>

struct DPCHeader
{
    char versionString[256];
    std::uint32_t fileType;
    std::uint32_t unitCount;
    std::uint32_t u0;
    std::uint32_t compressedSize;
    std::uint32_t paddedSize;
    std::uint32_t versionMinor;
    std::uint32_t versionPatch;
};

struct UnitDescriptor
{
    std::uint32_t fileType;
    std::uint32_t subFiles;
    std::uint32_t paddedSize;
    std::uint32_t dataSize;
    std::uint32_t u1;
    std::uint32_t crc32;
};

struct UnitHeader
{
    std::uint32_t u0;
    std::uint32_t u1;
    std::uint32_t u2;
    std::uint32_t u3;
    std::uint32_t u4;
    std::uint32_t crc32;
    std::uint32_t decompressedCRC32;
};

struct FooHeader
{
    std::uint32_t paddedSize;
    std::uint32_t alignment;
    std::uint32_t count;
    std::uint32_t u3;
};

struct ResourceHeader
{
    std::uint32_t dataSize;
    std::uint32_t zero;
    std::uint32_t decompressedSize;
    std::uint32_t compressedSize;
    std::uint32_t resourceType;
    std::uint32_t crc32;
};

#define HEADER_SIZE (0x800U)

#define READ(ifstream, variable) ifstream.read(reinterpret_cast<char*>(&variable), sizeof(variable))

void analyze(const std::filesystem::path& path)
{
    std::ifstream dpc(path, std::ios::binary);
    dpc.seekg(0, std::ios::end);
    std::uint32_t fileSize = static_cast<std::uint32_t>(dpc.tellg());
    dpc.seekg(0, std::ios::beg);

    DPCHeader header;
    READ(dpc, header);

    std::cout <<
        "path = " << path << "\n" <<
        "fileSize = " << fileSize << "\n" <<
        "fileSize - header = " << fileSize - HEADER_SIZE << "\n\n" <<
        "versionString = " << header.versionString << "\n" <<
        "fileType = " << header.fileType << "\n" <<
        "unitCount = " << header.unitCount << "\n" <<
        "u0 = " << header.u0 << "; u0 / 2048 = " << header.u0 / 2048 << "; " << std::bitset<32>(header.u0) << "\n" <<
        "compressedSize = " << header.compressedSize << "; compressedSize / 2048 = " << header.compressedSize / 2048 << "\n" <<
        "paddedSize = " << header.paddedSize << "\n" <<
        "versionMinor = " << header.versionMinor << "\n" <<
        "versionPatch = " << header.versionPatch << std::endl;

    assert((!std::strcmp(header.versionString, "v1.381.67.09 - Asobo Studio - Internal Cross Technology") && header.versionMinor == 272U && header.versionPatch == 380U ||
        !std::strcmp(header.versionString, "v1.381.66.09 - Asobo Studio - Internal Cross Technology") && header.versionMinor == 272U && header.versionPatch == 380U ||
        !std::strcmp(header.versionString, "v1.381.65.09 - Asobo Studio - Internal Cross Technology") && header.versionMinor == 271U && header.versionPatch == 380U ||
        !std::strcmp(header.versionString, "v1.381.64.09 - Asobo Studio - Internal Cross Technology") && header.versionMinor == 271U && header.versionPatch == 380U ||
        !std::strcmp(header.versionString, "v1.379.60.09 - Asobo Studio - Internal Cross Technology") && header.versionMinor == 269U && header.versionPatch == 380U ||
        !std::strcmp(header.versionString, "v1.325.50.07 - Asobo Studio - Internal Cross Technology") && header.versionMinor == 262U && header.versionPatch == 326U ||
        !std::strcmp(header.versionString, "v1.220.50.07 - Asobo Studio - Internal Cross Technology") && header.versionMinor == 262U && header.versionPatch == 221U ||
        0) && "Invalid version");

    assert((header.fileType == 0U ||
        header.fileType == 1U ||
        0) && "Unexpected file type");

    assert((header.u0 % 2048 == 0) && "IDK what header.u0 is");
    assert((header.compressedSize % 2048 == 0) && "IDK what header.u1 is");

    std::uint32_t subFilesSum = 0;
    std::uint32_t u1sum = 0;
    std::uint32_t paddedSizeSum = 0;
    std::uint32_t paddedSections = 0;
    std::uint32_t offset = HEADER_SIZE;

    bool hasCompressed = false;

    for (int i = 0; i < header.unitCount; ++i)
    {
        UnitDescriptor descriptor;
        READ(dpc, descriptor);

        std::cout <<
            "[" << i << "] " << offset << "\n" <<
            "\tfileType = " << descriptor.fileType << "\n" <<
            "\tsubFiles = " << descriptor.subFiles << "\n" <<
            "\tpaddedSize = " << descriptor.paddedSize << "\n" <<
            "\tdataSize = " << descriptor.dataSize << "\n" <<
            "\tu1 = " << descriptor.u1 << "; u1 / 2048 = " << descriptor.u1 / 2048 << "\n" <<
            "\tcrc32 = " << descriptor.crc32 << "\n" << std::endl;

        assert((descriptor.fileType == 0U ||
            descriptor.fileType == 253U ||
            descriptor.fileType == 144U ||
            descriptor.fileType == 146U ||
            descriptor.fileType == 252U ||
            descriptor.fileType == 211U ||
            descriptor.fileType == 249U ||
            0) && "Unexpected file type");

        assert((descriptor.u1 % 2048 == 0) && "IDK what descriptor.u1 is");

        subFilesSum += descriptor.subFiles;
        u1sum += descriptor.u1;
        paddedSizeSum += descriptor.paddedSize;

        if (descriptor.fileType == 0) hasCompressed = true;

        std::streampos pos2 = dpc.tellg();
        dpc.seekg(static_cast<std::streampos>(offset));

        std::vector<char> data;
        data.resize(descriptor.paddedSize);
        dpc.read(data.data(), data.size());
        std::ofstream file("dump\\" + path.stem().string() + "_" + std::to_string(i) + "_" + std::to_string(descriptor.crc32) + "." + std::to_string(descriptor.fileType), std::ios::binary);
        file.write(data.data(), data.size());
        dpc.seekg(pos2);

        UnitHeader unitHeader;
        std::streampos pos = dpc.tellg();
        dpc.seekg(static_cast<std::streampos>(offset));
        READ(dpc, unitHeader);

        std::cout <<
            "\tu0 = " << unitHeader.u0 << "\n" <<
            "\tu1 = " << unitHeader.u1 << "\n" <<
            "\tu2 = " << unitHeader.u2 << "\n" <<
            "\tu3 = " << unitHeader.u3 << "\n" <<
            "\tu4 = " << unitHeader.u4 << "\n" <<
            "\tcrc32 = " << unitHeader.crc32 << "\n" <<
            "\tdecompressedCRC32 = " << unitHeader.decompressedCRC32 << (unitHeader.crc32 == unitHeader.decompressedCRC32 ? " !=!" : "") << "\n" << std::endl;

        assert((unitHeader.u3 ? unitHeader.u0 == unitHeader.u1 + unitHeader.u3 : unitHeader.u0 == unitHeader.u1 + unitHeader.u2) && "u0-3 relation does not hold");
        // assert((unitHeader.u4 == 0x57B1F99E || unitHeader.u4 == 0x52B12EB5) && "u4 is not expected");
        assert((unitHeader.crc32 == descriptor.crc32) && "crc32 does not match");

        dpc.seekg(pos);
        offset += descriptor.paddedSize;
    }

    assert((paddedSizeSum == header.paddedSize) && "Unexpected padded size");
    assert(((hasCompressed == 0) == (header.compressedSize == 0)) && "Has uncompressed size but no compressed files");

    std::uint64_t zero;
    READ(dpc, zero);

    assert(zero == 0 && "Invalid DPC header");

    dpc.seekg(offset);
    FooHeader fooHeader;
    READ(dpc, fooHeader);

    if (!dpc.eof())
    {
        std::cout << "[foo] " << offset << "\n" <<
            "\tpaddedSize = " << fooHeader.paddedSize << "; paddedSize / 2048 = " << fooHeader.paddedSize / 2048 << "\n" <<
            "\talignment = " << fooHeader.alignment << "\n" <<
            "\tcount = " << fooHeader.count << "\n" <<
            "\tu3 = " << fooHeader.u3 << "\n" << std::endl;

        assert((fooHeader.paddedSize % 2048 == 0) && "should be a multiple of 2048");
        assert((fooHeader.paddedSize == 524288) && "should be 524288");
        assert((fooHeader.alignment == 2048) && "should be 2048");
    }

    //dpc.seekg(0x800);
    //std::uint64_t pad;
    //bool arePad = false;
    //while (dpc.read((char*)&pad, sizeof(pad)) && !dpc.eof())
    //{
    //    if (arePad && pad != std::numeric_limits<std::uint64_t>::max() && dpc.tellg() % 2048 == 0)
    //    {
    //        paddedSections++;
    //    }

    //    arePad = pad == std::numeric_limits<std::uint64_t>::max();
    //}

    //std::cout << "\n" <<
    //    "subFilesSum = " << subFilesSum << "\n" <<
    //    "u1sum = " << u1sum << "\n" <<
    //    "padded sections = " << paddedSections << "\n";

    std::cout << "------------------------\n" << std::endl;



    //dpc.seekg(0, std::ios::end);
    //std::uint32_t length = static_cast<std::uint32_t>(dpc.tellg());
    //std::uint32_t size = length - offset;
    //if (size > 0)
    //{
    //    dpc.seekg(static_cast<std::uint32_t>(offset));
    //    std::vector<char> data;
    //    data.resize(size);
    //    dpc.read(data.data(), size);
    //    std::ofstream file("dump\\" + path.stem().string() + "_data.dat");
    //    file.write(data.data(), data.size());
    //}
}

int main()
{
    std::filesystem::create_directory("dump\\");

    for (const auto& file : std::filesystem::recursive_directory_iterator("."))
    {
        if (file.path().extension() == ".DPC")
        {
            analyze(file.path());
        }
    }
}
