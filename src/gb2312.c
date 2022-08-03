/**
 * @author 耿浩
 * @date 2022年8月3日
 *
 * @file gb2312.c
 * @brief
 *
 * GB/T 2312，GB/T 2312–80 或 GB/T 2312–1980
 * 是中国国家标准简体中文字符集，字符集中的每个汉字及符号的码位都使用两个字节来表示。 UTF-8 是 Unicode
 * 字符集的一种变长度的编码表达方式，对于全部 GB2312 字符集的元素，其对应长度均为 3 字节。 UTF-16 是 Unicode
 * 字符集的一种定长编码方式，其对于全部 GB2312 字符集的元素的编码长度均为 2 字节。 由于码点不同， Unicode 与 GB2312
 * 字符集之间编码的转换必然要使用 LUT 的方式来实现，故使用 UTF-16 编码来制作 map 更能节约空间。 从而 GB2312
 * 转换器的转换思路为：UTF-8 <-(Codec)-> Unicode <-(Codec)-> UTF-16 <-(LUT)-> GB2312 。
 *
 */

#include "gb2312.h"
#include "utf16_gb2312_map.h"
#include <assert.h>

#define MAX_UTF8_SINGLE_CHAR_ENCODING_BYTES_NUM 6

/** 通过 UTF-8 的首字节计算该字符编码的字节数 */
static size_t get_utf8_byte_len(uint8_t first_byte)
{
    if (first_byte <= 0x7F) {
        return 1;
    }

    size_t len = 0;
    for (size_t s = 7; s > 0; --s) {
        if ((first_byte >> s) == 0) {
            len = 7 - s;
            break;
        }
    }

    return len;
}

/**
 * 将所给 Unicode 码点的字符使用 UTF-8 编码
 *
 * @param unicode 欲编码字符的 Unicode 码点
 * @param start 放置该字符 UTF-8 编码（1 ~ 4 字节）的起始字节地址
 * @return size_t 正常编码时返回该字符 UTF-8 编码的字节数；0 表示编码异常
 */
static size_t encode_utf8(const uint32_t unicode, uint8_t *start)
{
    assert(start);

    if (unicode <= 0x007F) { // 7 位，1 字节，ASCII 兼容
        uint8_t valid = (uint8_t) unicode & 0x7F;
        *start        = valid;
        return 1;
    } else if (unicode <= 0x07FF) { // 11 位， 2 字节
        uint16_t valid = (uint16_t) unicode & 0x07FF;
        *(start + 0)   = (0b110 << 5) | ((valid & (0b11111 << 6)) >> 6);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 2;
    } else if (unicode <= 0xFFFF) { // 16 位，3 字节
        uint16_t valid = (uint16_t) unicode & 0xFFFF;
        *(start + 0)   = (0b1110 << 4) | ((valid & (0b1111 << 12)) >> 12);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 6)) >> 6);
        *(start + 2)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 3;
    } else if (unicode <= 0x001FFFFF) { // 21 位， 4 字节
        uint32_t valid = (uint32_t) unicode & 0x001FFFFF;
        *(start + 0)   = (0b11110 << 3) | ((valid & (0b111 << 18)) >> 18);
        *(start + 1)   = (0b10 << 6) | ((valid & (0b111111 << 12)) >> 12);
        *(start + 2)   = (0b10 << 6) | ((valid & (0b111111 << 6)) >> 6);
        *(start + 3)   = (0b10 << 6) | ((valid & (0b111111 << 0)) >> 0);
        return 4;
    } else {
        return 0;
    }
}

/**
 * 将一个 UTF-8 编码的字符解码为 Unicode 码点。
 *
 * @param utf8 欲解码的 UTF-8 字符编码（1 ~ 4 字节）的起始字节的地址
 * @param p_unicode 放置返回的 Unicode 码点的指针
 * @return size_t 正常解码时返回该字符编码的长度，单位：字节；0 表示解码异常
 */
static size_t decode_utf8(const uint8_t *start, uint32_t *p_unicode)
{
    assert(start);
    assert(p_unicode);

    size_t len = get_utf8_byte_len(*start);
    if (len == 1) {
        *p_unicode = *start;
        return len;
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

/** 将所给 Unicode 码点的字符使用 UTF-16 编码，仅限编码在 0xFFFF 内的字符 */
static uint16_t encode_utf16le(uint32_t unicode)
{
    if (unicode <= 0xFFFF) {
        return BYTE_SWAP_16(unicode);
    } else {
        return 0;
    }
}

/** 将一个 UTF-16 小端序编码的字符解码为 Unicode 码点 */
static uint32_t decode_utf16le(uint16_t utf16)
{
    return BYTE_SWAP_16(utf16);
}

/** 使用二分法查表实现 GB2312 编码到 UTF-16 小端序编码之间的转换（双字节） */
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

/** 使用二分法查表实现 UTF-16 编码到 GB2312 小端序编码之间的转换（双字节） */
static uint16_t convert_utf16le_to_gb2312(const uint16_t utf16le)
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

size_t gb2312_str_to_utf8(const uint8_t *input_str, uint8_t *output_str, size_t max_len)
{
    assert(input_str);
    assert(output_str);

    size_t char_num = 0; /**< 当前输出字符串中 UTF-8 编码的字符数 */
    size_t i = 0, j = 0;


    while (input_str[i] != '\0') {
        uint8_t byte = input_str[i];

        if (byte < 0x7F) { // ASCII 兼容部分
            if (j + 1 > max_len - 1) {
                break;
            }

            i++;
            output_str[j++] = byte;
        } else if ((byte >= 0xA0 && byte < 0xAA) || (byte >= 0xB0 && byte <= 0xF8)) { // GB2312 部分，单次读取 2 字节
            if (!input_str[i + 1]) {
                break;
            }

            uint16_t word    = (((uint16_t) byte) << 8) | input_str[i + 1];
            uint32_t unicode = decode_utf16le(convert_gb2312_to_utf16le(word));
            if (!unicode) {
                break;
            }

            uint8_t buf[MAX_UTF8_SINGLE_CHAR_ENCODING_BYTES_NUM] = {0};

            size_t single = encode_utf8(unicode, buf);
            if (!single || j + single >= max_len - 1) {
                break;
            }

            for (size_t s = 0; s < single; ++s) {
                output_str[j + s] = buf[s];
            }

            i += 2;
            j += single;
        } else { // 其他
            if (j + 1 > max_len - 1) {
                break;
            }

            i++;
            output_str[j++] = byte;
        }

        char_num++;
    }

    output_str[j] = '\0';

    return char_num;
}

size_t utf8_str_to_gb2312(const uint8_t *input_str, uint8_t *output_str, size_t buf_len)
{
    assert(input_str);
    assert(output_str);

    size_t char_num = 0; /**< 当前输出字符串中 GB2312 编码的字符数 */
    size_t i = 0, j = 0;

    while (input_str[i] != '\0') {
        uint8_t  byte      = input_str[i];
        uint32_t unicode   = 0;
        size_t   char_size = decode_utf8(input_str + i, &unicode);

        if (char_size == 3 && unicode != 0) {
            uint16_t gb2312 = convert_utf16le_to_gb2312(encode_utf16le(unicode));
        }
    }

    return 0;
}