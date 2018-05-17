#include "arithmetic_coder_flat.h"

ArithmeticCodingHolder *arithmeticcoding__new() {
    ArithmeticCodingHolder *p_output = (ArithmeticCodingHolder *) (
        malloc(sizeof(*p_output)));
    p_output->codes = acode_t_vect__new();
    p_output->current_pos = 0;
    arithmeticcoding__reset_min_max(p_output);
    return p_output;
}

void arithmeticcoding__free(ArithmeticCodingHolder *p_output) {
    free(p_output);
}

void arithmeticcoding__encode(
    ArithmeticCodingHolder *p_output, prob_t one_prob, bit_t bit) {
    acode_t prev_max = p_output->current_max;
    acode_t prev_min = p_output->current_min;
    prob_t new_max, new_min;
    prob_t center = prev_min + (1 - one_prob) * (prev_max - prev_min);
    if (bit == ZERO) {
        new_min = SGREATER(prev_min);
        new_max = SLOWER(center);
    } else if (bit == ONE) {
        new_min = SGREATER(center);
        new_max = SLOWER(prev_max);
    } else {
        printf("<Error>\n");
    }
    if (new_max - new_min < 0 ||
        new_max < 0 ||
        new_max > MAX_ACODE ||
        new_min < 0 ||
        new_min > MAX_ACODE) {
        acode_t_vect__add(p_output->codes, prev_max);
        p_output->current_pos++;
        arithmeticcoding__reset_min_max(p_output);
        arithmeticcoding__encode(p_output, one_prob, bit);
    } else {
        p_output->current_min = (acode_t) (new_min);
        p_output->current_max = (acode_t) (new_max);
    }
}

acode_t_vect *
arithmeticcoding__to_out_format(ArithmeticCodingHolder *p_output) {
    acode_t_vect__add(
        p_output->codes, AVG(p_output->current_min, p_output->current_max));
    acode_t_vect__add(
        p_output->codes, p_output->current_max - p_output->current_min);
    acode_t_vect *p_result = p_output->codes;
    arithmeticcoding__free(p_output);
    return p_result;
}

ArithmeticCodingHolder *
arithmeticcoding__from_out_format(acode_t_vect *p_result) {
    ArithmeticCodingHolder *p_output = arithmeticcoding__new();
    p_output->current_pos = 0;
    p_output->codes = p_result;
    arithmeticcoding__reset_min_max(p_output);
}

bit_t arithmeticcoding__decode(
    ArithmeticCodingHolder *p_output, prob_t one_prob) {
    index_t size = p_output->codes->_size;
    if (p_output->current_pos == size - 1) {
        return BIT_STOP;
    }
    if (p_output->current_pos == size - 2) {
        return _arithmeticcoding__decode_until(
            p_output, one_prob, acode_t_vect__get(p_output->codes, size - 1));
    }
    return _arithmeticcoding__decode_until(p_output, one_prob, 0);
}

void arithmeticcoding__reset_min_max(ArithmeticCodingHolder *p_output) {
    p_output->current_min = 0;
    p_output->current_max = MAX_ACODE;
}

bit_t _arithmeticcoding__decode_until(
    ArithmeticCodingHolder *p_output, prob_t one_prob, prob_t delta_stop) {
    acode_t code = acode_t_vect__get(p_output->codes, p_output->current_pos);
    acode_t prev_min = p_output->current_min;
    acode_t prev_max = p_output->current_max;
    prob_t center = prev_min + (1 - one_prob) * (prev_max - prev_min);
    prob_t new_max, new_min;
    bit_t bit = BIT_ERROR;
    if (code >= prev_min && code < center) {
        bit = ZERO;
        new_min = SGREATER(prev_min);
        new_max = SLOWER(center);
    } else if (code >= center && code < prev_max) {
        bit = ONE;
        new_min = SGREATER(center);
        new_max = SLOWER(prev_max);
    }
    if (code == prev_max || new_max - new_min < delta_stop) {
        // If it should stop
        p_output->current_pos++;
        arithmeticcoding__reset_min_max(p_output);
        return arithmeticcoding__decode(p_output, one_prob);
    } else {
        p_output->current_min = (acode_t) (new_min);
        p_output->current_max = (acode_t) (new_max);
        return bit;
    }
}
