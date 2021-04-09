#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <cstdint>
#include <cctype>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <functional>

#define CRC32_POLYNOMIAL (0x04C11DB7)
#define CRC32_TABLE_SIZE (256)
const std::uint32_t crc32_table[CRC32_TABLE_SIZE] = {
    0x00000000, 0x04C11DB7, 0x09823B6E, 0x0D4326D9, 0x130476DC, 0x17C56B6B, 0x1A864DB2, 0x1E475005,  //   0 [0x00 .. 0x07]
    0x2608EDB8, 0x22C9F00F, 0x2F8AD6D6, 0x2B4BCB61, 0x350C9B64, 0x31CD86D3, 0x3C8EA00A, 0x384FBDBD,  //   8 [0x08 .. 0x0F]
    0x4C11DB70, 0x48D0C6C7, 0x4593E01E, 0x4152FDA9, 0x5F15ADAC, 0x5BD4B01B, 0x569796C2, 0x52568B75,  //  16 [0x10 .. 0x17]
    0x6A1936C8, 0x6ED82B7F, 0x639B0DA6, 0x675A1011, 0x791D4014, 0x7DDC5DA3, 0x709F7B7A, 0x745E66CD,  //  24 [0x18 .. 0x1F]
    0x9823B6E0, 0x9CE2AB57, 0x91A18D8E, 0x95609039, 0x8B27C03C, 0x8FE6DD8B, 0x82A5FB52, 0x8664E6E5,  //  32 [0x20 .. 0x27]
    0xBE2B5B58, 0xBAEA46EF, 0xB7A96036, 0xB3687D81, 0xAD2F2D84, 0xA9EE3033, 0xA4AD16EA, 0xA06C0B5D,  //  40 [0x28 .. 0x2F]
    0xD4326D90, 0xD0F37027, 0xDDB056FE, 0xD9714B49, 0xC7361B4C, 0xC3F706FB, 0xCEB42022, 0xCA753D95,  //  48 [0x30 .. 0x37]
    0xF23A8028, 0xF6FB9D9F, 0xFBB8BB46, 0xFF79A6F1, 0xE13EF6F4, 0xE5FFEB43, 0xE8BCCD9A, 0xEC7DD02D,  //  56 [0x38 .. 0x3F]
    0x34867077, 0x30476DC0, 0x3D044B19, 0x39C556AE, 0x278206AB, 0x23431B1C, 0x2E003DC5, 0x2AC12072,  //  64 [0x40 .. 0x47]
    0x128E9DCF, 0x164F8078, 0x1B0CA6A1, 0x1FCDBB16, 0x018AEB13, 0x054BF6A4, 0x0808D07D, 0x0CC9CDCA,  //  72 [0x48 .. 0x4F]
    0x7897AB07, 0x7C56B6B0, 0x71159069, 0x75D48DDE, 0x6B93DDDB, 0x6F52C06C, 0x6211E6B5, 0x66D0FB02,  //  80 [0x50 .. 0x57]
    0x5E9F46BF, 0x5A5E5B08, 0x571D7DD1, 0x53DC6066, 0x4D9B3063, 0x495A2DD4, 0x44190B0D, 0x40D816BA,  //  88 [0x58 .. 0x5F]
    0xACA5C697, 0xA864DB20, 0xA527FDF9, 0xA1E6E04E, 0xBFA1B04B, 0xBB60ADFC, 0xB6238B25, 0xB2E29692,  //  96 [0x60 .. 0x67]
    0x8AAD2B2F, 0x8E6C3698, 0x832F1041, 0x87EE0DF6, 0x99A95DF3, 0x9D684044, 0x902B669D, 0x94EA7B2A,  // 104 [0x68 .. 0x6F]
    0xE0B41DE7, 0xE4750050, 0xE9362689, 0xEDF73B3E, 0xF3B06B3B, 0xF771768C, 0xFA325055, 0xFEF34DE2,  // 112 [0x70 .. 0x77]
    0xC6BCF05F, 0xC27DEDE8, 0xCF3ECB31, 0xCBFFD686, 0xD5B88683, 0xD1799B34, 0xDC3ABDED, 0xD8FBA05A,  // 120 [0x78 .. 0x7F]
    0x690CE0EE, 0x6DCDFD59, 0x608EDB80, 0x644FC637, 0x7A089632, 0x7EC98B85, 0x738AAD5C, 0x774BB0EB,  // 128 [0x80 .. 0x87]
    0x4F040D56, 0x4BC510E1, 0x46863638, 0x42472B8F, 0x5C007B8A, 0x58C1663D, 0x558240E4, 0x51435D53,  // 136 [0x88 .. 0x8F]
    0x251D3B9E, 0x21DC2629, 0x2C9F00F0, 0x285E1D47, 0x36194D42, 0x32D850F5, 0x3F9B762C, 0x3B5A6B9B,  // 144 [0x90 .. 0x97]
    0x0315D626, 0x07D4CB91, 0x0A97ED48, 0x0E56F0FF, 0x1011A0FA, 0x14D0BD4D, 0x19939B94, 0x1D528623,  // 152 [0x98 .. 0x9F]
    0xF12F560E, 0xF5EE4BB9, 0xF8AD6D60, 0xFC6C70D7, 0xE22B20D2, 0xE6EA3D65, 0xEBA91BBC, 0xEF68060B,  // 160 [0xA0 .. 0xA7]
    0xD727BBB6, 0xD3E6A601, 0xDEA580D8, 0xDA649D6F, 0xC423CD6A, 0xC0E2D0DD, 0xCDA1F604, 0xC960EBB3,  // 168 [0xA8 .. 0xAF]
    0xBD3E8D7E, 0xB9FF90C9, 0xB4BCB610, 0xB07DABA7, 0xAE3AFBA2, 0xAAFBE615, 0xA7B8C0CC, 0xA379DD7B,  // 176 [0xB0 .. 0xB7]
    0x9B3660C6, 0x9FF77D71, 0x92B45BA8, 0x9675461F, 0x8832161A, 0x8CF30BAD, 0x81B02D74, 0x857130C3,  // 184 [0xB8 .. 0xBF]
    0x5D8A9099, 0x594B8D2E, 0x5408ABF7, 0x50C9B640, 0x4E8EE645, 0x4A4FFBF2, 0x470CDD2B, 0x43CDC09C,  // 192 [0xC0 .. 0xC7]
    0x7B827D21, 0x7F436096, 0x7200464F, 0x76C15BF8, 0x68860BFD, 0x6C47164A, 0x61043093, 0x65C52D24,  // 200 [0xC8 .. 0xCF]
    0x119B4BE9, 0x155A565E, 0x18197087, 0x1CD86D30, 0x029F3D35, 0x065E2082, 0x0B1D065B, 0x0FDC1BEC,  // 208 [0xD0 .. 0xD7]
    0x3793A651, 0x3352BBE6, 0x3E119D3F, 0x3AD08088, 0x2497D08D, 0x2056CD3A, 0x2D15EBE3, 0x29D4F654,  // 216 [0xD8 .. 0xDF]
    0xC5A92679, 0xC1683BCE, 0xCC2B1D17, 0xC8EA00A0, 0xD6AD50A5, 0xD26C4D12, 0xDF2F6BCB, 0xDBEE767C,  // 224 [0xE0 .. 0xE7]
    0xE3A1CBC1, 0xE760D676, 0xEA23F0AF, 0xEEE2ED18, 0xF0A5BD1D, 0xF464A0AA, 0xF9278673, 0xFDE69BC4,  // 232 [0xE8 .. 0xEF]
    0x89B8FD09, 0x8D79E0BE, 0x803AC667, 0x84FBDBD0, 0x9ABC8BD5, 0x9E7D9662, 0x933EB0BB, 0x97FFAD0C,  // 240 [0xF0 .. 0xF7]
    0xAFB010B1, 0xAB710D06, 0xA6322BDF, 0xA2F33668, 0xBCB4666D, 0xB8757BDA, 0xB5365D03, 0xB1F740B4,  // 248 [0xF8 .. 0xFF]
};

