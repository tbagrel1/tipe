#ifndef DEF_SUFFIX_ARRAY
#define DEF_SUFFIX_ARRAY

#include <stdlib.h>
#include <stdio.h>

#include "clist.h"

#define ALPHABET_SIZE 256

typedef unsigned char uchar;
typedef int *array;
typedef int const *array_const;
typedef uchar suffix_t;
typedef suffix_t *suffix_t_array;
typedef suffix_t const *suffix_t_array_const;

/* Need to be freed after use */
suffix_t_array build_type_map(array_const data, int data_size);

int is_lms_char(suffix_t_array_const typemap, int offset);

/*
void show_type_map(array data, int data_size);
*/

int lms_subdata_equal(
    array_const data, int data_size, suffix_t_array_const typemap,
    int offset_a, int offset_b);

/* Need to be freed after use */
array find_bucket_sizes(array_const data, int data_size, int alphabet_size);

/* Need to be freed after use */
array find_bucket_heads(array_const bucket_sizes);

/* Need to be freed after use */
array find_bucket_tails(array_const bucket_sizes);

/* Need to be freed after use */
array make_suffix_array_induced_sorting(
    array_const data, int data_size, int alphabet_size);

/* Need to be freed after use */
array guess_lms_sort(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const bucket_sizes);

void induce_sort_l(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const bucket_sizes, array guessed_suffix_array);

void induce_sort_s(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const bucket_sizes, array guessed_suffix_array);

void summarize_suffix_array(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const guessed_suffix_array, CList *p_summary_data,
    CList *p_summary_suffix_offsets, int *p_summary_alphabet_size);

/* Need to be freed after use */
array make_summary_suffix_array(
    CList *p_summary_data, int summary_alphabet_size);

/* Need to be freed after use */
array accurate_lms_sort(
    array_const data, int data_size, array_const bucket_sizes,
    array_const summary_suffix_array, CList *p_summary_data,
    CList *p_summary_suffix_offsets);

static inline array make_suffix_array(array data, int data_size) {
    return make_suffix_array_induced_sorting(data, data_size, ALPHABET_SIZE);
}

#endif  // DEF_SUFFIX_ARRAY
