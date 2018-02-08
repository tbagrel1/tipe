#include "sais.h"

#define START 0
#define END 1

void get_counts(
    index_t data_size, const_str data, index_t alphabet_size, array counts) {
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
        get_counts(data_size, data, alphabet_size, counts);
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
        get_counts(data_size, data, alphabet_size, counts);
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
    index_t i = 0;
    index_t j = 0;
    index_t p, q, p_length, q_length, name;
    bool_t diff;
    char_t c = 0;

    // Compact all the sorted substrings into the first range items of the
    // suffix_array

    while (suffix_array[i] < 0) {
        suffix_array[i] = ~suffix_array[i];
        i++;
    }
    if (i < range) {
        j = i + 1;
        while (j != range) {
            i++;
            if (suffix_array[i] < 0) {
                suffix_array[j] = ~suffix_array[j];
                j++;
                suffix_array[i] = 0;
            }
        }
    }

    // Store the length of all substrings
    i = data_size - 1;
    j = data_size - 1;
    do {
        c = data[i];
        i--;
    } while (i >= 0 && data[i] >= c);
    while (i >= 0) {
        do {
            c = data[i];
            i--;
        } while (i >= 0 && data[i] <= c);
        if (i >= 0) {
            suffix_array[range + ((i + 1) >> 1)] = j - i;
            j = i + 1;
            do {
                c = data[i];
                i--;
            } while (i >= 0 && data[i] >= c);
        }
    }

    // Find the lexicographic names of all substrings
    name = 0;
    q = data_size;
    q_length = 0;
    for (
        i = 0; i < range; i++) {
        p = suffix_array[i];
        p_length = suffix_array[range + (p >> 1)];
        diff = TRUE;
        if (p_length == q_length && (q + p_length) < data_size) {
            j = 0;
            while (j < p_length && data[p + j] == data[q + j]) {
                j++;
            }
            if (j == p_length) {
                diff = FALSE;
            }
        }
        if (diff == TRUE) {
            name++;
            q = p;
            q_length = p_length;
        }
        suffix_array[range + (p >> 1)] = name;
    }

    return name;
}

void lms_sort_2(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array, array summary_array) {
    index_t i, j;
    index_t t, d;
    index_t *b;
    char_t c_0, c_1;

    // Compute SAl
    get_buckets(alphabet_size, counts, buckets, START);
    j = data_size - 1;
    c_1 = data[j];
    b = suffix_array + buckets[c_1];
    j--;
    t = (data[j] < c_1);
    j += data_size;
    *b = t ? ~j : j;
    b++;
    d = 0;
    for (
        i = 0; i < data_size; i++) {
        j = suffix_array[i];
        if (j > 0) {
            if (j >= data_size) {
                d++;
                j -= data_size;
            }
            c_0 = data[j];
            if (c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + b[c_1];
            }
            j--;
            t = c_0;
            t = (t << 1) | (data[j] < c_1);
            if (summary_array[t] != d) {
                j += data_size;
                summary_array[t] = d;
            }
            *b = t ? ~j : j;
            b++;
            suffix_array[i] = 0;
        } else if (j < 0) {
            suffix_array[i] = ~j;
        }
    }
    i = data_size - 1;
    while (i >= 0) {
        if (suffix_array[i] > 0 && suffix_array[i] < data_size) {
            suffix_array[i] += data_size;
            j = i - 1;
            while (suffix_array[j] < data_size) {
                j--;
            }
            suffix_array[j] -= data_size;
            i = j;
        }
        i--;
    }

    // Compute SAs
    get_buckets(alphabet_size, counts, buckets, END);
    d++;
    c_1 = 0;
    b = suffix_array + buckets[c_1];
    for (
        i = data_size - 1; i >= 0; i--) {
        j = suffix_array[i];
        if (j > 0) {
            if (j >= data_size) {
                d++;
                j -= data_size;
            }
            c_0 = data[j];
            if (c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + buckets[c_1];
            }
            j--;
            t = c_0;
            t = (t << 1) | (data[j] > c_1);
            if (summary_array[t] != d) {
                j += data_size;
                summary_array[t] = d;
            }
            b--;
            *b = t ? ~(j + 1) : j;
            suffix_array[i] = 0;
        }
    }
}

index_t lms_postproc_2(
    index_t data_size, const_str data, array suffix_array, index_t range) {
    index_t i, j;
    index_t d;
    index_t name;

    // Compact all the sorted LMS substrings into the first range items of suffix_array
    name = 0;
    i = 0;
    j = suffix_array[i];
    while (j < 0) {
        j = ~j;
        if (j >= data_size) {
            name++;
        }
        suffix_array[i] = j;
        i++;
        j = suffix_array[i];
    }
    if (i < range) {
        d = i;
        bool_t done = FALSE;
        while (!done) {
            i++;
            j = suffix_array[i];
            if (j < 0) {
                j = ~j;
                if (j >= data_size) {
                    name++;
                }
                suffix_array[d] = j;
                d++;
                suffix_array[i] = 0;
                if (d == range) {
                    done = TRUE;
                }
            }
        }
    }

    if (name < range) {
        // Store the lexicographic names
        d = name + 1;
        for (
            i = range - 1; i >= 0; i--) {
            j = suffix_array[i];
            if (j >= data_size) {
                j -= data_size;
                d--;
            }
            suffix_array[range + (j >> 1)] = d;
        }
    } else {
        // Unset flags
        for (
            i = 0; i < range; i++) {
            j = suffix_array[i];
            if (j >= data_size) {
                j -= data_size;
                suffix_array[i] = j;
            }
        }
    }

    return name;
}

