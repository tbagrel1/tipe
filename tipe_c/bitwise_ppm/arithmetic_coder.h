#ifndef DEF_ARITHMETIC_CODER
#define DEF_ARTIHMETIC_CODER

#include "defs.h"

ArithmeticCodingHolder *arithmeticcoding__new();

void arithmeticcoding__encode(
    ArithmeticCodingHolder *p_output, prob_t one_prob, prob_t escape_prob,
    bit_t bit);

acode_t_vect *
arithmeticcoding__to_out_format(ArithmeticCodingHolder *p_output);

ArithmeticCodingHolder *
arithmeticcoding__from_out_format(acode_t_vect *p_result);

bit_t arithmeticcoding__decode(
    ArithmeticCodingHolder *p_output, prob_t one_prob, prob_t escape_prob);

bit_t _arithmeticcoding__decode_until(
    ArithmeticCodingHolder *p_output, prob_t one_prob, prob_t escape_prob,
    prob_t delta_stop);

void arithmeticcoding__reset_min_max(ArithmeticCodingHolder *p_output);

#endif  // DEF_ARITHMETIC_CODER
