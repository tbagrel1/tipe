#include <stdio.h>
#include <stdlib.h>

#include "rle_handler.h"

typedef char * str;
bool str_eq(str a, str b) {
    size_t i = 0;
    while (a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) {
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}


int main(int argc, char **argv) {
    str mode = argv[1];
    str in_path = argv[2];
    str out_path = argv[3];
    size_t count_bits;
    sscanf(argv[4], "%lu", &count_bits);
    if (str_eq(mode, "enc")) {
        encode_file(in_path, out_path, count_bits);
    } else if (str_eq(mode, "dec")) {
        decode_file(in_path, out_path, count_bits);
    } else {
        return 1;
    }
    return 0;
}
