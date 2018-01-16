#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef unsigned char uchar;

#define BLOCK_SIZE 256
#define INDEX_SIZE sizeof(index_t)
#define PADDING_CHAR (uchar)(2)

typedef uchar index_t;

typedef uchar * str;

str current_str = 0;

static inline uchar get(size_t offset, size_t i) {
    return current_str[(offset + i) % BLOCK_SIZE];
}

int comp_lex(const void *raw_offset_a, const void *raw_offset_b) {
    size_t os_a = *(size_t *)(raw_offset_a);
    size_t os_b = *(size_t *)(raw_offset_b);
    size_t i = 0;
    while (get(os_a, i) != '\0' && get(os_b, i) != '\0' &&
           get(os_a, i) == get(os_b, i)) {
        i += 1;
    }
    if (get(os_a, i) == '\0') {
        return get(os_b, i) == '\0' ? 0 : -1;
    }
    if (get(os_b, i) == '\0') {
        return 1;
    }
    return get(os_a, i) < get(os_b, i) ?
           -1 : (get(os_a, i) == get(os_b, i) ? 0 : 1);
}


void bwt_code_block(str in_str, str out_str) {
    // BEGIN Global
    current_str = in_str;
    // END Global
    size_t *rot_tab = malloc(BLOCK_SIZE * sizeof(*rot_tab));
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        rot_tab[i] = i;
    }
    qsort(rot_tab, BLOCK_SIZE, sizeof(*rot_tab), comp_lex);
    index_t index;
    for (size_t i = 0; i < BLOCK_SIZE; i++) {
        out_str[INDEX_SIZE + i] = get(rot_tab[i], BLOCK_SIZE - 1);
        if (rot_tab[i] == 0) {
            index = i;  // Known risks
        }
    }
    *(index_t *)(out_str) = index;
}

/*
str bwt_code(str in_str) {
    size_t size = strlen((const char *)in_str);
    size_t nb_block = (size_t)(ceil(size / BLOCK_SIZE));
    size_t last_block_size = size - (nb_block - 1) * BLOCK_SIZE;
    str last_block = malloc((BLOCK_SIZE * sizeof(*last_block)) + 1);
    last_block[BLOCK_SIZE] = '\0';
    memcpy(last_block, in_str + (nb_block - 1) * BLOCK_SIZE,
           last_block_size * sizeof(*in_str));
    for (size_t i = last_block_size; i < BLOCK_SIZE; i++) {
        last_block[i] = PADDING_CHAR;
    }
    size_t result_str_size = nb_block * (INDEX_SIZE + BLOCK_SIZE);
    str result_str = malloc((result_str_size * sizeof(*result_str)) + 1);
    result_str[result_str_size] = '\0';
    for (size_t i = 0; i < nb_block - 1; i++) {
        bwt_code_block(in_str + i * BLOCK_SIZE,
                       result_str + i * (INDEX_SIZE + BLOCK_SIZE));
    }
    bwt_code_block(
        last_block,
        (result_str + (nb_block - 1) * (INDEX_SIZE + BLOCK_SIZE)));
}
*/

static inline uchar get_char(const uchar *ichar) {
    return *(ichar + INDEX_SIZE);
}

static inline index_t get_i(uchar const *ichar) {
    return *(index_t *)(ichar);
}

int comp_ichar(const void *raw_ichar_a, const void *raw_ichar_b) {
    uchar *ichar_a = (uchar *)(raw_ichar_a);
    uchar *ichar_b = (uchar *)(raw_ichar_b);
    uchar char_a = get_char(ichar_a);
    uchar char_b = get_char(ichar_b);
    return char_a < char_b ?
           -1 : (char_a == char_b ? 0 : 1);
}

void bwt_decode_block(str in_coded, str out_str) {
    index_t index = *(index_t *)(in_coded);
    str coded_str = in_coded + INDEX_SIZE;
    size_t ichar_size = INDEX_SIZE + sizeof(uchar);
    uchar *ichar_concat = malloc(BLOCK_SIZE * ichar_size);
    for (index_t i = 0; i < BLOCK_SIZE; i++) {
        *(index_t *)(ichar_concat + i * ichar_size) = i;
        *(ichar_concat + i * ichar_size + INDEX_SIZE) = coded_str[i];
    }
    qsort(ichar_concat, BLOCK_SIZE, ichar_size, comp_ichar);
    for (size_t k = 0; k < BLOCK_SIZE; k++) {
        out_str[k] = get_char(ichar_concat + index * ichar_size);
        index = get_i(ichar_concat + index * ichar_size);
    }
}

str bwt_decode(str in_coded) {
    size_t size_coded = strlen((const char *)(in_coded));
    size_t nb_block = size_coded / (INDEX_SIZE + BLOCK_SIZE);
    str result_str = malloc(nb_block * BLOCK_SIZE * sizeof(*result_str));
    for (size_t i = 0; i < nb_block; i++) {
        bwt_decode_block(in_coded + i * (INDEX_SIZE + BLOCK_SIZE),
                         result_str + i * BLOCK_SIZE);
    }
    size_t i = nb_block * BLOCK_SIZE;
    while (result_str[i - 1] == PADDING_CHAR) {
        i--;
    }
    result_str[i] = '\0';
}