typedef std::uint32_t crc32_t;

struct pair_t
{
    crc32_t crc32;
    std::uint8_t i;
};
#define REVERSE_CRC32_TABLE_SIZE (256U)
pair_t reverse_crc32_table[REVERSE_CRC32_TABLE_SIZE] = {
    { 0x00000000, 0x00 }, { 0x018aeb13, 0x4c }, { 0x029f3d35, 0xd4 }, { 0x0315d626, 0x98 }, { 0x04c11db7, 0x01 }, { 0x054bf6a4, 0x4d }, { 0x065e2082, 0xd5 }, { 0x07d4cb91, 0x99 },
    { 0x0808d07d, 0x4e }, { 0x09823b6e, 0x02 }, { 0x0a97ed48, 0x9a }, { 0x0b1d065b, 0xd6 }, { 0x0cc9cdca, 0x4f }, { 0x0d4326d9, 0x03 }, { 0x0e56f0ff, 0x9b }, { 0x0fdc1bec, 0xd7 },
    { 0x1011a0fa, 0x9c }, { 0x119b4be9, 0xd0 }, { 0x128e9dcf, 0x48 }, { 0x130476dc, 0x04 }, { 0x14d0bd4d, 0x9d }, { 0x155a565e, 0xd1 }, { 0x164f8078, 0x49 }, { 0x17c56b6b, 0x05 },
    { 0x18197087, 0xd2 }, { 0x19939b94, 0x9e }, { 0x1a864db2, 0x06 }, { 0x1b0ca6a1, 0x4a }, { 0x1cd86d30, 0xd3 }, { 0x1d528623, 0x9f }, { 0x1e475005, 0x07 }, { 0x1fcdbb16, 0x4b },
    { 0x2056cd3a, 0xdd }, { 0x21dc2629, 0x91 }, { 0x22c9f00f, 0x09 }, { 0x23431b1c, 0x45 }, { 0x2497d08d, 0xdc }, { 0x251d3b9e, 0x90 }, { 0x2608edb8, 0x08 }, { 0x278206ab, 0x44 },
    { 0x285e1d47, 0x93 }, { 0x29d4f654, 0xdf }, { 0x2ac12072, 0x47 }, { 0x2b4bcb61, 0x0b }, { 0x2c9f00f0, 0x92 }, { 0x2d15ebe3, 0xde }, { 0x2e003dc5, 0x46 }, { 0x2f8ad6d6, 0x0a },
    { 0x30476dc0, 0x41 }, { 0x31cd86d3, 0x0d }, { 0x32d850f5, 0x95 }, { 0x3352bbe6, 0xd9 }, { 0x34867077, 0x40 }, { 0x350c9b64, 0x0c }, { 0x36194d42, 0x94 }, { 0x3793a651, 0xd8 },
    { 0x384fbdbd, 0x0f }, { 0x39c556ae, 0x43 }, { 0x3ad08088, 0xdb }, { 0x3b5a6b9b, 0x97 }, { 0x3c8ea00a, 0x0e }, { 0x3d044b19, 0x42 }, { 0x3e119d3f, 0xda }, { 0x3f9b762c, 0x96 },
    { 0x40d816ba, 0x5f }, { 0x4152fda9, 0x13 }, { 0x42472b8f, 0x8b }, { 0x43cdc09c, 0xc7 }, { 0x44190b0d, 0x5e }, { 0x4593e01e, 0x12 }, { 0x46863638, 0x8a }, { 0x470cdd2b, 0xc6 },
    { 0x48d0c6c7, 0x11 }, { 0x495a2dd4, 0x5d }, { 0x4a4ffbf2, 0xc5 }, { 0x4bc510e1, 0x89 }, { 0x4c11db70, 0x10 }, { 0x4d9b3063, 0x5c }, { 0x4e8ee645, 0xc4 }, { 0x4f040d56, 0x88 },
    { 0x50c9b640, 0xc3 }, { 0x51435d53, 0x8f }, { 0x52568b75, 0x17 }, { 0x53dc6066, 0x5b }, { 0x5408abf7, 0xc2 }, { 0x558240e4, 0x8e }, { 0x569796c2, 0x16 }, { 0x571d7dd1, 0x5a },
    { 0x58c1663d, 0x8d }, { 0x594b8d2e, 0xc1 }, { 0x5a5e5b08, 0x59 }, { 0x5bd4b01b, 0x15 }, { 0x5c007b8a, 0x8c }, { 0x5d8a9099, 0xc0 }, { 0x5e9f46bf, 0x58 }, { 0x5f15adac, 0x14 },
    { 0x608edb80, 0x82 }, { 0x61043093, 0xce }, { 0x6211e6b5, 0x56 }, { 0x639b0da6, 0x1a }, { 0x644fc637, 0x83 }, { 0x65c52d24, 0xcf }, { 0x66d0fb02, 0x57 }, { 0x675a1011, 0x1b },
    { 0x68860bfd, 0xcc }, { 0x690ce0ee, 0x80 }, { 0x6a1936c8, 0x18 }, { 0x6b93dddb, 0x54 }, { 0x6c47164a, 0xcd }, { 0x6dcdfd59, 0x81 }, { 0x6ed82b7f, 0x19 }, { 0x6f52c06c, 0x55 },
    { 0x709f7b7a, 0x1e }, { 0x71159069, 0x52 }, { 0x7200464f, 0xca }, { 0x738aad5c, 0x86 }, { 0x745e66cd, 0x1f }, { 0x75d48dde, 0x53 }, { 0x76c15bf8, 0xcb }, { 0x774bb0eb, 0x87 },
    { 0x7897ab07, 0x50 }, { 0x791d4014, 0x1c }, { 0x7a089632, 0x84 }, { 0x7b827d21, 0xc8 }, { 0x7c56b6b0, 0x51 }, { 0x7ddc5da3, 0x1d }, { 0x7ec98b85, 0x85 }, { 0x7f436096, 0xc9 },
    { 0x803ac667, 0xf2 }, { 0x81b02d74, 0xbe }, { 0x82a5fb52, 0x26 }, { 0x832f1041, 0x6a }, { 0x84fbdbd0, 0xf3 }, { 0x857130c3, 0xbf }, { 0x8664e6e5, 0x27 }, { 0x87ee0df6, 0x6b },
    { 0x8832161a, 0xbc }, { 0x89b8fd09, 0xf0 }, { 0x8aad2b2f, 0x68 }, { 0x8b27c03c, 0x24 }, { 0x8cf30bad, 0xbd }, { 0x8d79e0be, 0xf1 }, { 0x8e6c3698, 0x69 }, { 0x8fe6dd8b, 0x25 },
    { 0x902b669d, 0x6e }, { 0x91a18d8e, 0x22 }, { 0x92b45ba8, 0xba }, { 0x933eb0bb, 0xf6 }, { 0x94ea7b2a, 0x6f }, { 0x95609039, 0x23 }, { 0x9675461f, 0xbb }, { 0x97ffad0c, 0xf7 },
    { 0x9823b6e0, 0x20 }, { 0x99a95df3, 0x6c }, { 0x9abc8bd5, 0xf4 }, { 0x9b3660c6, 0xb8 }, { 0x9ce2ab57, 0x21 }, { 0x9d684044, 0x6d }, { 0x9e7d9662, 0xf5 }, { 0x9ff77d71, 0xb9 },
    { 0xa06c0b5d, 0x2f }, { 0xa1e6e04e, 0x63 }, { 0xa2f33668, 0xfb }, { 0xa379dd7b, 0xb7 }, { 0xa4ad16ea, 0x2e }, { 0xa527fdf9, 0x62 }, { 0xa6322bdf, 0xfa }, { 0xa7b8c0cc, 0xb6 },
    { 0xa864db20, 0x61 }, { 0xa9ee3033, 0x2d }, { 0xaafbe615, 0xb5 }, { 0xab710d06, 0xf9 }, { 0xaca5c697, 0x60 }, { 0xad2f2d84, 0x2c }, { 0xae3afba2, 0xb4 }, { 0xafb010b1, 0xf8 },
    { 0xb07daba7, 0xb3 }, { 0xb1f740b4, 0xff }, { 0xb2e29692, 0x67 }, { 0xb3687d81, 0x2b }, { 0xb4bcb610, 0xb2 }, { 0xb5365d03, 0xfe }, { 0xb6238b25, 0x66 }, { 0xb7a96036, 0x2a },
    { 0xb8757bda, 0xfd }, { 0xb9ff90c9, 0xb1 }, { 0xbaea46ef, 0x29 }, { 0xbb60adfc, 0x65 }, { 0xbcb4666d, 0xfc }, { 0xbd3e8d7e, 0xb0 }, { 0xbe2b5b58, 0x28 }, { 0xbfa1b04b, 0x64 },
    { 0xc0e2d0dd, 0xad }, { 0xc1683bce, 0xe1 }, { 0xc27dede8, 0x79 }, { 0xc3f706fb, 0x35 }, { 0xc423cd6a, 0xac }, { 0xc5a92679, 0xe0 }, { 0xc6bcf05f, 0x78 }, { 0xc7361b4c, 0x34 },
    { 0xc8ea00a0, 0xe3 }, { 0xc960ebb3, 0xaf }, { 0xca753d95, 0x37 }, { 0xcbffd686, 0x7b }, { 0xcc2b1d17, 0xe2 }, { 0xcda1f604, 0xae }, { 0xceb42022, 0x36 }, { 0xcf3ecb31, 0x7a },
    { 0xd0f37027, 0x31 }, { 0xd1799b34, 0x7d }, { 0xd26c4d12, 0xe5 }, { 0xd3e6a601, 0xa9 }, { 0xd4326d90, 0x30 }, { 0xd5b88683, 0x7c }, { 0xd6ad50a5, 0xe4 }, { 0xd727bbb6, 0xa8 },
    { 0xd8fba05a, 0x7f }, { 0xd9714b49, 0x33 }, { 0xda649d6f, 0xab }, { 0xdbee767c, 0xe7 }, { 0xdc3abded, 0x7e }, { 0xddb056fe, 0x32 }, { 0xdea580d8, 0xaa }, { 0xdf2f6bcb, 0xe6 },
    { 0xe0b41de7, 0x70 }, { 0xe13ef6f4, 0x3c }, { 0xe22b20d2, 0xa4 }, { 0xe3a1cbc1, 0xe8 }, { 0xe4750050, 0x71 }, { 0xe5ffeb43, 0x3d }, { 0xe6ea3d65, 0xa5 }, { 0xe760d676, 0xe9 },
    { 0xe8bccd9a, 0x3e }, { 0xe9362689, 0x72 }, { 0xea23f0af, 0xea }, { 0xeba91bbc, 0xa6 }, { 0xec7dd02d, 0x3f }, { 0xedf73b3e, 0x73 }, { 0xeee2ed18, 0xeb }, { 0xef68060b, 0xa7 },
    { 0xf0a5bd1d, 0xec }, { 0xf12f560e, 0xa0 }, { 0xf23a8028, 0x38 }, { 0xf3b06b3b, 0x74 }, { 0xf464a0aa, 0xed }, { 0xf5ee4bb9, 0xa1 }, { 0xf6fb9d9f, 0x39 }, { 0xf771768c, 0x75 },
    { 0xf8ad6d60, 0xa2 }, { 0xf9278673, 0xee }, { 0xfa325055, 0x76 }, { 0xfbb8bb46, 0x3a }, { 0xfc6c70d7, 0xa3 }, { 0xfde69bc4, 0xef }, { 0xfef34de2, 0x77 }, { 0xff79a6f1, 0x3b },
};

