#include "unpack_message.h"
#include <string.h>

int split_c_string(c_str_t input_str, c_str_literal delim_str, c_str_t substring_arr[], size_t arr_size)
{
    if (!input_str || !delim_str || !substring_arr || arr_size == 0) {
        return -1;
    }

    memset(substring_arr, 0, arr_size);

    c_str_t temp = strtok(input_str, delim_str);
    for (size_t s = 0; s < arr_size && temp;) {
        substring_arr[s++] = temp;
        temp               = strtok(NULL, delim_str);// Continue processing from the previously parsed position.
    }

    return 0;
}