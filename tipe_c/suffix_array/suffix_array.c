#include <stdio.h>
#include <stdlib.h>

#include "suffix_array.h"

#define S_TYPE 'S'
#define L_TYPE 'L'

suffix_t_array build_type_map(str data, size_t data_size) {
    suffix_t_array res =
        (suffix_t_array)(malloc((data_size + 1) * sizeof(*res)));
    res[data_size] = S_TYPE;
    if (data_size == 0) {
        return res;
    }
    res[data_size - 1] = L_TYPE;
    for (int i = data_size - 1; i >= 0; i--) {
        if (data[i] > data[i + 1]) {
            res[i] = L_TYPE;
        } else if (data[i] == data[i + 1] && res[i + 1] == L_TYPE) {
            res[i] = L_TYPE;
        } else {
            res[i] = S_TYPE;
        }
    }

    return res;
}

int is_lms_char(suffix_t_array typemap, size_t offset) {
    if (offset == 0) {
        return 0;
    }
    if (typemap[offset] == S_TYPE &&
        typemap[offset - 1] == L_TYPE) {
        return 1;
    }
    return 0;
}

void show_typemap(str data, size_t data_size) {
    suffix_t_array typemap = build_type_map(data, data_size);
    printf("%s\n", typemap);
    for (size_t i = 0; i < data_size + 1; i++) {
        if (is_lms_char(typemap, i)) {
            printf("^");
        } else {
            printf(" ");
        }
    }
    printf("\n");
}

int lms_substring_equal(str data, size_t data_size, suffix_t_array typemap,
                        size_t offset_a, size_t offset_b) {
    if (offset_a == data_size or offset_b == data_size) {
        return 0;
    }

    size_t i = 0;
    int a_is_lms, b_is_lms;
    while (1) {
        a_is_lms = is_lms_char(typemap, i + offset_a);
        b_is_lms = is_lms_char(typemap, i + offset_b);
        if (i > 0 && a_is_lms && b_is_lms) {
            return 1;
        }
        if (a_is_lms != b_is_lms) {
            return 0;
        }
        if (data[i + offset_a] != data[i + offset_b]) {
            return 0;
        }
        i++;
    }
}

size_t *find_bucket_sizes(str data, size_t data_size) {
    size_t *bucket_sizes = (size_t *)(malloc(256 * sizeof(*bucket_sizes)));
    for (size_t i = 0; i < data_size; i++) {
        bucket_sizes[data[i]] += 1;
    }
    return bucket_sizes;
}

size_t *find_bucket_heads(size_t *bucket_sizes) {
    size_t offset = 1;
    size_t *bucket_heads = (size_t *)(malloc(256 * sizeof(*bucket_heads)));
    for (size_t i = 0; i < 256; i++) {
        bucket_heads[i] = offset;
        offset += bucket_sizes[i];
    }
    return bucket_heads;
}

size_t *find_bucket_tails(size_t *bucket_sizes) {
    size_t offset = 1;
    size_t *bucket_tails = (size_t *)(malloc(256 * sizeof(*bucket_tails)));
    for (size_t i = 0; i < 256; i++) {
        offset += bucket_sizes[i];
        bucket_tails[i] = offset;
    }
    return bucket_tails;
}

guess_lms_sort(str data, size_t data_size; suffix_t_array typemap,
               size_t *bucket_sizes) {
    size_t *guessed_suffix_array =
        (size_t *)(malloc((data_size + 1) * sizeof(*guessed_suffix_array)));
    for (size_t i = 0; i < data_size + 1; i++) {
        guessed_suffix_array[i] = -1;
    }

    size_t *bucket_tails = find_bucket_tails(bucket_sizes);
    for 

    free(bucket_tails);
