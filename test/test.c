#include "../src/gb2312.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_LINE_BUF_LEN 128

static int test(const char *path);

int main(int argc, char *argv[])
{
    if (argc == 2) {
        test(argv[1]);
    } else {
        printf("[ERR] Wrong arguments number!\n");
    }

    return 0;
}

static int test(const char *path)
{
    assert(path);

    FILE *fp = NULL;

    fp = fopen(path, "r");
    if (!fp) {
        printf("[ERR] Failed to open the file.\n");
        return -1;
    }

    char buf_0[MAX_LINE_BUF_LEN] = {0};
    char buf_1[MAX_LINE_BUF_LEN] = {0};
    while (1) {
        if (fgets(buf_0, MAX_LINE_BUF_LEN - 1, fp)) {
            transcode_str_gb2312_to_utf8((uint8_t *)buf_0, (uint8_t *)buf_1, MAX_LINE_BUF_LEN - 1);
            printf("%s", buf_1);
        } else {
            break;
        }
    }
    putc('\n', stdout);
    fclose(fp);
    return 0;
}
