#include "gb2312_codec.h"
#include "unpack_message.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>


#define ELEMENT_NUM_IN_ARRAY(arr) (sizeof(arr) / sizeof(arr[0]))
#define MAX_BUFF_LEN              64

static int print_utf8_str(const uint8_t *utf8_str);

int main()
{
    //    const uint8_t utf8_str[]   = {0xE4, 0xBD, 0xA0, 0xE5, 0xA5, 0xBD, 0xEF, 0xBC, 0x8C, 0xE4, 0xB8, 0x96, 0xE7, 0x95, 0x8C, 0xEF, 0xBC, 0x81, 0x00};
    const uint16_t gb2312_str[] = {0xC4E3, 0xBAC3, 0xA3AC, 0xCAC0, 0xBDE7, 0xA3A1, 0x0000};

    printf("GB2312 转换到 UTF-8 测试：\n");
    uint8_t buff[MAX_BUFF_LEN] = {0};
    size_t char_num = convert_gb2312_str_to_utf8(gb2312_str, ELEMENT_NUM_IN_ARRAY(gb2312_str) - 1, buff, MAX_BUFF_LEN);
    printf("共转换了%zu个字符。\n", char_num);
    print_utf8_str(buff);

    printf("UTF-8 转换到 GB2312 测试：\n");


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
