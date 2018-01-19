#ifndef TIPE_C_SUFFIX_ARRAY
#define TIPE_C_SUFFIX_ARRAY

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char uchar;
typedef uchar * str;
typedef uchar suffix_t;
typedef suffix_t * suffix_t_array;

suffix_t_array build_type_array(str, size_t);
int is_lms_char(suffix_t_array, size_t);
void show_typemap(str, size_t);
size_t *find_bucket_sizes(str, size_t);
int lms_substring_equal(str, size_t, suffix_t_array, size_t, size_t);
size_t *find_bucket_heads(size_t *);
size_t *find_bucket_tails(size_t *);

#endif  // TIPE_C_SUFFIX_ARRAY
