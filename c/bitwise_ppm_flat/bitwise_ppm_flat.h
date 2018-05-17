#ifndef DEF_BITWISE_PPM_FLAT
#define DEF_BITWISE_PPM_FLAT

#include "defs.h"

ModelCell *modelcells__new(size_t context_size);

void modelcells__init(ModelCell *modelcells, size_t context_size);

void update_one_prob(ModelCell *modelcells, context_t context, bit_t bit);

void init_one_prob(ModelCell *modelcells, context_t context, bit_t bit);

void modelcells__free(ModelCell *modelcells);

void encode(
    ArithmeticCodingHolder *p_output, ModelCell *modelcells, context_t context,
    bit_t bit);

bit_t decode(
    ArithmeticCodingHolder *p_output, ModelCell *modelcells,
    context_t context);

#endif