crc32_t reverse_crc320s(crc32_t hash, int count)
{
    while (count)
    {
        const pair_t& pair = reverse_crc32_table[hash >> 24];
        hash = ((hash ^ pair.crc32) << 8) | (pair.i);
        --count;
    }

    return hash;
}

std::uint32_t crc32(const std::string& str)
{
    std::uint32_t value = 0;

    for (char c : str)
    {
        value = (value >> 8) ^ crc32_table[(std::tolower(c) ^ value) & 0xff];
    }

    return value;
}

std::uint32_t crc32Prechecked(const char* str)
{
    std::uint32_t value = 0;

    while (*str != '\0')
    {
        value = (value >> 8) ^ crc32_table[(*str ^ value) & 0xff];
        ++str;
    }

    return value;
}

std::uint32_t crc32raw(const char* str, int len)
{
    std::uint32_t value = 0;

    while (len)
    {
        value = (value >> 8) ^ crc32_table[(*str ^ value) & 0xff];
        ++str;
        --len;
    }

    return value;
}

std::uint32_t crc3240suffix( std::uint32_t value)
{
    value = (value >> 8) ^ crc32_table[value & 0xff];
    value = (value >> 8) ^ crc32_table[value & 0xff];
    value = (value >> 8) ^ crc32_table[value & 0xff];
    value = (value >> 8) ^ crc32_table[value & 0xff];

    return value;
}

