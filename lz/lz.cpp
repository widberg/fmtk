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

std::uint32_t lz_fuel_compress(const std::uint8_t* decompressed_buffer, std::uint32_t decompressed_buffer_size, std::uint8_t* compressed_buffer, std::uint32_t compressed_buffer_size) {
	constexpr std::uint32_t WINDOW_LOG = 14;
	constexpr std::uint32_t WINDOW_MASK = (1 << WINDOW_LOG) - 1;
	constexpr std::uint32_t MATCH_NUM = 30;
	constexpr std::uint32_t MATCH_ITER = 4;
	constexpr std::uint32_t MIN_MATCH_LEN = 3;
	constexpr std::uint32_t MIN_DISTANCE = 1;

	std::uint32_t distances_table[MATCH_NUM][MATCH_ITER]; // distances
	std::uint32_t lengths_table[MATCH_NUM][MATCH_ITER]; // lengths

	const std::uint8_t* decompressed_buffer_ptr = decompressed_buffer;
	const std::uint8_t* decompressed_buffer_end = decompressed_buffer + decompressed_buffer_size;
	std::uint8_t* compressed_buffer_ptr = compressed_buffer;

	while (true) {
		if (decompressed_buffer_ptr >= decompressed_buffer_end) {
			break;
		}

		const std::uint8_t* decompressed_buffer_ptr_backup = decompressed_buffer_ptr;
		std::uint8_t* pflag = compressed_buffer_ptr;
		compressed_buffer_ptr += 4;
		std::uint32_t opt_flag = 0;
		double opt_rate = 0;

		for (std::uint32_t t = 0; t < MATCH_ITER; t++) {

			std::uint32_t flag = 0;
			std::uint32_t ulen = 0;
			std::uint32_t clen = 0;
			decompressed_buffer_ptr = decompressed_buffer_ptr_backup;
			std::uint32_t temp_wlog = WINDOW_LOG - t;
			std::uint32_t temp_mlen = (1 << (16 - temp_wlog)) - 1 + MIN_MATCH_LEN;
			std::uint32_t temp_mask = WINDOW_MASK >> t;

			for (std::uint32_t i = 0; i < MATCH_NUM; i++) {
				distances_table[i][t] = 0;
				lengths_table[i][t] = 1;
			}

			for (std::uint32_t i = 0; i < MATCH_NUM; i++) {
				if (decompressed_buffer_ptr >= decompressed_buffer_end) {
					break;
				}

				std::uint32_t pos = decompressed_buffer_ptr - decompressed_buffer;
				std::uint32_t k = pos - (temp_mask + MIN_DISTANCE);
				if ((k & 0x80000000) != 0) {
					k = 0;
				}
				std::uint32_t l = decompressed_buffer_end - decompressed_buffer_ptr;
				if (l > temp_mlen) {
					l = temp_mlen;
				}
				std::uint32_t ml = 0; // max match len
				std::uint32_t mj = 0; // max match pos
				std::uint32_t start = pos - 1;
				std::uint32_t end = k - 1;
				std::uint32_t j = start;
				while (j != end) {
					std::uint32_t rr = l;
					for (std::uint32_t r = 0; r < l; r++) {
						if (decompressed_buffer_ptr[r] != decompressed_buffer[j + r]) {
							rr = r;
							break;
						}
					}
					if (rr > ml) {
						ml = rr;
						mj = pos - j;
					}

					j--;
				}

				if (ml < MIN_MATCH_LEN) {
					// literal
					ulen += 1;
					decompressed_buffer_ptr++;
					clen += 1;
				}
				else {
					// match
					distances_table[i][t] = mj;
					lengths_table[i][t] = ml;
					flag |= 1 << (31 - i);
					ulen += ml;
					decompressed_buffer_ptr += ml;
					clen += 2;
				}

			} // for

			double new_rate = double(ulen) / (4 + clen);

			if (new_rate > opt_rate) {
				opt_rate = new_rate;
				opt_flag = flag | t;
			}
		}

		*(std::uint32_t*)pflag = _byteswap_ulong(opt_flag); // write as big endian

		std::uint32_t t = opt_flag & 3;
		decompressed_buffer_ptr = decompressed_buffer_ptr_backup;
		std::uint32_t temp_wlog = WINDOW_LOG - t;
		std::uint32_t temp_mask = WINDOW_MASK >> t;

		for (std::uint32_t i = 0; i < MATCH_NUM; i++) {
			if (decompressed_buffer_ptr >= decompressed_buffer_end) {
				break;
			}
			if (opt_flag & (1 << (31 - i))) {
				// match
				std::uint32_t ml = lengths_table[i][t];
				std::uint32_t mj = distances_table[i][t];
				std::uint16_t c = ((ml - MIN_MATCH_LEN) << temp_wlog) + ((mj - MIN_DISTANCE) & temp_mask);
				*(std::uint16_t*)compressed_buffer_ptr = _byteswap_ushort(c);
				compressed_buffer_ptr += 2;
				decompressed_buffer_ptr += ml;
			}
			else {
				// literal
				*compressed_buffer_ptr++ = *decompressed_buffer_ptr++;
			}
		}
	}

	return compressed_buffer_ptr - compressed_buffer;
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
