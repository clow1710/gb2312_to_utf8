#include "gb2312.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>


#define MAX_BUFF_LEN 32

static int print_utf8_str(const uint8_t *utf8_str);

int main()
{
    const uint8_t gb2312_str[] = {0x31, 0x32, 0x41, 0x42, 0xC4, 0xE3, 0xCE, 0xD2, 0xA3, 0xA1, 0x21, 0x00};

    printf("GB2312 转换到 UTF-8 测试：\n");
    uint8_t buff[MAX_BUFF_LEN] = {0};
    size_t  char_num           = transcode_str_gb2312_to_utf8(gb2312_str, buff, MAX_BUFF_LEN);
    printf("共转换了 %zu 个字符。\n", char_num);
    print_utf8_str(buff);

    return 0;
}

/**
 * 尝试以 UTF-8 的编码读取并输出一个字符串
 *
 * @param utf8_str 指向该字符串首个字节的指针
 * @return 执行正常返回0
 */
static int print_utf8_str(const uint8_t *utf8_str)
{
    assert(utf8_str);

    printf("原始字符串以 UTF-8 格式输出为：\n%s\n", (char *) utf8_str);
    printf("该以十六进制读取该字符串数据：\n");
    for (size_t s = 0; utf8_str[s] != '\0'; ++s) {
        printf("0x%02X, ", utf8_str[s]);
    }
    putc('\n', stdout);

    return 0;
}