void induce_sa(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array) {
    index_t i, j;
    char_t c_0, c_1;
    index_t *b;

    if (counts == buckets) {
        get_counts(data_size, data, alphabet_size, counts);
    }
    // Find starts of buckets
    get_buckets(alphabet_size, counts, buckets, START);
    j = data_size - 1;
    c_1 = data[j];
    b = suffix_array + buckets[c_1];
    *b = ((j > 0) && (data[j - 1] < c_1)) ? ~j : j;
    b++;
    for (
        i = 0; i < data_size; i++) {
        j = suffix_array[i];
        suffix_array[i] = ~j;
        if (j > 0) {
            j--;
            c_0 = data[j];
            if (c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + buckets[c_1];
            }
            *b = ((j > 0) && (data[j - 1] < c_1)) ? ~j : j;
            b++;
        }
    }

    // Compute SAs
    if (counts != buckets) {
        get_counts(data_size, data, alphabet_size, counts);
    }
    // Find ends of buckets
    get_buckets(alphabet_size, counts, buckets, END);
    c_1 = 0;
    b = suffix_array + buckets[c_1];
    for (
        i = data_size - 1; i >= 0; i--) {
        j = suffix_array[i];
        if (j > 0) {
            j--;
            c_0 = data[j];
            if (c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + buckets[c_1];
            }
            b--;
            *b = ((j == 0) || (data[j - 1] > c_1)) ? ~j : j;
        } else {
            suffix_array[i] = ~j;
        }
    }
}

index_t compute_bwt(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array) {
    index_t i, j;
    index_t *b;
    char_t c_0, c_1;
    index_t offset = -1;

    // Compute SAl
    if (counts == buckets) {
        get_counts(data_size, data, alphabet_size, counts);
    }
    // Find starts of buckets
    get_buckets(alphabet_size, counts, buckets, START);
    j = data_size - 1;
    c_1 = data[j];
    b = suffix_array + buckets[c_1];
    *b = ((j > 0) && (data[j - 1] < c_1)) ? ~j : j;
    b++;
    for (
        i = 0; i < data_size; i++) {
        j = suffix_array[i];
        if (j > 0) {
            j--;
            c_0 = data[j];
            suffix_array[i] = ~((index_t) (c_0));
            if (c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + buckets[c_1];
            }
            *b = ((j > 0) && (data[j - 1] < c_1)) ? ~j : j;
            b++;
        } else if (j != 0) {
            suffix_array[i] = ~j;
        }
    }
    // Compute SAs
    if (counts == buckets) {
        get_counts(data_size, data, alphabet_size, counts);
    }
    // Find ends of buckets
    get_buckets(alphabet_size, counts, buckets, END);
    c_1 = 0;
    b = suffix_array + buckets[c_1];
    for (
        i = data_size - 1; i >= 0; i--) {
        j = suffix_array[i];
        if (j > 0) {
            c_0 = data[j];
            suffix_array[i] = c_0;
            if (c_0 != c_1) {
                buckets[c_1] = b - suffix_array;
                c_1 = c_0;
                b = suffix_array + b[c_1];
            }
            --b;
            *b =
                ((j > 0) && (data[j - 1] > c_1))
                ? ~((index_t) (data[j - 1]))
                : j;
        } else if (j != 0) {
            suffix_array[i] = ~j;
        } else {
            offset = i;
        }
    }

    return offset;
}

index_t sais(
    index_t data_size, const_str data, index_t alphabet_size,
    array suffix_array, index_t fs, bool_t is_bwt) {
    index_t i, j;
    index_t range, p, q, t;
    index_t name, new_fs;
    index_t offset = 0;
    index_t *b;
    index_t *counts, *buckets, *summary_array;
    char_t c_0, c_1;
    flag_t flags;

    counts = (index_t *) (malloc(alphabet_size * sizeof(*counts)));
    if (alphabet_size <= fs) {
        buckets = suffix_array + (data_size + fs - alphabet_size);
        flags = 1;
    } else {
        buckets = (index_t *) (malloc(alphabet_size * sizeof(*counts)));
        flags = 3;
    }

    if ((data_size / alphabet_size) >= 2) {
        flags |= ((alphabet_size * 2) <= (fs - alphabet_size)) ? 32 : 16;
    }

    // Stage 1: reduce the problem by at least 1 / 2
    // Sort all the LMS-substrings
    get_counts(data_size, data, alphabet_size, counts);
    // Find ends of buckets
    get_buckets(alphabet_size, counts, buckets, END);
    for (i = 0; i < data_size; i++) {
        suffix_array[i] = 0;
    }
    i = data_size - 2;
    j = data_size;
    range = 0;
    while (i >= 0 && data[i + 1] <= data[i]) {
        i--;
    }
    while (i >= 0) {
        i--;
        while (i >= 0 && data[i + 1] >= data[i]) {
            i--;
        }
        c_1 = data[i + 1];
        if (i >= 0) {
            *b = j;
            b = suffix_array + buckets[c_1];
            buckets[c_1]--;
            j = i;
            range++;
            i--;
            while (i >= 0 && data[i + 1] <= data[i]) {
                i--;
            }
        }
    }

    if (range > 1) {
        if (flags & (16 | 32))
    }
}
