/**
 * @author Clow.G
 * @date 2022年8月2日
 *
 * @file gb2312_codec.c
 * @brief
 *
 * GB/T 2312，GB/T 2312–80 或 GB/T 2312–1980 是中国国家标准简体中文字符集，字符集中的每个汉字及符号的码位都使用两个字节来表示。
 * UTF-8 是 Unicode 字符集的一种变长度的编码表达方式，对于全部 GB2312 字符集的元素，其对应长度均在 3 字节以下。
 * UTF-16 是 Unicode 字符集的一种定长编码方式，其对于全部 GB2312 字符集的元素的编码长度均为 2 字节。
 * 由于码点不同， Unicode 与 GB2312 字符集之间编码的转换必然要使用 LUT 的方式来实现，故使用 UTF-16 编码来制作 map 更能节约空间。
 * 从而 GB2312 转换器的转换思路为：UTF-8 --> Unicode 码点 --> UTF-16 -(LUT)-> GB2312，
 * 或 GB2312 -(LUT)-> UTF-16 --> Unicode 码点 --> UTF-8 。
 *
 */

#include "gb2312_codec.h"
#include <assert.h>

#define MAX_UTF8_BYTE_LEN 6

extern const uint16_t utf16le_gb2312_map_1[][2];
extern size_t utf16le_gb2312_map_1_size;
extern const uint16_t utf16le_gb2312_map_2[][2];
extern size_t utf16le_gb2312_map_2_size;

static size_t get_utf8_byte_len(uint8_t utf)
{
    return 0;
}

size_t encode_utf8(const uint32_t unicode, uint8_t *start)
{
    assert(start);

    if (unicode <= 0x007F) {
        uint8_t valid = (uint8_t) unicode & 0x7F;// 7 位， 1 字节
        *(start + 0)  = valid;
        return 1;
    } else if (unicode <= 0x07FF) {// 11 位， 2 字节
        uint16_t valid = (uint16_t) unicode & 0x07FF;
        *(start + 0)   = (0b110 << 5) | ((valid & (0b11111 << 6)) >> 6);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 2;
    } else if (unicode <= 0xFFFF) {// 16 位，3 字节
        uint16_t valid = (uint16_t) unicode & 0xFFFF;
        *(start + 0)   = (0b1110 << 4) | ((valid & (0b1111 << 12)) >> 12);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 6)) >> 6);
        *(start + 2)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 3;
    } else if (unicode <= 0x001FFFFF) {// 21 位， 4 字节
        uint32_t valid = (uint32_t) unicode & 0x001FFFFF;
        *(start + 0)   = (0b11110 << 3) | ((valid & (0b111 << 18)) >> 18);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 12)) >> 12);
        *(start + 2)   = (0b10 << 6) | ((valid & (0b111111 << 6)) >> 6);
        *(start + 3)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 4;
    } else if (unicode <= 0x03FFFFFF) {// 26 位，5 字节
        uint32_t valid = (uint32_t) unicode & 0x03FFFFFF;
        *(start + 0)   = (0b111110 << 2) | ((valid & (0b11 << 24)) >> 24);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 18)) >> 18);
        *(start + 2)   = (0b10 << 6) | ((valid & (0b111111 << 12)) >> 12);
        *(start + 3)   = (0b10 << 6) | ((valid & (0b111111 << 6)) >> 6);
        *(start + 4)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 5;
    } else if (unicode <= 0x7FFFFFFF) {// 31 位，6字节
        uint32_t valid = (uint32_t) unicode & 0x7FFFFFFF;
        *(start + 0)   = (0b1111110 << 1) | ((valid & (0b1 << 30)) >> 30);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 24)) >> 24);
        *(start + 2)   = (0b10 << 6) | ((valid & (0b111111 << 18)) >> 18);
        *(start + 3)   = (0b10 << 6) | ((valid & (0b111111 << 12)) >> 12);
        *(start + 4)   = (0b10 << 6) | ((valid & (0b111111 << 6)) >> 6);
        *(start + 5)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 6;
    } else {
        return 0;
    }
}

size_t decode_utf8(const uint8_t *start, uint32_t *p_unicode)
{
    assert(start);
    assert(p_unicode);

    if (*start <= 0x7F) {
        *p_unicode = *start;
        return 1;
    }

    size_t len = 0;
    for (size_t s = 7; s > 0; --s) {
        if ((*start >> s) == 0) {
            len = 7 - s;
            break;
        }
    }

    *p_unicode = (*start & (0b11111111 << (7 - len))) << ((len - 1) * 6);

    for (size_t s = 1; s < len; ++s) {
        if ((*(start + s) >> 6) != 0b10) {
            *p_unicode = 0;
            return 0;
        }

        *p_unicode |= (*(start + s) & 0b00111111) << ((s - 1) * 6);
    }

    return len;
}

uint16_t encode_utf16le(uint32_t unicode)
{
    if (unicode <= 0xFFFF) {
        return BYTE_SWAP_16(unicode);
    } else {
        return 0;
    }
}

uint32_t decode_utf16le(uint16_t utf16)
{
    return BYTE_SWAP_16(utf16);
}

uint16_t convert_gb2312_to_utf16le(const uint16_t gb2312)
{
    size_t mid, low = 0, high = utf16le_gb2312_map_1_size - 1;

    while (low <= high) {
        mid = (low + high) / 2;
        if (gb2312 > utf16le_gb2312_map_1[mid][1]) {
            low = mid + 1;
        } else if (gb2312 < utf16le_gb2312_map_1[mid][1]) {
            high = mid - 1;
        } else {
            return utf16le_gb2312_map_1[mid][0];
        }
    }

    return 0;
}

uint16_t convert_utf16le_to_gb2312(const uint16_t utf16le)
{
    size_t mid, low = 0, high = utf16le_gb2312_map_2_size - 1;

    while (low <= high) {
        mid = (low + high) / 2;
        if (utf16le > utf16le_gb2312_map_2[mid][0]) {
            low = mid + 1;
        } else if (utf16le < utf16le_gb2312_map_2[mid][0]) {
            high = mid - 1;
        } else {
            return utf16le_gb2312_map_2[mid][1];
        }
    }

    return 0;
}

size_t convert_gb2312_str_to_utf8(const uint16_t *gb2312_str, size_t str_byte, uint8_t *utf8_buf, size_t buf_len)
{
    size_t utf8_char_num = 0;
    size_t buff_top      = 0;
    for (size_t s = 0; s < str_byte; ++s) {
        uint16_t unicode = decode_utf16le(convert_gb2312_to_utf16le(gb2312_str[s]));
        if (!unicode) {
            break;
        }

        uint8_t temp_buff[MAX_UTF8_BYTE_LEN] = {0};

        size_t byte_num = encode_utf8(unicode, temp_buff);
        if (!byte_num || buff_top + byte_num >= buf_len - 1) {
            break;
        }

        for (size_t s0 = 0; s0 < byte_num; ++s0) {
            utf8_buf[buff_top + s0] = temp_buff[s0];
        }

        buff_top += byte_num;
        utf8_char_num += 1;
    }

    utf8_buf[buff_top] = '\0';

    return utf8_char_num;
}

size_t convert_utf8_str_to_gb2312(const uint8_t *utf8_str, size_t str_byte, uint8_t *gb2312_buf, size_t buf_len)
{
    size_t buff_top = 0;
}