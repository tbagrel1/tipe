#include <stdlib.h>
#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned char* str;

typedef __uint16_t index_t;  // Blocks up to 65536 o
const size_t index_size = sizeof(index_t);

str current_str = NULL;
size_t current_size = 0;

static inline uchar get(index_t offset, index_t i) {
    return current_str[(offset + i) % current_size];
}

int comp_lex(const void *raw_offset_a, const void *raw_offset_b) {
    index_t os_a = *(index_t *)(raw_offset_a);
    index_t os_b = *(index_t *)(raw_offset_b);
    index_t i = 0;
    while (i < current_size && get(os_a, i) == get(os_b, i)) {
        i++;
    }
    if (i == current_size) {
        return 0
    }
    return get(os_a, i) < get(os_b, i) ? -1 : 1;
}

/*
 * Result need to be freed after use.
 */
str bwt_code_block(str in_str, size_t in_size) {
    current_str = in_str;
    current_size = in_size;

    index_t *rot_tab = malloc(current_size * sizeof(*rot_tab));
    for (index_t i = 0; i < current_size; i++) {
        rot_tab[i] = i;
    }
    qsort(rot_tab, current_size, sizeof(*rot_tab), comp_lex);
    str out_str = malloc((index_size + current_size) * sizeof(*out_str));
    index_t index;
    for (index_t i = 0; i < current_size; i++) {
        out_str[index_size + i] = get(rot_tab[i], current_size - 1);
        if (rot_tab[i] == 0) {
            index = i;
        }
    }
    *(index_t *)(out_str) = index;
    return out_str;
}


int comp_ichar(const void *raw_ichar_a, const void *raw_ichar_b) {
    uchar *ichar_a = (uchar *)(raw_ichar_a);
    uchar *ichar_b = (uchar *)(raw_ichar_b);
    uchar char_a = get_char(ichar_a);
    uchar char_b = get_char(ichar_b);
    return char_a < char_b ?
           -1 : (char_a == char_b ? 0 : 1);
}

static inline uchar get_char(const uchar *ichar) {
    return *(ichar + index_size);
}

static inline index_t get_i(uchar const *ichar) {
    return *(index_t *)(ichar);
}

str bwt_decode_block(str in_coded, size_t in_size) {
    index_t index = *(index_t *)(in_coded);
    str coded_str = in_coded + index_size;
    size_t ichar_size = index_size + sizeof(uchar);
    uchar *ichar_concat = malloc(in_size * ichar_size);
    for (index_t i = 0; i < in_size; i++) {
        *(index_t *)(ichar_concat + i * ichar_size) = i;
        *(ichar_concat + i * ichar_size + index_size) = coded_str[i];
    }
    qsort(ichar_concat, in_size, ichar_size, comp_ichar);
    str out_str = malloc(in_size * sizeof(*out_str));
    for (size_t k = 0; k < in_size; k++) {
        out_str[k] = get_char(ichar_concat + index * ichar_size);
        index = get_i(ichar_concat + index * ichar_size);
    }
    return out_str;
}
