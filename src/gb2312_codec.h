#pragma once

#include <stddef.h>
#include <stdint.h>

/** 按照字节交换一个 16 位的字节 */
#define BYTE_SWAP_16(x)           (uint16_t)((((uint16_t) (x) & 0x00FF) << 8) | (((uint16_t) (x) & 0xFF00) >> 8))

/**
 * 将所给 Unicode 码点的字符使用 UTF-8 编码
 *
 * @param unicode 欲编码字符的 Unicode 码点
 * @param start 放置该字符 UTF-8 编码的起始指针（1 ~ 3 字节）
 * @return size_t 正常编码时返回该字符 UTF-8 编码的长度，单位：字节；0 表示编码异常
 */
size_t encode_utf8(uint32_t unicode, uint8_t *start);

/**
 * 将一个 UTF-8 编码的字符解码为 Unicode 码点。
 *
 * @param utf8 欲解码的 UTF-8 字符编码的起始字节地址（1 ~ 3 字节）
 * @param p_unicode 放置返回的 Unicode 码点的指针
 * @return size_t 正常解码时返回该字符编码的长度，单位：字节；0 表示解码异常
 */
size_t decode_utf8(const uint8_t *start, uint32_t *p_unicode);

/**
 * 将所给 Unicode 码点的字符使用 UTF-16 编码
 *
 * @param unicode 欲编码字符的 Unicode 码点
 */
uint16_t encode_utf16le(uint32_t unicode);

/**
 * 将一个 UTF-16 小端序编码的字符解码为 Unicode 码点。
 *
 * @param utf16 欲解码的 UTF-16 字符小端序编码，仅限编码在 0xFFFF 内的字符
 * @return uint32_t  该字符的 Unicode 码点
 */
uint32_t decode_utf16le(uint16_t utf16);

/**
 * 使用二分法查表实现 GB2312 编码到 UTF-16 小端序编码之间的转换
 *
 * @param gb2312 欲转换的字符编码
 * @return 转换后的编码, 转换失败返回 0
 */
uint16_t convert_gb2312_to_utf16le(uint16_t gb2312);

/**
 * 使用二分法查表实现 UTF-16 编码到 GB2312 小端序编码之间的转换
 *
 * @param utf16le 欲转换的字符编码
 * @return 转换后的编码, 转换失败返回 0
 */
uint16_t convert_utf16le_to_gb2312(uint16_t utf16le);

/**
 * 将一个 GB2312 编码的字符串转换为 UTF-8 编码
 *
 * @param gb2312_str 字符串指针
 * @param str_byte 字符串长度
 * @param utf8_buf 存放 UTF-8 编码字符串的 buff 指针
 * @param buf_len buff 的最大容量
 * @return
 */
size_t convert_gb2312_str_to_utf8(const uint16_t *gb2312_str, size_t str_byte, uint8_t *utf8_buf, size_t buf_len);


