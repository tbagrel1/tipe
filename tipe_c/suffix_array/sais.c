#include "sais.h"

#define START 0
#define END 1

void get_counts(
    index_t data_size, const_str data, array counts, index_t alphabet_size) {
    for (
        index_t i = 0; i < alphabet_size; i++) {
        counts[i] = 0;
    }
    for (
        index_t i = 0; i < data_size; i++) {
        counts[data[i]]++;
    }
}

void get_buckets(
    index_t alphabet_size, const_array counts, array buckets,
    bool_t mode_end) {
    index_t sum = 0;
    if (mode_end) {
        for (
            index_t i = 0; i < alphabet_size; i++) {
            sum += counts[i];
            buckets[i] = sum;
        }
    } else {
        for (
            index_t i = 0; i < alphabet_size; i++) {
            buckets[i] = sum;
            sum += counts[i];
        }
    }
}

void lms_sort_1(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array) {
    index_t *b;
    index_t j;
    char_t c_0, c_1;
    // Compute SAl
    if (counts == buckets) {
        get_counts(data_size, data, counts, alphabet_size);
    }
    get_buckets(alphabet_size, counts, buckets, START);
    j = data_size - 1;
    c_1 = data[j];
    b = suffix_array + buckets[c_1];
    j--;
    *b = (data[j] < c_1) ? ~j : j;
    b++;
    for (
        index_t i = 0; i < data_size; i++) {
        j = suffix_array[i];
        if (j > 0) {
            c_0 = data[j];
            if (j > 0 && c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + buckets[c_1];
            }
            j--;
            *b = (data[j] < c_1) ? ~j : j;
            b++;
            suffix_array[i] = 0;
        } else if (j < 0) {
            suffix_array[i] = ~j;
        }
    }

    // Computes SAs
    if (counts == buckets) {
        get_counts(data_size, data, counts, alphabet_size);
    }
    get_buckets(alphabet_size, counts, buckets, END);
    c_1 = 0;
    b = suffix_array + buckets[c_1];
    for (
        index_t i = data_size - 1; i >= 0; i--) {
        j = suffix_array[i];
        if (j > 0) {
            c_0 = data[j];
            if (c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + buckets[c_1];
            }
            j--;
            b--;
            *b = data[j] > c_1 ? ~(j + 1) : j;
            suffix_array[i] = 0;
        }
    }
}

index_t lms_postproc_1(
    index_t data_size, const_str data, array suffix_array, index_t range) {
    return 0;
}

void lms_sort_2(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array, array summary_array) {

}

index_t lms_postproc_2(
    index_t data_size, const_str data, array suffix_array, index_t range) {
    return 0;

}

void induce_sa(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array) {

}

index_t compute_bwt(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array) {
    return 0;

}

index_t sais(
    index_t data_size, const_str data, index_t alphabet_size,
    array suffix_array, index_t fs, bool_t is_bwt) {
    return 0;
}
