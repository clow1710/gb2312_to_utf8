#pragma once

#include <stddef.h>

typedef char *c_str_t;             // C语言字符串类型
typedef const char *c_str_literal; // C语言字符串字面量类型

/**
 * 根据分隔符原地分割一个字符串，源字符串不能是字面量。
 *
 * @param input_str 欲分割的字符串
 * @param delim_str 用作分隔符的字符串
 * @param substring_arr 分割后的子字符串数组
 * @param arr_size 子字符串数组的最大容量
 * @return
 */
int split_c_string(c_str_t input_str, c_str_literal delim_str, c_str_t substring_arr[], size_t arr_size);
