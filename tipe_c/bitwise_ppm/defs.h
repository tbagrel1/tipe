#ifndef DEF_DEFS
#define DEF_DEFS

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <math.h>

#include "acode_t_vect.h"

#define MAX_ORDER 3

#define BYTE_SIZE 8
#define INIT_OCCS 4

typedef double prob_t;
#define PRIprob_t "lf"

typedef uint64_t count_t;
#define PRIcount_t PRIu64

typedef uint8_t bit_t;
#define PRIbit_t PRIu8

typedef uint32_t context_t;
#define PRIcontext_t PRIu32

#define MAX_ACODE ((acode_t) (-1))
#define SLOWER(x) \
    (ceil(x) - 1.0)
#define SGREATER(x) \
    (floor(x) + 1.0)
#define AVG(a, b) \
    ((acode_t) (((double) (a) + (double) (b)) / 2))

#define ESCAPE_CHAR ((bit_t) (-1))
#define ZERO ((bit_t) (0))
#define ONE ((bit_t) (1))

#define BIT_STOP ((bit_t) (2))
#define BIT_ERROR ((bit_t) (3))

typedef struct ModelCell {
    prob_t one_prob;
    count_t total_occs;
} ModelCell;

typedef struct ModelOrder {
    size_t order;
    prob_t escape_prob;
    count_t escape_occs;
    count_t total_occs;
    ModelCell *model_cells;
} ModelOrder;

typedef struct ModelHolder {
    ModelOrder model_orders[MAX_ORDER + 1];
} ModelHolder;

typedef struct ArithmeticCodingHolder {
    acode_t_vect *codes;
    acode_t current_min;
    acode_t current_max;
    size_t current_pos;
} ArithmeticCodingHolder;

#endif  // DEF_DEFS
