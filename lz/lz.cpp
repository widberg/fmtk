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

        for (std::uint8_t i = 0; i < 30; ++i)
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

            flags <<= 1;
        }
    }
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)

typedef std::uint16_t word;
typedef std::uint32_t   uint;
typedef std::uint8_t  byte;

uint flen(FILE* f) {
    fseek(f, 0, SEEK_END);
    uint len = ftell(f);
    fseek(f, 0, SEEK_SET);
    return len;
}

enum {
    WINDOW_LOG = 14,
    WINDOW_MASK = (1 << WINDOW_LOG) - 1,
    MATCH_NUM = 30,
    MATCH_ITER = 4,
    MIN_MATCH_LEN = 3,
    MIN_DISTANCE = 1
};
//byte win[winsize];

std::uint32_t lz_fuel_compress(const std::uint8_t* decompressed_buffer_ptr, std::uint32_t decompressed_buffer_size, std::uint8_t* compressed_buffer_ptr, std::uint32_t compressed_buffer_size) {
    uint c, d, r, l, ml, mj, i, j, k, t, flag, opt_flag, pos, ulen, clen;

    double opt_rate, new_rate;
    uint D[MATCH_NUM][MATCH_ITER]; // distances
    uint L[MATCH_NUM][MATCH_ITER]; // lengths

    const byte* inpptr; const byte* p_inpptr; const byte* next = 0;
    byte* outptr;
    byte* pflag;

    //  for( i=0; i<winsize; i++ ) win[i]=0;

    for (pos = 0, inpptr = decompressed_buffer_ptr, outptr = compressed_buffer_ptr;; ) {

        if (inpptr >= next) {
            fprintf(stderr, "inp=%i/%i out=%i\r", int(inpptr - decompressed_buffer_ptr), decompressed_buffer_size, int(outptr - compressed_buffer_ptr));
            next = inpptr + 0x10000; if (next > decompressed_buffer_ptr + decompressed_buffer_size) next = decompressed_buffer_ptr + decompressed_buffer_size;
        }

        if (inpptr >= decompressed_buffer_ptr + decompressed_buffer_size) break;

        p_inpptr = inpptr; pflag = outptr; outptr += 4;
        opt_flag = 0; opt_rate = 0;

        for (t = 0; t < MATCH_ITER; t++) {

            flag = 0; ulen = 0; clen = 0; inpptr = p_inpptr;
            uint temp_wlog = WINDOW_LOG - t, temp_mlen = (1 << (16 - temp_wlog)) - 1 + MIN_MATCH_LEN;
            uint temp_mask = WINDOW_MASK >> t;

            for (i = 0; i < MATCH_NUM; i++) { D[i][t] = 0; L[i][t] = 1; }

            for (i = 0; i < MATCH_NUM; i++) {
                if (inpptr >= decompressed_buffer_ptr + decompressed_buffer_size) break;

                pos = inpptr - decompressed_buffer_ptr;
                k = pos - (temp_mask + MIN_DISTANCE); if (int(k) < 0) k = 0;
                l = decompressed_buffer_ptr + decompressed_buffer_size - inpptr; if (l > temp_mlen) l = temp_mlen;
                ml = 0; // max match len
                mj = 0; // max match pos
                for (j = pos - 1; j != k - 1; j--) {
                    for (r = 0; r < l; r++) if (inpptr[r] != decompressed_buffer_ptr[j + r]) break;
                    if (r > ml) { ml = r; mj = pos - j; }
                }

                if (ml < MIN_MATCH_LEN) {
                    // literal
                    ulen += 1; inpptr++;
                    clen += 1;
                }
                else {
                    // match
                    D[i][t] = mj; L[i][t] = ml;
                    flag |= 1 << (31 - i);
                    ulen += ml; inpptr += ml;
                    clen += 2;
                }

            } // for

            new_rate = double(ulen) / (4 + clen);

            if (new_rate > opt_rate) opt_rate = new_rate, opt_flag = flag | t;
        }

        pflag[0] = opt_flag >> 24;
        pflag[1] = opt_flag >> 16;
        pflag[2] = opt_flag >> 8;
        pflag[3] = opt_flag;

        t = opt_flag & 3; inpptr = p_inpptr;
        uint temp_wlog = WINDOW_LOG - t, temp_mlen = (1 << temp_wlog) + MIN_MATCH_LEN;
        uint temp_mask = WINDOW_MASK >> t;

        for (i = 0; i < MATCH_NUM; i++) {
            if (inpptr >= decompressed_buffer_ptr + decompressed_buffer_size) break;
            if (opt_flag & (1 << (31 - i))) {
                // match
                ml = L[i][t]; mj = D[i][t];
                c = ((ml - MIN_MATCH_LEN) << temp_wlog) + ((mj - MIN_DISTANCE) & temp_mask);
                *outptr++ = c >> 8;
                *outptr++ = c;
                inpptr += ml;
            }
            else {
                // literal
                *outptr++ = *inpptr++;
            }
        }
    }

    fprintf(stderr, "\n");

    return outptr - compressed_buffer_ptr;
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

#if 0
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
#else
    std::vector<std::uint8_t> inData;
    std::vector<std::uint8_t> outData;

    inData.resize(inSize);

    in.read(reinterpret_cast<char*>(inData.data()), inData.size());

    outData.resize(inSize * 2);

    std::uint32_t len = lz_fuel_compress(inData.data(), inSize, outData.data(), inSize * 2);
    std::uint32_t full_length = len + 4;

    out.write(reinterpret_cast<char*>(&inSize), sizeof(inSize));
    out.write(reinterpret_cast<char*>(&full_length), sizeof(len));

	outData.resize(len);

    out.write(reinterpret_cast<char*>(outData.data()), outData.size());
#endif

    return 0;
}
