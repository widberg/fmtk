#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

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
    std::uint8_t windowBuffer[WINDOW_SIZE] = { 0 };
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

int main(int argc, const char* argv[])
{
    if (argc < 3)
    {
        return 1;
    }

    std::ifstream in(argv[1], std::ios::binary | std::ios::ate);
    if (!in.good())
    {
        return 2;
    }
    
    std::uint32_t inSize = in.tellg();
    in.seekg(0, std::ios::beg);

    std::ofstream out(argv[2], std::ios::binary);
    if (!out.good())
    {
        return 3;
    }

    std::vector<std::uint8_t> inData;
    inData.resize(inSize);

    in.read(reinterpret_cast<char*>(inData.data()), inData.size());

    std::vector<std::uint8_t> outData = decompress(inData);

    out.write(reinterpret_cast<char*>(outData.data()), outData.size());

    return 0;
}
