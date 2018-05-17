#include "bitwise_ppm.h"
#include "arithmetic_coder.h"

/**
 * ModelHolder constructor. Returns an empty, non-initialized model holder
 * object.
 * @return an empty, non-initialized model holder struct
 */
ModelHolder *modelholder__new() {
    return (ModelHolder *) (malloc(sizeof(ModelHolder)));
}

/**
 * Initializes the struct ModelHolder by creating cells and by settings params
 * @param p_model_holder a pointer to the struct to be initialized
 * @param escape_probs a prob_t array[MAX_ORDER + 1] which defines probs for
 * escape character (the one emitted when the order goes down)
 */
void modelholder__init(
    ModelHolder *p_model_holder, prob_t *escape_probs) {
    ModelCell *p_cell;
    for (size_t order = 0; order <= MAX_ORDER; order++) {
        p_model_holder->model_orders[order].order = order;
        p_model_holder->model_orders[order].escape_prob = escape_probs[order];
        p_model_holder->model_orders[order].escape_occs = 0;
        p_model_holder->model_orders[order].total_occs = 0;
        size_t nb_cells = 1UL << (BYTE_SIZE * order);
        p_model_holder->model_orders[order].model_cells =
            malloc(nb_cells * sizeof(ModelCell));
        for (size_t i_cell = 0; i_cell < nb_cells; i_cell++) {
            p_cell =
                &(p_model_holder->model_orders[order].model_cells[i_cell]);
            p_cell->one_prob = 0.5;
            p_cell->total_occs = 0;
        }
    }
    // Reset for order 0 = fallback
    p_model_holder->model_orders[0].order = 0;
    p_model_holder->model_orders[0].escape_prob = 0.0;
    p_model_holder->model_orders[0].escape_occs = 0;
    p_model_holder->model_orders[0].total_occs = 2;
    p_cell = &(p_model_holder->model_orders[0].model_cells[0]);
    p_cell->one_prob = 0.5;
    p_cell->total_occs = 2;
}

/**
 * Frees the struct ModelHolder by freeing arrays of cells.
 * @param p_model_holder a pointer to the struct to be freed
 */
void modelholder__free(ModelHolder *p_model_holder) {
    for (size_t order = 0; order <= MAX_ORDER; order++) {
        free(p_model_holder->model_orders[order].model_cells);
    }
    free(p_model_holder);
}

/**
 * Updates the escape prob of the model order of the specified order. If a new
 * escape char was emitted, increases escape prob. Otherwise, decreases it.
 * @param p_model_holder a pointer to the struct to work on
 * @param order the order where an escape char was emitted or not
 * @param bit if an escape char was emitted, bit equals -1
 */
void update_escape_prob(
    ModelHolder *p_model_holder, size_t order, bit_t bit) {
    // TODO: find an implementation which alters escape prob decently
    // At the moment, escape probs are constant
}

/**
 * Updates the one prob of the model order of the specified order for the
 * specified context.
 * BE CAREFUL, at this stage, bit must NOT be an escape char (escape chars must
 * have been handled BEFORE).
 * @param p_model_holder a pointer to the struct to work on
 * @param order the order where the update will be made
 * @param s_context the context for which the update will be made (sanitized)
 * @param bit indicates which bit was emitted, must be 0 or 1
 */
void update_one_prob(
    ModelHolder *p_model_holder, size_t order, context_t s_context,
    bit_t bit) {
    ModelCell
        *p_cell =
        &(p_model_holder->model_orders[order].model_cells[s_context]);
    p_cell->one_prob =
        (p_cell->one_prob * p_cell->total_occs + bit) /
        (p_cell->total_occs + 1);
    p_cell->total_occs += 1;
}

/**
 * Encodes the specified bit and modify the specified ouput struct.
 * @param p_output a pointer to the output struct
 * @param p_model_holder a pointer to the model holder struct
 * @param context previous context of the specified bit
 * @param bit bit to encode
 */
void encode(
    ArithmeticCodingHolder *p_output, ModelHolder *p_model_holder,
    context_t context, bit_t bit) {
    size_t order = MAX_ORDER;
    context = context & ((1U << (BYTE_SIZE * order)) - 1);
    ModelCell
        *p_cell = &(p_model_holder->model_orders[order].model_cells[context]);
    while (p_cell->total_occs == 0) {
        // Decrease orders
        arithmeticcoding__encode(
            p_output, p_cell->one_prob,
            p_model_holder->model_orders[order].escape_prob, ESCAPE_CHAR);
        update_escape_prob(p_model_holder, order, ESCAPE_CHAR);
        init_one_prob(p_model_holder, order, context, bit);
        order--;
        context = context & ((1U << (BYTE_SIZE * order)) - 1);
        p_cell = &(p_model_holder->model_orders[order].model_cells[context]);
    }
    // Correct order found
    arithmeticcoding__encode(
        p_output, p_cell->one_prob,
        p_model_holder->model_orders[order].escape_prob, bit);
    update_escape_prob(p_model_holder, order, bit);
    update_one_prob(p_model_holder, order, context, bit);
}

/**
 * Decodes the next real bit (ie not an escape char) from the output struct.
 * @param p_output a pointer to the output struct
 * @param p_model_holder a pointer to the model holder struct
 * @param context previous context of the bit to decode
 * @return the bit read
 */
bit_t decode(
    ArithmeticCodingHolder *p_output, ModelHolder *p_model_holder,
    context_t context) {
    size_t order = MAX_ORDER;
    context = context & ((1U << (BYTE_SIZE * order)) - 1);
    ModelCell
        *p_cell = &(p_model_holder->model_orders[order].model_cells[context]);
    bit_t bit;
    context_t contexts[MAX_ORDER + 1];
    while ((
               bit = arithmeticcoding__decode(
                   p_output, p_cell->one_prob,
                   p_model_holder->model_orders[order].escape_prob), bit) ==
           ESCAPE_CHAR) {
        update_escape_prob(p_model_holder, order, ESCAPE_CHAR);
        contexts[order] = context;
        order--;
        context = context & ((1U << (BYTE_SIZE * order)) - 1);
        p_cell = &(p_model_holder->model_orders[order].model_cells[context]);
    }
    for (size_t i_order = MAX_ORDER; i_order > order; i_order--) {
        init_one_prob(p_model_holder, i_order, contexts[i_order], bit);
    }
    update_escape_prob(p_model_holder, order, bit);
    update_one_prob(p_model_holder, order, context, bit);
    return bit;
}

/**
 * Init the specified model cell when the context is encountered for the first
 * time.
 * @param p_model_holder the struct to work on
 * @param order order of the cell
 * @param s_context context of the bit (sanitized)
 * @param bit bit encountered
 */
void init_one_prob(
    ModelHolder *p_model_holder, size_t order, context_t s_context,
    bit_t bit) {
    ModelCell
        *p_cell =
        &(p_model_holder->model_orders[order].model_cells[s_context]);
    p_cell->total_occs = (count_t) (INIT_OCCS);
    p_cell->one_prob =
        (prob_t) ((bit == ONE) ? (INIT_OCCS - 1) : 1) / (prob_t) (INIT_OCCS);
}
