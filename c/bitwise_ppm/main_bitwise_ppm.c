#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bitwise_ppm_handler.h"
// #define TEST_RANDOM

#ifdef TEST_RANDOM

#define TEST_LENGTH 1000000

int main(int argc, char **argv) {
    srand(time(NULL));
    ModelHolder *my_model_enc = modelholder__new();
    ModelHolder *my_model_dec = modelholder__new();
    prob_t escape_probs[] = {0.0, 0.05, 0.1, 0.2};
    modelholder__init(my_model_enc, escape_probs);
    modelholder__init(my_model_dec, escape_probs);
    ArithmeticCodingHolder *my_coder = arithmeticcoding__new();
    bit_t my_bit;
    bit_t my_bits[TEST_LENGTH];
    context_t my_context = 0;
    for (size_t i = 0; i < TEST_LENGTH; i++) {
        my_bit = (rand() < RAND_MAX / 2) ? (bit_t) (1) : (bit_t) (0);
        my_bits[i] = my_bit;
        // printf("i: [%lu] ", i);
        encode(my_coder, my_model_enc, my_context, my_bit);
        my_context = (my_context << 1U) | my_bit;
    }
    my_coder =
        arithmeticcoding__from_out_format(
            arithmeticcoding__to_out_format(my_coder));
    // printf("--------------------\n--------------------\n");
    size_t i = 0;
    my_context = 0;
    while ((my_bit = decode(my_coder, my_model_dec, my_context), my_bit) !=
           BIT_STOP) {
        if (my_bit != my_bits[i]) {
            printf("Error %lu", i);
            return 1;
        }
        my_context = (my_context << 1U) | my_bit;
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
    size_t probs_length = argc - 4;
    prob_t *escape_probs = malloc(sizeof(prob_t) * probs_length);
    for (size_t i = 0; i < probs_length; i++) {
        sscanf(argv[4 + i], "%lf", escape_probs + i);
    }
    if (str_eq(mode, "enc")) {
        encode_file(in_path, out_path, escape_probs);
    } else if (str_eq(mode, "dec")) {
        decode_file(in_path, out_path, escape_probs);
    } else {
        return 1;
    }
    return 0;
}

#endif
