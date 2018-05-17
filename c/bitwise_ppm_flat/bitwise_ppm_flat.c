#include "bitwise_ppm_flat.h"
#include "arithmetic_coder_flat.h"

ModelCell *modelcells__new(size_t context_size) {
    return (ModelCell *) (malloc(sizeof(ModelCell) * (1U << context_size)));
}

void modelcells__free(ModelCell *modelcells) {
    free(modelcells);
}

void modelcells__init(ModelCell *modelcells, size_t context_size) {
    context_t max_context = (1U << context_size);
    for (context_t i = 0; i < max_context; i++) {
        modelcells[i].one_prob = 0.5;
        modelcells[i].total_occs = 0;
    }
}

void update_one_prob(ModelCell *modelcells, context_t context, bit_t bit) {
    ModelCell *p_cell = &(modelcells[context]);
    p_cell->one_prob =
        (p_cell->one_prob * p_cell->total_occs + bit) /
        (p_cell->total_occs + 1);
    p_cell->total_occs += 1;
}

void init_one_prob(ModelCell *modelcells, context_t context, bit_t bit) {
    ModelCell *p_cell = &(modelcells[context]);
    p_cell->total_occs = (count_t) (INIT_OCCS);
    p_cell->one_prob =
        (prob_t) ((bit == ONE) ? (INIT_OCCS - 1) : 1) / (prob_t) (INIT_OCCS);
}

void encode(
    ArithmeticCodingHolder *p_output, ModelCell *modelcells, context_t context,
    bit_t bit) {
    ModelCell *p_cell = &(modelcells[context]);
    arithmeticcoding__encode(p_output, p_cell->one_prob, bit);
    if (p_cell->total_occs == 0) {
        init_one_prob(modelcells, context, bit);
    } else {
        update_one_prob(modelcells, context, bit);
    }
}

bit_t decode(
    ArithmeticCodingHolder *p_output, ModelCell *modelcells,
    context_t context) {
    ModelCell *p_cell = &(modelcells[context]);
    bit_t bit = arithmeticcoding__decode(p_output, p_cell->one_prob);
    if (p_cell->total_occs == 0) {
        init_one_prob(modelcells, context, bit);
    } else {
        update_one_prob(modelcells, context, bit);
    }
    return bit;
}
