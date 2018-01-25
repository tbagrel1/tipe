#include <stdio.h>
#include <stdlib.h>

#include "suffix_array.h"

#define S_TYPE 'S'
#define L_TYPE 'L'

suffix_t_array build_type_map(array_const data, int data_size) {
    suffix_t_array
        res = (suffix_t_array) (malloc((data_size + 1) * sizeof(*res)));
    res[data_size] = S_TYPE;
    if (data_size == 0) {
        return res;
    }
    res[data_size - 1] = L_TYPE;
    for (
        int i = data_size - 1; i >= 0; i--) {
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

int is_lms_char(suffix_t_array_const typemap, int offset) {
    if (offset == 0) {
        return 0;
    }
    if (typemap[offset] == S_TYPE && typemap[offset - 1] == L_TYPE) {
        return 1;
    }
    return 0;
}

/*
void show_type_map(array data, int data_size) {
    suffix_t_array typemap = build_type_map(data, data_size);
    printf("%s\n", typemap);
    for (int i = 0; i < data_size + 1; i++) {
        if (is_lms_char(typemap, i)) {
            printf("^");
        } else {
            printf(" ");
        }
    }
    printf("\n");
    free(typemap);
}
*/

int lms_subdata_equal(
    array_const data, int data_size, suffix_t_array_const typemap,
    int offset_a, int offset_b) {
    if (offset_a == data_size || offset_b == data_size) {
        return 0;
    }
    int i = 0;
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

array find_bucket_sizes(array_const data, int data_size, int alphabet_size) {
    array
        bucket_sizes = (array) (malloc(alphabet_size * sizeof(*bucket_sizes)));
    for (
        int i = 0; i < data_size; i++) {
        bucket_sizes[data[i]] += 1;
    }
    return bucket_sizes;
}

array find_bucket_heads(array_const bucket_sizes) {
    int offset = 1;
    array bucket_heads = (array) (malloc(256 * sizeof(*bucket_heads)));
    for (
        int i = 0; i < 256; i++) {
        bucket_heads[i] = offset;
        offset += bucket_sizes[i];
    }
    return bucket_heads;
}

array find_bucket_tails(array_const bucket_sizes) {
    int offset = 1;
    array bucket_tails = (array) (malloc(256 * sizeof(*bucket_tails)));
    for (
        int i = 0; i < 256; i++) {
        offset += bucket_sizes[i];
        bucket_tails[i] = offset;
    }
    return bucket_tails;
}

array make_suffix_array_induced_sorting(
    array_const data, int data_size, int alphabet_size) {
    suffix_t_array typemap = build_type_map(data, data_size);
    array bucket_sizes = find_bucket_sizes(data, data_size, alphabet_size);
    array guessed_suffix_array = guess_lms_sort(
        data, data_size, typemap, bucket_sizes);
    induce_sort_l(
        data, data_size, typemap, bucket_sizes, guessed_suffix_array);
    induce_sort_s(
        data, data_size, typemap, bucket_sizes, guessed_suffix_array);
    CList *p_summary_data = clist_new();
    CList *p_summary_suffix_offsets = clist_new();
    int summary_alphabet_size;
    summarize_suffix_array(
        data, data_size, typemap, guessed_suffix_array, p_summary_data,
        p_summary_suffix_offsets, &summary_alphabet_size);
    array summary_suffix_array = make_summary_suffix_array(
        p_summary_data, summary_alphabet_size);

    array result = accurate_lms_sort(
        data, data_size, bucket_sizes, summary_suffix_array, p_summary_data,
        p_summary_suffix_offsets);
    induce_sort_l(
        data, data_size, typemap, bucket_sizes, guessed_suffix_array);
    induce_sort_s(
        data, data_size, typemap, bucket_sizes, guessed_suffix_array);
    free(summary_suffix_array);
    clist_free(p_summary_data);
    clist_free(p_summary_suffix_offsets);
    free(guessed_suffix_array);
    free(bucket_sizes);
    free(typemap);
    return result;
}

array guess_lms_sort(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const bucket_sizes) {
    array
        guessed_suffix_array =
        (array) (malloc((data_size + 1) * sizeof(*guessed_suffix_array)));
    for (
        int i = 0; i < data_size + 1; i++) {
        guessed_suffix_array[i] = -1;
    }

    array bucket_tails = find_bucket_tails(bucket_sizes);
    int bucket_index;
    for (
        int i = 0; i < data_size; i++) {
        if (is_lms_char(typemap, i)) {
            bucket_index = data[i];
            guessed_suffix_array[bucket_tails[bucket_index]] = i;
            bucket_tails[bucket_index]--;
        }
    }
    guessed_suffix_array[0] = data_size;
    free(bucket_tails);
    return guessed_suffix_array;
}

void induce_sort_l(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const bucket_sizes, array guessed_suffix_array) {
    array bucket_heads = find_bucket_heads(bucket_sizes);
    int j;
    int bucket_index;
    for (
        int i = 0; i < data_size + 1; i++) {
        if (guessed_suffix_array[i] != -1) {
            j = guessed_suffix_array[i] - 1;
            if (j >= 0 && typemap[j] == L_TYPE) {
                bucket_index = data[i];
                guessed_suffix_array[bucket_heads[bucket_index]] = j;
                bucket_heads[bucket_index]++;
            }
        }
    }
    free(bucket_heads);
}

void induce_sort_s(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const bucket_sizes, array guessed_suffix_array) {
    array bucket_tails = find_bucket_tails(bucket_sizes);
    int j;
    int bucket_index;
    for (
        int i = data_size; i >= 0; i--) {
        j = guessed_suffix_array[i] - 1;
        if (j > 0 && typemap[j] == S_TYPE) {
            bucket_index = data[j];
            guessed_suffix_array[bucket_tails[bucket_index]] = j;
            bucket_tails[bucket_index]--;
        }
    }
    free(bucket_tails);
}

void summarize_suffix_array(
    array_const data, int data_size, suffix_t_array_const typemap,
    array_const guessed_suffix_array, CList *p_summary_data,
    CList *p_summary_suffix_offsets, int *p_summary_alphabet_size) {
    array lms_names = (array) (malloc((data_size + 1) * sizeof(*lms_names)));
    for (
        int i = 0; i < data_size + 1; i++) {
        lms_names[i] = -1;
    }
    int current_name = 0;
    int last_lms_suffix_offset;
    int suffix_offset;
    lms_names[guessed_suffix_array[0]] = current_name;
    last_lms_suffix_offset = guessed_suffix_array[0];
    for (
        int i = 0; i < data_size + 1; i++) {
        suffix_offset = guessed_suffix_array[i];
        if (is_lms_char(typemap, suffix_offset)) {
            if (!lms_subdata_equal(
                data, data_size, typemap, last_lms_suffix_offset,
                suffix_offset)) {
                current_name++;
            }
            last_lms_suffix_offset = suffix_offset;
            lms_names[suffix_offset] = current_name;
        }
    }
    for (
        int i = 0; i < data_size + 1; i++) {
        if (lms_names[i] != -1) {
            clist_push(p_summary_suffix_offsets, i);
            clist_push(p_summary_data, lms_names[i]);
        }
    }
    *p_summary_alphabet_size = current_name + 1;
    free(lms_names);
}

array make_summary_suffix_array(
    CList *p_summary_data, int summary_alphabet_size) {
    if (summary_alphabet_size == p_summary_data->size) {
        array summary_suffix_array = (array) (
            malloc(
                (p_summary_data->size + 1) * sizeof(*summary_suffix_array)));
        summary_suffix_array[0] = p_summary_data->size;
        int value;
        for (
            int i = 0; i < p_summary_data->size; i++) {
            value = p_summary_data->tab[i];
            summary_suffix_array[value + 1] = i;
        }
        return summary_suffix_array;
    } else {
        return make_suffix_array_induced_sorting(
            p_summary_data->tab, p_summary_data->size, summary_alphabet_size);
    }
}

array accurate_lms_sort(
    array_const data, int data_size, array_const bucket_sizes,
    array_const summary_suffix_array, CList *p_summary_data,
    CList *p_summary_suffix_offsets) {
    array suffix_offsets = (array) (
        malloc((data_size + 1) * sizeof(*suffix_offsets)));
    for (
        int i = 0; i < data_size + 1; i++) {
        suffix_offsets[i] = -1;
    }
    array bucket_tails = find_bucket_tails(bucket_sizes);
    // TODO: Here, it could be i >= 0 or i >= 2 ?
    int data_index;
    int bucket_index;
    for (
        int i = p_summary_data->size; i >= 0; i--) {
        data_index = p_summary_suffix_offsets->tab[summary_suffix_array[i]];
        bucket_index = data[data_index];
        suffix_offsets[bucket_tails[bucket_index]] = data_index;
        bucket_tails[bucket_index]--;
    }
    suffix_offsets[0] = data_size;
    return suffix_offsets;
}
