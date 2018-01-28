#ifndef DEF_SAIS_H
#define DEF_SAIS_H

#include <stdio.h>
#include <stdlib.h>

typedef int8_t index_t;
typedef unsigned char char_t;
typedef char bool_t;
typedef index_t *array;
typedef index_t const *const_array;
typedef char_t *str;
typedef char_t const *const_str;

/*
 * T -> data
 * C -> counts
 * B -> buckets
 * n -> data_size
 * k -> alphabet_size
 * end -> mode_end
 * SA -> suffix_array
 * m -> range
 * D -> unknown
 */

/**
 * Computes the number of each char in the specified string.
 * @param data_size size of the input string in char_t unit
 * @param data input string
 * @param counts output array for counts
 * @param alphabet_size size of the alphabet
 */
void get_counts(
    index_t data_size, const_str data, array counts, index_t alphabet_size);

/**
 * Computes start or end of each bucket (switch with mode_end param).
 * @param alphabet_size size of the alphabet
 * @param counts number of each char in the input string
 * @param buckets output array for buckets start/end
 * @param mode_end 0 to compute buckets start, 1 to compute buckets end
 */
void get_buckets(
    index_t alphabet_size, const_array counts, array buckets, bool_t mode_end);

/**
 * Makes the first step of LMS sort.
 * @param data_size size of the input string in char_t unit
 * @param data input string
 * @param alphabet_size size of the alphabet
 * @param counts number of each char in the input string
 * @param buckets work array for buckets start/end
 * @param suffix_array input/output array
 */
void lms_sort_1(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array);

/**
 * Makes the first step of LMS post-processing operations.
 * @param data_size size of the input string in char_t unit
 * @param data input string
 * @param suffix_array input/output array
 * @param range range of the operations
 * @return name
 */
index_t lms_postproc_1(
    index_t data_size, const_str data, array suffix_array, index_t range);

/**
 * Makes the second step of LMS sort.
 * @param data_size size of the input string in char_t unit
 * @param data input string
 * @param alphabet_size size of the alphabet
 * @param counts number of each char in the input string
 * @param buckets work array for buckets start/end
 * @param suffix_array input/output array
 * @param summary_array work array
 */
void lms_sort_2(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array, array summary_array);

/**
 * Makes the second step of LMS post-processing operations.
 * @param data_size size of the input string in char_t unit
 * @param data input_string
 * @param suffix_array input/output array
 * @param range range of the operations
 * @return name
 */
index_t lms_postproc_2(
    index_t data_size, const_str data, array suffix_array, index_t range);

/**
 * Makes induce sort.
 * @param data_size size of the data in char_t unit
 * @param data input string
 * @param alphabet_size size of the alphabet
 * @param counts number of each char in the input string
 * @param buckets work array for buckets start/end
 * @param suffix_array input/output array
 */
void induce_sa(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array);

/**
 * Computes BWT.
 * @param data_size size of the data in char_t unit
 * @param data input string
 * @param alphabet_size size of the alphabet
 * @param counts number of each char in the input string
 * @param buckets work array for buckets start/end
 * @param suffix_array input/output array
 * @return offset
 */
index_t compute_bwt(
    index_t data_size, const_str data, index_t alphabet_size, array counts,
    array buckets, array suffix_array);

/**
 * Final SA-IS algorithm.
 * @param data_size size of the input_string in char_t unit
 * @param data input_string
 * @param alphabet_size size of the alphabet
 * @param suffix_array output array
 * @param fs fs
 * @param is_bwt 0 if it isn't BWT, 1 if it is
 * @return offset
 */
index_t sais(
    index_t data_size, const_str data, index_t alphabet_size,
    array suffix_array, index_t fs, bool_t is_bwt);

#endif //  DEF_SAIS_H
