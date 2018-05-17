#ifndef DEF_BITWISE_PPM_FLAT_HANDLER
#define DEF_BITWISE_PPM_FLAT_HANDLER

#define BUFFER_SIZE 4096

#include "bitwise_ppm_flat.h"
#include "arithmetic_coder_flat.h"

typedef char * str;

void encode_file(
    str raw_path, str enc_path, size_t context_size);
void decode_file(
    str enc_path, str raw_path, size_t context_size);

#endif
