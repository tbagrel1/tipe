#include "arithmetic_coder.h"

/**
 * ArithmeticCodingHolder constructor. Returns a new instance of the struct.
 * @return a new instance of ArithmeticCodingHolder
 */
ArithmeticCodingHolder *arithmeticcoding__new() {
    ArithmeticCodingHolder *p_output = (ArithmeticCodingHolder *) (
        malloc(sizeof(*p_output)));
    p_output->codes = acode_t_vect__new();
    p_output->current_pos = 0;
    arithmeticcoding__reset_min_max(p_output);
    return p_output;
}

/**
 * ArithmeticCodingHolder destructor. Frees the struct.
 * @param p_output the struct to free
 */
void arithmeticcoding__free(ArithmeticCodingHolder *p_output) {
    // Because we want to get the vect
    // acode_t_vect__free(p_output->codes);
    free(p_output);
}

/**
 * Encodes the specified bit (or escape char) according to the specified probs.
 * @param p_output a pointer to the output struct
 * @param one_prob prob of getting a 1
 * @param escape_prob prob of getting an escape char
 * @param bit the bit (or escape char) to encode
 */
void arithmeticcoding__encode(
    ArithmeticCodingHolder *p_output, prob_t one_prob, prob_t escape_prob,
    bit_t bit) {
    acode_t prev_max = p_output->current_max;
    acode_t prev_min = p_output->current_min;
    prob_t alpha = prev_min + escape_prob * (prev_max - prev_min);
    prob_t beta = alpha + (1 - one_prob) * (prev_max - alpha);
    prob_t new_max, new_min;
    if (bit == ESCAPE_CHAR) {
        new_min = SGREATER(prev_min);
        new_max = SLOWER(alpha);
    } else if (bit == ZERO) {
        new_min = SGREATER(alpha);
        new_max = SLOWER(beta);
    } else if (bit == ONE) {
        new_min = SGREATER(beta);
        new_max = SLOWER(prev_max);
    }
    if (new_max - new_min < 0 ||
        new_max < 0 ||
        new_max > MAX_ACODE ||
        new_min < 0 ||
        new_min > MAX_ACODE) {
        acode_t_vect__add(
            p_output->codes, prev_max);
        p_output->current_pos++;
        arithmeticcoding__reset_min_max(p_output);
        arithmeticcoding__encode(p_output, one_prob, escape_prob, bit);
    } else {
        p_output->current_min = (acode_t) (new_min);
        p_output->current_max = (acode_t) (new_max);
    }
}

/**
 * Transforms the specified struct into a acode_t vect.
 * @param p_output the struct to consume
 * @return a acode_t vect corresponding to the output
 */
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

/**
 * Creates a holder struct from the speicifed acode_t vect.
 * @param p_result the acode_t vect to create the holder struct from
 * @return a holder struct
 */
ArithmeticCodingHolder *
arithmeticcoding__from_out_format(acode_t_vect *p_result) {
    ArithmeticCodingHolder *p_output = arithmeticcoding__new();
    p_output->current_pos = 0;
    p_output->codes = p_result;
    arithmeticcoding__reset_min_max(p_output);
}

/**
 * Decodes a bit (or escape char) from the specified output struct.
 * @param p_holder a pointer to the output struct
 * @param one_prob prob of getting a 1
 * @param escape_prob prob of getting an escape char
 * @return the bit (or escape char) read from the output struct
 */
bit_t arithmeticcoding__decode(
    ArithmeticCodingHolder *p_output, prob_t one_prob, prob_t escape_prob) {
    index_t size = p_output->codes->_size;
    if (p_output->current_pos == size - 1) {
        return BIT_STOP;
    }
    if (p_output->current_pos == size - 2) {
        return _arithmeticcoding__decode_until(
            p_output, one_prob, escape_prob, acode_t_vect__get(
                p_output->codes, size - 1));
    }
    return _arithmeticcoding__decode_until(p_output, one_prob, escape_prob, 0);
}

bit_t _arithmeticcoding__decode_until(
    ArithmeticCodingHolder *p_output, prob_t one_prob, prob_t escape_prob,
    prob_t delta_stop) {
    acode_t code = acode_t_vect__get(p_output->codes, p_output->current_pos);
    acode_t prev_min = p_output->current_min;
    acode_t prev_max = p_output->current_max;
    prob_t alpha = prev_min + (prev_max - prev_min) * escape_prob;
    prob_t beta = alpha + (prev_max - alpha) * (1 - one_prob);
    prob_t new_max, new_min;
    bit_t bit = BIT_ERROR;
    if (code >= prev_min && code < alpha) {
        bit = ESCAPE_CHAR;
        new_min = SGREATER(prev_min);
        new_max = SLOWER(alpha);
    } else if (code >= alpha && code < beta) {
        bit = ZERO;
        new_min = SGREATER(alpha);
        new_max = SLOWER(beta);
    } else if (code >= beta && code < prev_max) {
        bit = ONE;
        new_min = SGREATER(beta);
        new_max = SLOWER(prev_max);
    }
    if (code == prev_max || new_max - new_min < delta_stop) {
        // If it should stop
        p_output->current_pos++;
        arithmeticcoding__reset_min_max(p_output);
        return arithmeticcoding__decode(p_output, one_prob, escape_prob);
    } else {
        p_output->current_min = (acode_t) (new_min);
        p_output->current_max = (acode_t) (new_max);
        return bit;
    }
}

/**
 * Resets min and max bounds of the output struct.
 * @param p_output the struct to work on
 */
void arithmeticcoding__reset_min_max(ArithmeticCodingHolder *p_output) {
    p_output->current_min = 0;
    p_output->current_max = MAX_ACODE;
}
