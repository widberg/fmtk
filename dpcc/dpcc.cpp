#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <vector>

std::uint32_t calculatePaddedSize(std::uint32_t unpaddedSize)
{
    return (unpaddedSize + 0x7ff) & 0xfffff800;
}

std::uint32_t calculatePaddingSize(std::uint32_t unpaddedSize)
{
    return calculatePaddedSize(unpaddedSize) - unpaddedSize;
}

struct BlockDescription
{
    std::uint32_t blockType;
    std::uint32_t objectCount;
    std::uint32_t paddedSize;
    std::uint32_t dataSize;
    std::uint32_t workingBufferOffset;
    std::uint32_t crc32;
};

class Block
{
public:
    void pushObject(const std::string& objectPath)
    {
        objects.push_back(objectPath);
    }

    BlockDescription getBlockDescription() const
    {
        BlockDescription blockDescription = {
            0,
            objects.size(),
            getPaddedSize(),
            getDataSize(),
            0,
            0
        };

        return blockDescription;
    }

    void write(std::ostream& out) const
    {
        for (const std::string& objectPath : objects)
        {
            std::ifstream in(objectPath, std::ios::ate | std::ios::binary);
            std::uint32_t size = in.tellg();
            in.seekg(0, std::ios::beg);
            std::vector<char> buffer;
            buffer.resize(size);
            in.read(buffer.data(), buffer.size());
            out.write(buffer.data(), buffer.size());
        }
        std::vector<char> fill;
        fill.resize(calculatePaddingSize((getDataSize() == 0) + out.tellp()) + (getDataSize() == 0));
        std::memset(fill.data(), 0, fill.size());
        out.write(fill.data(), fill.size());
    }

    std::uint32_t getDataSize() const
    {
        std::uint32_t dataSize = 0;
        for (const std::string& objectPath : objects)
        {
            std::ifstream out(objectPath, std::ios::ate | std::ios::binary);
            dataSize += out.tellg();
        }
        return dataSize;
    }

    std::uint32_t getPaddedSize() const
    {
        return calculatePaddedSize(getDataSize());
    }
private:
    std::vector<std::string> objects;
};

class DPC
{
public:
    DPC()
    {

    }

    Block& newBlock()
    {
        blocks.push_back({});
        return blocks.back();
    }

    void write(const std::string& outPath) const
    {
        std::ofstream out(outPath, std::ios::binary);

        if (!out.good())
        {
            exit(1);
        }

        char buffer[256] = {};
        std::strcpy(buffer, versionString.c_str());
        std::uint32_t buffer32 = 0;

        out.write(buffer, sizeof(buffer));
        buffer32 = 1;
        out.write((char*)&buffer32, sizeof(buffer32));
        buffer32 = blocks.size();
        out.write((char*)&buffer32, sizeof(buffer32));
        
        buffer32 = 0;
        for (const Block& block : blocks)
        {
            buffer32 = std::max(buffer32, block.getPaddedSize());
        }
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));

        buffer32 = 0;
        for (const Block& block : blocks)
        {
            buffer32 += block.getPaddedSize();
        }
        out.write((char*)&buffer32, sizeof(buffer32));

        buffer32 = 0;
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));

        for (const Block& block : blocks)
        {
            out.write((char*)&block.getBlockDescription(), sizeof(BlockDescription));;
        }

        out.seekp(1824, std::ios::beg);

        char buffer2[128] = {};

        buffer32 = 0;
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write((char*)&buffer32, sizeof(buffer32));
        out.write(buffer2, sizeof(buffer2));
        std::vector<char> fill;
        fill.resize(calculatePaddingSize(out.tellp()));
        std::memset(fill.data(), 0xFF, fill.size());
        out.write(fill.data(), fill.size());

        for (const Block& block : blocks)
        {
            block.write(out);
            std::vector<char> fill;
            fill.resize(calculatePaddingSize(out.tellp()));
            std::memset(fill.data(), 0, fill.size());
            out.write(fill.data(), fill.size());
        }
    }
private:
    std::string versionString = "v1.381.67.09 - Asobo Studio - Internal Cross Technology";
    std::vector<BlockDescription> blockDescription;
    std::vector<Block> blocks;
};

int main(int argc, const char* argv[])
{
    DPC dpc;
    Block& block = dpc.newBlock();
    block.pushObject("D:\\SteamLibrary\\steamapps\\common\\FUEL\\DATAS\\BIKE_files\\4206951608.TLODDATA");

    dpc.write("DATAS\\TEST.DPC");

    return 0;
}
