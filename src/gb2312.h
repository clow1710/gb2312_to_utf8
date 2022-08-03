#pragma once

#include <stddef.h>
#include <stdint.h>

/** 按照字节交换一个 16 位的字节的高8位和低8位 */
#define BYTE_SWAP_16(x)           (uint16_t)((((uint16_t) (x) & 0x00FF) << 8) | (((uint16_t) (x) & 0xFF00) >> 8))

/**
 * 将一个 GB2312 编码的字符串转换为 UTF-8 编码
 *
 * @param input_str GB2312 编码的字符串指针，假定字符串尾部含有 '\0' 结束符
 * @param output_str 存放转码后 UTF-8 编码字符串的指针
 * @param max_len 字符串的最大容量
 * @return size_t 成功转换的 UTF-8 编码的字符数量，不包含字符串尾部的'\0'
 */
size_t gb2312_str_to_utf8(const uint8_t *input_str, uint8_t *output_str, size_t max_len);

size_t utf8_str_to_gb2312(const uint8_t *input_str, uint8_t *output_str, size_t buf_len);
