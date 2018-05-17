#ifndef DEF_RLE_HANDLER
#define DEF_RLE_HANDLER

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>

#include "rle_code_t_vect.h"

#define BUFFER_SIZE 4096
#define BYTE_SIZE 8

typedef char * str;
typedef uint8_t count_t;
typedef uint8_t byte_t;

size_t _gcd(size_t a, size_t b);

size_t _lcm(size_t a, size_t b);

size_t lcm(size_t a, size_t b);

void encode_file(str raw_path, str enc_path, size_t count_bits);

void decode_file(str enc_path, str raw_path, size_t count_bits);

void buffer_write_at(byte_t *buffer, size_t i_bit, rle_code_t rle_code,
                     size_t rle_code_size);

rle_code_t buffer_read_at(byte_t *buffer, size_t i_bit, size_t rle_code_size);

#endif
