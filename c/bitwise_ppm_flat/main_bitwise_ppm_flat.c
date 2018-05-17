#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitwise_ppm_flat_handler.h"
// #define TEST_RANDOM

#ifdef TEST_RANDOM

#define TEST_LENGTH 1000000

int main(int argc, char **argv) {
    size_t context_size;
    sscanf(argv[1], "%lu", &context_size);

    srand(time(NULL));
    ModelCell *modelcells = modelcells__new(context_size);
    modelcells__init(modelcells, context_size);
    context_t mask = (1U << context_size) - 1;
    ArithmeticCodingHolder *acoder = arithmeticcoding__new();
    bit_t bit;
    bit_t bits[TEST_LENGTH];
    context_t context = 0;
    for (size_t i = 0; i < TEST_LENGTH; i++) {
        bit = (rand() < RAND_MAX / 2) ? (bit_t) (1) : (bit_t) (0);
        bits[i] = bit;
        encode(acoder, modelcells, context, bit);
        context = ((context << 1U) | bit) & mask;
    }
    modelcells__free(modelcells);
    modelcells = modelcells__new(context_size);
    modelcells__init(modelcells, context_size);
    acoder =
        arithmeticcoding__from_out_format(
            arithmeticcoding__to_out_format(acoder));
    size_t i = 0;
    context = 0;
    while ((bit = decode(acoder, modelcells, context), bit) != BIT_STOP) {
        if (bit != bits[i]) {
            printf("Error %lu", i);
            return 1;
        }
        context = ((context << 1U) | bit) & mask;
        i++;
    }
    printf("Ok");
    return 0;
}

#else

typedef char * str;
bool str_eq(str a, str b) {
    size_t i = 0;
    while (a[i] != '\0' && b[i] != '\0' && a[i] == b[i]) {
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

int main(int argc, char **argv) {
    // Not secured at all
    str mode = argv[1];
    str in_path = argv[2];
    str out_path = argv[3];
    size_t context_size;
    sscanf(argv[4], "%lu", &context_size);
    if (str_eq(mode, "enc")) {
        encode_file(in_path, out_path, context_size);
    } else if (str_eq(mode, "dec")) {
        decode_file(in_path, out_path, context_size);
    } else {
        return 1;
    }
    return 0;
}

#endif