std::uint32_t crc320suffix( std::uint32_t value, std::uint32_t len)
{
    for (; len; --len)
    {
        value = (value >> 8) ^ crc32_table[value & 0xff];
    }

    return value;
}

std::uint32_t crc32rawprefix(std::uint32_t value, const char* str, int len)
{
    while (len)
    {
        value = (value >> 8) ^ crc32_table[(*str ^ value) & 0xff];
        ++str;
        --len;
    }

    return value;
}

void generateReverseTableFromNames(std::istream& in, std::ostream& out)
{
    out << "std::unordered_map<std::uint32_t, std::string> crc32_reverse_lookup = {\n";

    std::string line;
    while (!std::getline(in, line).eof())
    {
        out << "\t{ " << crc32(line) << ", \"" << line << "\" },\n";
    }

    out << "};\n";
}

std::uint32_t crc32PrecheckedPrefix(std::uint32_t value, const std::string& str)
{
    for (char c : str)
    {
        value = (value >> 8) ^ crc32_table[(c ^ value) & 0xff];
    }

    return value;
}

std::uint32_t crc32PrecheckedAppend(std::uint32_t hash, char c)
{
    return (hash >> 8) ^ crc32_table[(c ^ hash) & 0xff];
}

void generateNPCFromCRC32s(std::istream& in, std::ostream& out)
{
    std::unordered_map<std::uint32_t, std::vector<std::string>> results;

    std::uint32_t value;
    while (!(in >> value).eof())
    {
        results[value] = {};
    }

    std::function<void(std::string, int)> searchAllKLengthRec;
    searchAllKLengthRec = [&](std::string prefix, int k)
    {
        static const char chars[] = "0123456789abcdefghijklmnopqrstuvwxyz_.>";
        static const int chars_len = sizeof(chars) - 1;

        if (k == 0)
        {
            std::uint32_t value = crc32(prefix);
            if (results.count(value))
            {
                results[value].push_back(prefix);
            }
            return;
        }

        for (int i = 0; i < chars_len; i++)
        {
            std::string newPrefix;
            newPrefix = prefix + chars[i];
            searchAllKLengthRec(newPrefix, k - 1);
        }
    };

    auto searchAllKLength = [&](int k)
    {
        searchAllKLengthRec("", k);
    };

    searchAllKLength(1);

    for (auto pair : results)
    {
        out << pair.first << "\n";
        for (auto name : pair.second)
        {
            out << "\t" << name << "\n";
        }
    }
    out << "\n";
}

