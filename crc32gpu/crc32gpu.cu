#include <cstdio>
#include <cstdint>
#include <cassert>

__host__
inline void cudaAssert(cudaError_t code, const char* file, int line)
{
    if (code != cudaSuccess)
    {
        fprintf(stderr, "CUDA_ASSERT: \"%s\" @ %s : %d\n", cudaGetErrorString(code), file, line);
        exit(code);
    }
}
#define CUDA_ASSERT(code) do { cudaAssert(code, __FILE__, __LINE__); } while(0)

typedef std::uint32_t crc32_t;
#define CRC32_MAX (UINT32_MAX)
#define CRC32_MIN (UINT32_MIN)

#define CRC32_POLYNOMIAL (0x04C11DB7)
#define CRC32_TABLE_SIZE (256)
__device__
const crc32_t crc32_table[CRC32_TABLE_SIZE] = {
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

#define BLOCK_MAX (65535)

#define CHARS_LEN (40)
__constant__ char CHARS[CHARS_LEN + 1] = "-.0123456789>_abcdefghijklmnopqrstuvwxyz";
//     = "-.0123456789>_abcdefghijklmnopqrstuvwxyz"
//     = "_lmnohijkdefgabcxyztuvwpqrs-.>8945670123"
//     = "_tuvwpqrsxyzdefgabclmnohijk45670123>89-."
//     = "89>01234567-._xyzpqrstuvwhijklmnoabcdefg"
//     = "-.1032547698>_acbedgfihkjmlonqpsrutwvyxz"
//     = "_mlonihkjedgfacbyxzutwvqpsr-.>9854761032"
//     = "_utwvqpsryxzedgfacbmlonihkj54761032>98-."
//     = "98>10325476-._yxzqpsrutwvihkjmlonacbedgf"
//     = "_nolmjkhifgdebcazxyvwturspq.->8967452301"
//     = ".-2301674589>_bcafgdejkhinolmrspqvwtuzxy"
//     = "89>23016745.-_zxyrspqvwtujkhinolmbcafgde"
//     = "_vwturspqzxyfgdebcanolmjkhi67452301>89.-"
//     = "_onmlkjihgfedcbazyxwvutsrqp.->9876543210"
//     = ".-3210765498>_cbagfedkjihonmlsrqpwvutzyx"
//     = "98>32107654.-_zyxsrqpwvutkjihonmlcbagfed"
//     = "_wvutsrqpzyxgfedcbaonmlkjih76543210>98.-"
//     = ">8945670123-._xyztuvwpqrslmnohijkdefgabc"
//     = "_pqrstuvwxyzabcdefghijklmno0123456789>-."
//     = "_hijklmnoabcdefgxyzpqrstuvw-.89>01234567"
//     = "-.45670123>89_defgabclmnohijktuvwpqrsxyz"
//     = ">9854761032-._yxzutwvqpsrmlonihkjedgfacb"
//     = "_qpsrutwvyxzacbedgfihkjmlon1032547698>-."
//     = "_ihkjmlonacbedgfyxzqpsrutwv-.98>10325476"
//     = "-.54761032>98_edgfacbmlonihkjutwvqpsryxz"
//     = "_rspqvwtuzxybcafgdejkhinolm2301674589>.-"
//     = ">8967452301.-_zxyvwturspqnolmjkhifgdebca"
//     = ".-67452301>89_fgdebcanolmjkhivwturspqzxy"
//     = "_jkhinolmbcafgdezxyrspqvwtu.-89>23016745"
//     = "_srqpwvutzyxcbagfedkjihonml3210765498>.-"
//     = ">9876543210.-_zyxwvutsrqponmlkjihgfedcba"
//     = ".-76543210>98_gfedcbaonmlkjihwvutsrqpzyx"
//     = "_kjihonmlcbagfedzyxsrqpwvut.-98>32107654"
//     = "_yxzutwvqpsrmlonihkjedgfacb>9854761032-."
//     = "1032547698>-._qpsrutwvyxzacbedgfihkjmlon"
//     = "-.98>10325476_ihkjmlonacbedgfyxzqpsrutwv"
//     = "_edgfacbmlonihkjutwvqpsryxz-.54761032>98"
//     = "_xyztuvwpqrslmnohijkdefgabc>8945670123-."
//     = "0123456789>-._pqrstuvwxyzabcdefghijklmno"
//     = "-.89>01234567_hijklmnoabcdefgxyzpqrstuvw"
//     = "_defgabclmnohijktuvwpqrsxyz-.45670123>89"
//     = "3210765498>.-_srqpwvutzyxcbagfedkjihonml"
//     = "_zyxwvutsrqponmlkjihgfedcba>9876543210.-"
//     = "_gfedcbaonmlkjihwvutsrqpzyx.-76543210>98"
//     = ".-98>32107654_kjihonmlcbagfedzyxsrqpwvut"
//     = "2301674589>.-_rspqvwtuzxybcafgdejkhinolm"
//     = "_zxyvwturspqnolmjkhifgdebca>8967452301.-"
//     = "_fgdebcanolmjkhivwturspqzxy.-67452301>89"
//     = ".-89>23016745_jkhinolmbcafgdezxyrspqvwtu"
//     = "_acbedgfihkjmlonqpsrutwvyxz-.1032547698>"
//     = "-.>9854761032_mlonihkjedgfacbyxzutwvqpsr"
//     = "54761032>98-._utwvqpsryxzedgfacbmlonihkj"
//     = "_yxzqpsrutwvihkjmlonacbedgf98>10325476-."
//     = "_abcdefghijklmnopqrstuvwxyz-.0123456789>"
//     = "-.>8945670123_lmnohijkdefgabcxyztuvwpqrs"
//     = "45670123>89-._tuvwpqrsxyzdefgabclmnohijk"
//     = "_xyzpqrstuvwhijklmnoabcdefg89>01234567-."
//     = ".->9876543210_onmlkjihgfedcbazyxwvutsrqp"
//     = "_cbagfedkjihonmlsrqpwvutzyx.-3210765498>"
//     = "_zyxsrqpwvutkjihonmlcbagfed98>32107654.-"
//     = "76543210>98.-_wvutsrqpzyxgfedcbaonmlkjih"
//     = ".->8967452301_nolmjkhifgdebcazxyvwturspq"
//     = "_bcafgdejkhinolmrspqvwtuzxy.-2301674589>"
//     = "_zxyrspqvwtujkhinolmbcafgde89>23016745.-"
//     = "67452301>89.-_vwturspqzxyfgdebcanolmjkhi"
//     = "_zyxwvutsrqponmlkjihgfedcba>9876543210.-"
//     = "3210765498>.-_srqpwvutzyxcbagfedkjihonml"
//     = ".-98>32107654_kjihonmlcbagfedzyxsrqpwvut"
//     = "_gfedcbaonmlkjihwvutsrqpzyx.-76543210>98"
//     = "_zxyvwturspqnolmjkhifgdebca>8967452301.-"
//     = "2301674589>.-_rspqvwtuzxybcafgdejkhinolm"
//     = ".-89>23016745_jkhinolmbcafgdezxyrspqvwtu"
//     = "_fgdebcanolmjkhivwturspqzxy.-67452301>89"
//     = "1032547698>-._qpsrutwvyxzacbedgfihkjmlon"
//     = "_yxzutwvqpsrmlonihkjedgfacb>9854761032-."
//     = "_edgfacbmlonihkjutwvqpsryxz-.54761032>98"
//     = "-.98>10325476_ihkjmlonacbedgfyxzqpsrutwv"
//     = "0123456789>-._pqrstuvwxyzabcdefghijklmno"
//     = "_xyztuvwpqrslmnohijkdefgabc>8945670123-."
//     = "_defgabclmnohijktuvwpqrsxyz-.45670123>89"
//     = "-.89>01234567_hijklmnoabcdefgxyzpqrstuvw"
//     = "_cbagfedkjihonmlsrqpwvutzyx.-3210765498>"
//     = ".->9876543210_onmlkjihgfedcbazyxwvutsrqp"
//     = "76543210>98.-_wvutsrqpzyxgfedcbaonmlkjih"
//     = "_zyxsrqpwvutkjihonmlcbagfed98>32107654.-"
//     = "_bcafgdejkhinolmrspqvwtuzxy.-2301674589>"
//     = ".->8967452301_nolmjkhifgdebcazxyvwturspq"
//     = "67452301>89.-_vwturspqzxyfgdebcanolmjkhi"
//     = "_zxyrspqvwtujkhinolmbcafgde89>23016745.-"
//     = "-.>9854761032_mlonihkjedgfacbyxzutwvqpsr"
//     = "_acbedgfihkjmlonqpsrutwvyxz-.1032547698>"
//     = "_yxzqpsrutwvihkjmlonacbedgf98>10325476-."
//     = "54761032>98-._utwvqpsryxzedgfacbmlonihkj"
//     = "-.>8945670123_lmnohijkdefgabcxyztuvwpqrs"
//     = "_abcdefghijklmnopqrstuvwxyz-.0123456789>"
//     = "_xyzpqrstuvwhijklmnoabcdefg89>01234567-."
//     = "45670123>89-._tuvwpqrsxyzdefgabclmnohijk"
//     = ".-2301674589>_bcafgdejkhinolmrspqvwtuzxy"
//     = "_nolmjkhifgdebcazxyvwturspq.->8967452301"
//     = "_vwturspqzxyfgdebcanolmjkhi67452301>89.-"
//     = "89>23016745.-_zxyrspqvwtujkhinolmbcafgde"
//     = ".-3210765498>_cbagfedkjihonmlsrqpwvutzyx"
//     = "_onmlkjihgfedcbazyxwvutsrqp.->9876543210"
//     = "_wvutsrqpzyxgfedcbaonmlkjih76543210>98.-"
//     = "98>32107654.-_zyxsrqpwvutkjihonmlcbagfed"
//     = "_lmnohijkdefgabcxyztuvwpqrs-.>8945670123"
//     = "-.0123456789>_abcdefghijklmnopqrstuvwxyz"
//     = "89>01234567-._xyzpqrstuvwhijklmnoabcdefg"
//     = "_tuvwpqrsxyzdefgabclmnohijk45670123>89-."
//     = "_mlonihkjedgfacbyxzutwvqpsr-.>9854761032"
//     = "-.1032547698>_acbedgfihkjmlonqpsrutwvyxz"
//     = "98>10325476-._yxzqpsrutwvihkjmlonacbedgf"
//     = "_utwvqpsryxzedgfacbmlonihkj54761032>98-."
//     = ">8967452301.-_zxyvwturspqnolmjkhifgdebca"
//     = "_rspqvwtuzxybcafgdejkhinolm2301674589>.-"
//     = "_jkhinolmbcafgdezxyrspqvwtu.-89>23016745"
//     = ".-67452301>89_fgdebcanolmjkhivwturspqzxy"
//     = ">9876543210.-_zyxwvutsrqponmlkjihgfedcba"
//     = "_srqpwvutzyxcbagfedkjihonml3210765498>.-"
//     = "_kjihonmlcbagfedzyxsrqpwvut.-98>32107654"
//     = ".-76543210>98_gfedcbaonmlkjihwvutsrqpzyx"
//     = "_pqrstuvwxyzabcdefghijklmno0123456789>-."
//     = ">8945670123-._xyztuvwpqrslmnohijkdefgabc"
//     = "-.45670123>89_defgabclmnohijktuvwpqrsxyz"
//     = "_hijklmnoabcdefgxyzpqrstuvw-.89>01234567"
//     = "_qpsrutwvyxzacbedgfihkjmlon1032547698>-."
//     = ">9854761032-._yxzutwvqpsrmlonihkjedgfacb"
//     = "-.54761032>98_edgfacbmlonihkjutwvqpsryxz"
//     = "_ihkjmlonacbedgfyxzqpsrutwv-.98>10325476"
//     = "rspqvwtuzxybcafgdejkhinolm_2301674589>.-"
//     = ">8967452301.-zxyvwturspqnolmjkhifgdebca_"
//     = ".-67452301>89fgdebcanolmjkhivwturspqzxy_"
//     = "jkhinolmbcafgdezxyrspqvwtu_.-89>23016745"
//     = "srqpwvutzyxcbagfedkjihonml_3210765498>.-"
//     = ">9876543210.-zyxwvutsrqponmlkjihgfedcba_"
//     = ".-76543210>98gfedcbaonmlkjihwvutsrqpzyx_"
//     = "kjihonmlcbagfedzyxsrqpwvut_.-98>32107654"
//     = ">8945670123-.xyztuvwpqrslmnohijkdefgabc_"
//     = "pqrstuvwxyzabcdefghijklmno_0123456789>-."
//     = "hijklmnoabcdefgxyzpqrstuvw_-.89>01234567"
//     = "-.45670123>89defgabclmnohijktuvwpqrsxyz_"
//     = ">9854761032-.yxzutwvqpsrmlonihkjedgfacb_"
//     = "qpsrutwvyxzacbedgfihkjmlon_1032547698>-."
//     = "ihkjmlonacbedgfyxzqpsrutwv_-.98>10325476"
//     = "-.54761032>98edgfacbmlonihkjutwvqpsryxz_"
//     = "nolmjkhifgdebcazxyvwturspq_.->8967452301"
//     = ".-2301674589>bcafgdejkhinolmrspqvwtuzxy_"
//     = "89>23016745.-zxyrspqvwtujkhinolmbcafgde_"
//     = "vwturspqzxyfgdebcanolmjkhi_67452301>89.-"
//     = "onmlkjihgfedcbazyxwvutsrqp_.->9876543210"
//     = ".-3210765498>cbagfedkjihonmlsrqpwvutzyx_"
//     = "98>32107654.-zyxsrqpwvutkjihonmlcbagfed_"
//     = "wvutsrqpzyxgfedcbaonmlkjih_76543210>98.-"
//     = "-.0123456789>abcdefghijklmnopqrstuvwxyz_"
//     = "lmnohijkdefgabcxyztuvwpqrs_-.>8945670123"
//     = "tuvwpqrsxyzdefgabclmnohijk_45670123>89-."
//     = "89>01234567-.xyzpqrstuvwhijklmnoabcdefg_"
//     = "-.1032547698>acbedgfihkjmlonqpsrutwvyxz_"
//     = "mlonihkjedgfacbyxzutwvqpsr_-.>9854761032"
//     = "utwvqpsryxzedgfacbmlonihkj_54761032>98-."
//     = "98>10325476-.yxzqpsrutwvihkjmlonacbedgf_"
//     = ".->9876543210onmlkjihgfedcbazyxwvutsrqp_"
//     = "cbagfedkjihonmlsrqpwvutzyx_.-3210765498>"
//     = "zyxsrqpwvutkjihonmlcbagfed_98>32107654.-"
//     = "76543210>98.-wvutsrqpzyxgfedcbaonmlkjih_"
//     = ".->8967452301nolmjkhifgdebcazxyvwturspq_"
//     = "bcafgdejkhinolmrspqvwtuzxy_.-2301674589>"
//     = "zxyrspqvwtujkhinolmbcafgde_89>23016745.-"
//     = "67452301>89.-vwturspqzxyfgdebcanolmjkhi_"
//     = "acbedgfihkjmlonqpsrutwvyxz_-.1032547698>"
//     = "-.>9854761032mlonihkjedgfacbyxzutwvqpsr_"
//     = "54761032>98-.utwvqpsryxzedgfacbmlonihkj_"
//     = "yxzqpsrutwvihkjmlonacbedgf_98>10325476-."
//     = "abcdefghijklmnopqrstuvwxyz_-.0123456789>"
//     = "-.>8945670123lmnohijkdefgabcxyztuvwpqrs_"
//     = "45670123>89-.tuvwpqrsxyzdefgabclmnohijk_"
//     = "xyzpqrstuvwhijklmnoabcdefg_89>01234567-."
//     = "3210765498>.-srqpwvutzyxcbagfedkjihonml_"
//     = "zyxwvutsrqponmlkjihgfedcba_>9876543210.-"
//     = "gfedcbaonmlkjihwvutsrqpzyx_.-76543210>98"
//     = ".-98>32107654kjihonmlcbagfedzyxsrqpwvut_"
//     = "2301674589>.-rspqvwtuzxybcafgdejkhinolm_"
//     = "zxyvwturspqnolmjkhifgdebca_>8967452301.-"
//     = "fgdebcanolmjkhivwturspqzxy_.-67452301>89"
//     = ".-89>23016745jkhinolmbcafgdezxyrspqvwtu_"
//     = "yxzutwvqpsrmlonihkjedgfacb_>9854761032-."
//     = "1032547698>-.qpsrutwvyxzacbedgfihkjmlon_"
//     = "-.98>10325476ihkjmlonacbedgfyxzqpsrutwv_"
//     = "edgfacbmlonihkjutwvqpsryxz_-.54761032>98"
//     = "xyztuvwpqrslmnohijkdefgabc_>8945670123-."
//     = "0123456789>-.pqrstuvwxyzabcdefghijklmno_"
//     = "-.89>01234567hijklmnoabcdefgxyzpqrstuvw_"
//     = "defgabclmnohijktuvwpqrsxyz_-.45670123>89"
//     = "-.>9854761032mlonihkjedgfacbyxzutwvqpsr_"
//     = "acbedgfihkjmlonqpsrutwvyxz_-.1032547698>"
//     = "yxzqpsrutwvihkjmlonacbedgf_98>10325476-."
//     = "54761032>98-.utwvqpsryxzedgfacbmlonihkj_"
//     = "-.>8945670123lmnohijkdefgabcxyztuvwpqrs_"
//     = "abcdefghijklmnopqrstuvwxyz_-.0123456789>"
//     = "xyzpqrstuvwhijklmnoabcdefg_89>01234567-."
//     = "45670123>89-.tuvwpqrsxyzdefgabclmnohijk_"
//     = "cbagfedkjihonmlsrqpwvutzyx_.-3210765498>"
//     = ".->9876543210onmlkjihgfedcbazyxwvutsrqp_"
//     = "76543210>98.-wvutsrqpzyxgfedcbaonmlkjih_"
//     = "zyxsrqpwvutkjihonmlcbagfed_98>32107654.-"
//     = "bcafgdejkhinolmrspqvwtuzxy_.-2301674589>"
//     = ".->8967452301nolmjkhifgdebcazxyvwturspq_"
//     = "67452301>89.-vwturspqzxyfgdebcanolmjkhi_"
//     = "zxyrspqvwtujkhinolmbcafgde_89>23016745.-"
//     = "1032547698>-.qpsrutwvyxzacbedgfihkjmlon_"
//     = "yxzutwvqpsrmlonihkjedgfacb_>9854761032-."
//     = "edgfacbmlonihkjutwvqpsryxz_-.54761032>98"
//     = "-.98>10325476ihkjmlonacbedgfyxzqpsrutwv_"
//     = "0123456789>-.pqrstuvwxyzabcdefghijklmno_"
//     = "xyztuvwpqrslmnohijkdefgabc_>8945670123-."
//     = "defgabclmnohijktuvwpqrsxyz_-.45670123>89"
//     = "-.89>01234567hijklmnoabcdefgxyzpqrstuvw_"
//     = "zyxwvutsrqponmlkjihgfedcba_>9876543210.-"
//     = "3210765498>.-srqpwvutzyxcbagfedkjihonml_"
//     = ".-98>32107654kjihonmlcbagfedzyxsrqpwvut_"
//     = "gfedcbaonmlkjihwvutsrqpzyx_.-76543210>98"
//     = "zxyvwturspqnolmjkhifgdebca_>8967452301.-"
//     = "2301674589>.-rspqvwtuzxybcafgdejkhinolm_"
//     = ".-89>23016745jkhinolmbcafgdezxyrspqvwtu_"
//     = "fgdebcanolmjkhivwturspqzxy_.-67452301>89"
//     = "pqrstuvwxyzabcdefghijklmno_0123456789>-."
//     = ">8945670123-.xyztuvwpqrslmnohijkdefgabc_"
//     = "-.45670123>89defgabclmnohijktuvwpqrsxyz_"
//     = "hijklmnoabcdefgxyzpqrstuvw_-.89>01234567"
//     = "qpsrutwvyxzacbedgfihkjmlon_1032547698>-."
//     = ">9854761032-.yxzutwvqpsrmlonihkjedgfacb_"
//     = "-.54761032>98edgfacbmlonihkjutwvqpsryxz_"
//     = "ihkjmlonacbedgfyxzqpsrutwv_-.98>10325476"
//     = ">8967452301.-zxyvwturspqnolmjkhifgdebca_"
//     = "rspqvwtuzxybcafgdejkhinolm_2301674589>.-"
//     = "jkhinolmbcafgdezxyrspqvwtu_.-89>23016745"
//     = ".-67452301>89fgdebcanolmjkhivwturspqzxy_"
//     = ">9876543210.-zyxwvutsrqponmlkjihgfedcba_"
//     = "srqpwvutzyxcbagfedkjihonml_3210765498>.-"
//     = "kjihonmlcbagfedzyxsrqpwvut_.-98>32107654"
//     = ".-76543210>98gfedcbaonmlkjihwvutsrqpzyx_"
//     = "lmnohijkdefgabcxyztuvwpqrs_-.>8945670123"
//     = "-.0123456789>abcdefghijklmnopqrstuvwxyz_"
//     = "89>01234567-.xyzpqrstuvwhijklmnoabcdefg_"
//     = "tuvwpqrsxyzdefgabclmnohijk_45670123>89-."
//     = "mlonihkjedgfacbyxzutwvqpsr_-.>9854761032"
//     = "-.1032547698>acbedgfihkjmlonqpsrutwvyxz_"
//     = "98>10325476-.yxzqpsrutwvihkjmlonacbedgf_"
//     = "utwvqpsryxzedgfacbmlonihkj_54761032>98-."
//     = ".-2301674589>bcafgdejkhinolmrspqvwtuzxy_"
//     = "nolmjkhifgdebcazxyvwturspq_.->8967452301"
//     = "vwturspqzxyfgdebcanolmjkhi_67452301>89.-"
//     = "89>23016745.-zxyrspqvwtujkhinolmbcafgde_"
//     = ".-3210765498>cbagfedkjihonmlsrqpwvutzyx_"
//     = "onmlkjihgfedcbazyxwvutsrqp_.->9876543210"
//     = "wvutsrqpzyxgfedcbaonmlkjih_76543210>98.-"
//     = "98>32107654.-zyxsrqpwvutkjihonmlcbagfed_"

__global__
void kernel(std::uint32_t hashOffset)
{
    crc32_t hash = blockIdx.x + hashOffset;
    crc32_t newHash = (hash >> 8) ^ crc32_table[(threadIdx.x ^ hash) & 0xff];

    //for (int i = 0; i < 1048576; ++i)
    //{
    //    std::uint32_t X = ((16 * (268435456 / 256)) * i);
    //    if (newHash == X)
    //    {
    //        printf("%d %u %u %c\n", i, hash, newHash, CHARS[threadIdx.x]);
    //    }
    //}
    std::uint32_t X = ((16 * (268435456 / 256)));
    if (newHash % X == 0)
    {
        printf("%u %u %u\n", newHash / X, hash, threadIdx.x);
    }
}

int main()
{
    for (std::uint32_t i = 0; i < CRC32_MAX / BLOCK_MAX; ++i)
    {
        kernel<<<BLOCK_MAX, 256>>>(i * BLOCK_MAX);
    }
    CUDA_ASSERT(cudaPeekAtLastError());
    CUDA_ASSERT(cudaDeviceSynchronize());
    cudaDeviceReset();
	return 0;
}
