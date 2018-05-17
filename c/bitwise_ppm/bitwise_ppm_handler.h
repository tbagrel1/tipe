#ifndef DEF_BITWISE_PPM_HANDLER
#define DEF_BITWISE_PPM_HANDLER

#define BUFFER_SIZE 4096

#include "bitwise_ppm.h"
#include "arithmetic_coder.h"

typedef char * str;

void encode_file(
    str raw_path, str enc_path, prob_t *escape_probs);
void decode_file(
    str enc_path, str raw_path, prob_t *escape_probs);

#endif  // DEF_BITWISE_PPM_HANDLER
