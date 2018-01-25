#ifndef TIPE_C_BWT_2_H
#define TIPE_C_BWT_2_H

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned char *str;

typedef __uint32_t index_t;  // Blocks up to 4 294 967 296 o
#define INDEX_SIZE (sizeof(index_t) / sizeof(uchar))

int comp_lex(const void *, const void *);

str bwt_code_block(str, size_t);

int comp_ichar(const void *, const void *);

str bwt_decode_block(str, size_t);

#endif //TIPE_C_BWT_2_H
