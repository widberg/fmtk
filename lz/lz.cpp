#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <intrin.h>

void lz_fuel_decompress(const std::uint8_t* compressed_buffer_ptr, std::uint32_t compressed_buffer_size, std::uint8_t* decompressed_buffer_ptr, std::uint32_t decompressed_buffer_size, bool is_in_place)
{
    // Magic Numbers
    constexpr std::uint32_t WINDOW_LOG = 0xe;
    constexpr std::uint32_t WINDOW_MASK = 0x3fff;

    std::uint8_t* end_ptr = decompressed_buffer_ptr + decompressed_buffer_size;

    while (true)
    {
        std::uint32_t flags = _byteswap_ulong(*(std::uint32_t*)compressed_buffer_ptr); // read as big endian
        std::uint8_t len = flags & 0x3; // 0b11
        std::uint8_t temp_shift = WINDOW_LOG - len;
        std::uint32_t temp_mask = WINDOW_MASK >> len;
        compressed_buffer_ptr += 4;

        for (std::uint8_t i = 0; i < 30; ++i, flags <<= 1)
        {
            if ((flags & 0x80000000) != 0)
            {
                std::uint16_t temp = _byteswap_ushort(*(std::uint16_t*)compressed_buffer_ptr); // read as big endian
                compressed_buffer_ptr += 2;

                std::uint8_t* window_ptr = decompressed_buffer_ptr - ((temp & temp_mask) + 1);
                for (std::uint32_t length = (temp >> temp_shift) + 3; length > 0; --length)
                {
                    *(decompressed_buffer_ptr++) = *(window_ptr++);
                }
            }
            else
            {
                *(decompressed_buffer_ptr++) = *(compressed_buffer_ptr++);
            }

            if ((decompressed_buffer_ptr == end_ptr) || (is_in_place && (decompressed_buffer_ptr > compressed_buffer_ptr)))
            {
                return;
            }
        }
    }
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
    std::vector<std::uint8_t> outData;

    inData.resize(inSize);

    in.read(reinterpret_cast<char*>(inData.data()), inData.size());

    struct LZHeader
    {
        std::uint32_t decompressedSize;
        std::uint32_t compressedSize;
    } lzHeader = *(LZHeader*)inData.data();

    outData.resize(lzHeader.decompressedSize);

    lz_fuel_decompress(inData.data() + sizeof(LZHeader), lzHeader.compressedSize, outData.data(), lzHeader.decompressedSize, false);

    out.write(reinterpret_cast<char*>(outData.data()), outData.size());

    return 0;
}
