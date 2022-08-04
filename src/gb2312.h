#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * 将一个 GB2312 编码的字符串转换为 UTF-8 编码
 *
 * @param input_str GB2312 编码的字符串指针，假定字符串尾部含有 '\0' 结束符
 * @param output_str 存放转码后 UTF-8 编码字符串的指针
 * @param max_len 字符串的最大容量
 * @return size_t 成功转换的 UTF-8 编码的字符数量，不包含字符串尾部的'\0'
 */
__attribute__((unused)) size_t transcode_str_gb2312_to_utf8(const uint8_t *input_str, uint8_t *output_str, size_t max_len);