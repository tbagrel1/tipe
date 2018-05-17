#ifndef DEF_RLE_CODE_T_VECT
#define DEF_RLE_CODE_T_VECT

#include <stdint.h>
#include <inttypes.h>

typedef uint16_t rle_code_t;
#define PRIrle_code_t PRIu16

#define CVECTOR_T rle_code_t
#define CVECTOR_DEFAULT_VALUE 0

#include "cvector_interface.h"

#endif  // DEF_RLE_CODE_T_VECT