double profile()
{
    static constexpr unsigned int times = 1000000;

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < times; ++i)
    {
        crc32Prechecked("DB:>LEVELS>COURSES3>QUARTIER_1>3DNODEGEOMETRY>BARRIERECLIGNOTANTEROT_009.TROTSHAPE");
    }

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / (double)times;
}

void previous(std::uint32_t hash)
{
    std::uint32_t start = hash * 256;
    static const char chars[] = "0123456789abcdefghijklmnopqrstuvwxyz_.>-";
    static const int chars_len = sizeof(chars) - 1;
    for (int i = 0; i < chars_len; ++i)
    {
        std::cout << start + chars[i] << std::endl;
    }
}

int main(int argc, const char* argv[])
{
    std::ifstream inn("in.txt");
    std::string line;
    while (!std::getline(inn, line).eof())
    {
        std::cout << crc32(line) << " " << line << "\n";
    }
    return 0;
    /*
    std::uint32_t a = crc32("abcdef");
    std::uint32_t b = crc32raw("abc\0\0\0", 6) ^ crc32("def");
    std::uint32_t c = crc32("abc") ^ reverse_crc320s(crc32("def"), 3);

    std::cout << a << " " << b << " " << c << "\n";

    return 0;*/
    //return crc32("DEFAULTLIGHT");

    //std::uint32_t len = 0;
    //while (true)
    //{
    //    std::cout << crc320suffix(len, 1) << "\n";
    //    if (len == std::numeric_limits<std::uint32_t>::max()) break;
    //    ++len;
    //}

    //return 0;

    /*std::uint32_t prefix = 0;
    while (true)
    {


        std::uint32_t str = crc32PrecheckedPrefix(prefix, "widberg");

        std::uint32_t t = 0;
        while (true)
        {
            if (str == crc32rawprefix(prefix, (char*)&t, 4))
            {
                std::cout << prefix << " " << t << "\n";
                break;
            }
            if (t == std::numeric_limits<std::uint32_t>::max()) break;
            ++t;
        }

        if (prefix == std::numeric_limits<std::uint32_t>::max()) return 0;
        ++prefix;
    }

    std::cout << crc32("abcdwidberg") << " " << (crc32raw("abcd\0\0\0\0\0", 9) ^ crc32("efghi")) << "\n";
    return 0;

    for (int i = 0; i < 1000; ++i)
    {
        std::cout << crc32PrecheckedPrefix(i, "fuel") << "\n";
    }

    return 0;*/

    //std::ofstream ooo("crcstuffs.txt");

    //for (int i = 0; i < 100; ++i)
    //{
    //    // DB:>LEVELS>SHARED>INTERFACE>INTERFACE_039.TBITMAP
    //    // DB:>LEVELS>SHARED>FX>FX_010.TBITMAP
    //    // _BF_MATERIAL_0
    //    std::stringstream ss;
    //    ss << "DB:>LEVELS>SHARED>INTERFACE>INTERFACE_" << std::setw(3) << std::setfill('0') << i << ".TBITMAP";
    //    std::int32_t scrc = crc32(ss.str());
    //    ooo << crc32(ss.str() + "_BF_MATERIAL_0") << " " << ss.str() + "_BF_MATERIAL_0" << "\n";
    //    ooo << crc32(ss.str() + "_BF_MATERIAL_1") << " " << ss.str() + "_BF_MATERIAL_1" << "\n";
    //    ooo << crc32(ss.str() + "_BF_MATERIAL_2") << " " << ss.str() + "_BF_MATERIAL_2" << "\n";
    //    ooo << crc32(ss.str() + "_BF_MATERIAL_Z_" + std::to_string(scrc)) << " " << ss.str() + "_BF_MATERIAL_Z_" + std::to_string(scrc) << "\n";

    //    std::stringstream sss;
    //    sss << "DB:>LEVELS>SHARED>FX>FX_" << std::setw(3) << std::setfill('0') << i << ".TBITMAP";
    //    std::int32_t sscrc = crc32(sss.str());
    //    ooo << crc32(sss.str() + "_BF_MATERIAL_0") << " " << sss.str() + "_BF_MATERIAL_0" << "\n";
    //    ooo << crc32(sss.str() + "_BF_MATERIAL_1") << " " << sss.str() + "_BF_MATERIAL_1" << "\n";
    //    ooo << crc32(sss.str() + "_BF_MATERIAL_2") << " " << sss.str() + "_BF_MATERIAL_2" << "\n";
    //    ooo << crc32(sss.str() + "_BF_MATERIAL_Z_" + std::to_string(sscrc)) << " " << sss.str() + "_BF_MATERIAL_Z_" + std::to_string(sscrc) << "\n";
    //}

    //return 0;

    if (argc < 4)
    {
        return 1;
    }

    std::string option(argv[1]);
    int operation = 0;
    if (option == "rtn")
    {
        operation = 1;
    }
    else if (option == "npcc")
    {
        operation = 2;
    }
    else
    {
        return 4;
    }

    std::ifstream in(argv[2]);
    if (!in.good())
    {
        return 2;
    }

    std::ofstream out(argv[3]);
    if (!out.good())
    {
        return 3;
    }

    switch (operation)
    {
    case 1:
        generateReverseTableFromNames(in, out);
        break;
    case 2:
        generateNPCFromCRC32s(in, out);
        break;
    default:
        return 5;
    }

    return 0;
}
