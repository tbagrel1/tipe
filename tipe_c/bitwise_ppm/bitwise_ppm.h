#ifndef DEF_BITWISE_PPM
#define DEF_BITWISE_PPM

#include "defs.h"

ModelHolder *modelholder__new();

void modelholder__init(
    ModelHolder *p_model_holder, prob_t *escape_probs);

void modelholder__free(ModelHolder *p_model_holder);

void update_escape_prob(
    ModelHolder *p_model_holder, size_t order, bit_t bit);

void update_one_prob(
    ModelHolder *p_model_holder, size_t order, context_t s_context, bit_t bit);

void init_one_prob(
    ModelHolder *p_model_holder, size_t order, context_t s_context, bit_t bit);

void encode(
    ArithmeticCodingHolder *p_output, ModelHolder *p_model_holder,
    context_t context, bit_t bit);

bit_t decode(
    ArithmeticCodingHolder *p_output, ModelHolder *p_model_holder,
    context_t context);

#endif  // DEF_BITWISE_PPM
