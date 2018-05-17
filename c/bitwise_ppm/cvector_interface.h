/*
 * Copyright (C) 2018 Thomas BAGREL

 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, version 3.0.

 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
 * for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#define _CONCAT(a, b) a ## b
#define CONCAT(a, b) _CONCAT(a, b)

#define index_t long
#define NOT_FOUND_INDEX ((index_t) (-1))
#define ROUND_INDEX(x) ((index_t) (lrint(x)))

/*
 * Be careful, as this library can work for different types,
 * it will be copied every time it is imported.
 * Avoid unnecessary copies if possible.
 * Functions starting with __ like __cvector_setspace musn't be used by the
 * user!
 */

/**
 * Space in element units of a fresh created cvector, if no space was
 * specified.
 */
#ifndef CVECTOR_INIT_SPACE
#define CVECTOR_INIT_SPACE 8
#endif

/**
 * Space factor used when a copy of cvector is created, or a concatenation of
 * two cvectors. It means that the resulting array will have a space for
 * INIT_FACTOR * actual size items.
 */
#ifndef CVECTOR_INIT_FACTOR
#define CVECTOR_INIT_FACTOR 1.25
#endif

/**
 * Space factor used when a cvector becomes too short to hold additional
 * values. It means that the new cvector will have a space for ADDSPACE_FACTOR
 * * the old space.
 */
#ifndef CVECTOR_ADDSPACE_FACTOR
#define CVECTOR_ADDSPACE_FACTOR 2.0
#endif

/**
 * Threshold from which the cvector will be shrank in a readjust operation.
 * It means that if the current size of the cvector is under SHRINK_THRESHOLD *
 * its space, it will be shrank. Set to under 0 to prevent shrink during
 * readjust operations.
 */
#ifndef CVECTOR_SHRINK_THRESHOLD
#define CVECTOR_SHRINK_THRESHOLD 0.5
#endif

/**
 * Space factor used when a shrink operation is triggered. It means that the
 * new space of the cvector will be SHRINK_FACTOR * the current space.
 */
#ifndef CVECTOR_SHRINK_FACTOR
#define CVECTOR_SHRINK_FACTOR 0.5
#endif

/**
 * Threshold from which the cvector will be extended in a readjust operation.
 * It means that if the current size of the cvector is above EXTEND_THRESHOLD
 * * its space, it will be extended. Set to above 1 to prevent extend during
 * readjust operations.
 */
#ifndef CVECTOR_EXTEND_THRESHOLD
#define CVECTOR_EXTEND_THRESHOLD 0.90
#endif

/**
 * Space factor used when a extend operation is triggered. It means that the
 * new space of the cvector will be EXTEND_FACTOR * the current space.
 */
#ifndef CVECTOR_EXTEND_FACTOR
#define CVECTOR_EXTEND_FACTOR 2.0
#endif

/**
 * Print debug function called when some error or log message needs to be
 * printed on the screen or the log. The function signature must be
 *     void print_debug(int level, const char *message)
 */
#ifdef CVECTOR_ERROR_FUNC
#define CVECTOR_ERROR(lvl, msg) CVECTOR_ERROR_FUNC(lvl, msg)
#else
#define CVECTOR_ERROR(lvl, msg) NULL
#endif

/**
 * Type of the elements to hold in this instance of the cvector library.
 * BE CAREFUL! The specified type must be a correct indentifier, since it
 * will prefix any function of this cvector instance.
 * For example
 *     #define CVECTOR_T int *
 * should be replaced with
 *     typedef int * pint;
 *     #define CVECTOR_T pint
 */
#ifndef CVECTOR_T
#define CVECTOR_T int
#endif

/**
 * Default value for the type of this instance of cvector, used when an error
 * occurs and when a function needs to return a value.
 */
#ifndef CVECTOR_DEFAULT_VALUE
#define CVECTOR_DEFAULT_VALUE 0
#endif

#ifndef CVECTOR_HASH_T
#define CVECTOR_HASH_T size_t
#endif

#define value_t CVECTOR_T
#define hash_t CVECTOR_HASH_T
#define cvector CONCAT(CVECTOR_T, _vect)
#define __cvector_setspace CONCAT(CVECTOR_T, _vect__setspace)
#define __cvector_shrink CONCAT(CVECTOR_T, _vect__shrink)
#define __cvector_extend CONCAT(CVECTOR_T, _vect__extend)
#define cvector_readjust CONCAT(CVECTOR_T, _vect__readjust)
#define cvector_addspace CONCAT(CVECTOR_T, _vect__addspace)
#define cvector_new CONCAT(CVECTOR_T, _vect__new)
#define cvector_new_space CONCAT(CVECTOR_T, _vect__new_space)
#define cvector_new_copy CONCAT(CVECTOR_T, _vect__new_copy)
#define cvector_new_copy_space CONCAT(CVECTOR_T, _vect__new_copy_space)
#define cvector_free CONCAT(CVECTOR_T, _vect__free)
#define cvector_getsize CONCAT(CVECTOR_T, _vect__getsize)
#define cvector_free_func CONCAT(CVECTOR_T, _vect__free_value)
#define cvector_add CONCAT(CVECTOR_T, _vect__add)
#define cvector_addi CONCAT(CVECTOR_T, _vect__addi)
#define cvector_insert CONCAT(CVECTOR_T, _vect__insert)
#define cvector_remove CONCAT(CVECTOR_T, _vect__remove)
#define cvector_removei CONCAT(CVECTOR_T, _vect__removei)
#define cvector_drop CONCAT(CVECTOR_T, _vect__drop)
#define cvector_clear CONCAT(CVECTOR_T, _vect__clear)
#define cvector_get CONCAT(CVECTOR_T, _vect__get)
#define cvector_safeget CONCAT(CVECTOR_T, _vect__safeget)
#define cvector_set CONCAT(CVECTOR_T, _vect__set)
#define cvector_safeset CONCAT(CVECTOR_T, _vect__safeset)
#define cvector_appendto CONCAT(CVECTOR_T, _vect__appendto)
#define cvector_concat CONCAT(CVECTOR_T, _vect__concat)
#define cvector_reversed CONCAT(CVECTOR_T, _vect__reversed)
#define cvector_hash CONCAT(CVECTOR_T, _vect__hash)
#define cvector_equal CONCAT(CVECTOR_T, _vect__equal)
#define cvector_equal_func CONCAT(CVECTOR_T, _vect__equal_func)
#define cvector_toarray CONCAT(CVECTOR_T, _vect__toarray)
#define cvector_replace CONCAT(CVECTOR_T, _vect__replace)
#define cvector_replace_func CONCAT(CVECTOR_T, _vect__replace_func)
#define cvector_sort CONCAT(CVECTOR_T, _vect__sort)
#define cvector_indexof CONCAT(CVECTOR_T, _vect__indexof)
#define cvector_indexof_func CONCAT(CVECTOR_T, _vect__indexof_func)
#define cvector_in CONCAT(CVECTOR_T, _vect__in)
#define cvector_in_func CONCAT(CVECTOR_T, _vect__in_func)
#define cvector_slice CONCAT(CVECTOR_T, _vect__slice)
#define cvector_slicetoarray CONCAT(CVECTOR_T, _vect__slicetoarray)
/*#define CVECTOR_ERROR(lvl, msg) \
    (((CVECTOR_ERROR_FUNC) == NULL) ? \
         NULL : CVECTOR_ERROR_FUNC(lvl, msg)*/

typedef struct cvector cvector;

/**
* Main struct holding the cvector structure.
*/
struct cvector {
    index_t _size;
    index_t _space;
    value_t *_vector;
};

void __cvector_setspace(cvector *p_cvector, index_t new_space);

void __cvector_shrink(cvector *p_cvector);

void __cvector_extend(cvector *p_cvector);

void cvector_readjust(cvector *p_cvector);

void cvector_addspace(cvector *p_cvector);

cvector *cvector_new();

cvector *cvector_new_space(index_t space);

cvector *cvector_new_copy(cvector *p_original);

cvector *cvector_new_copy_space(cvector *p_original, index_t space);

void cvector_free(cvector *p_cvector);

void cvector_free_func(cvector *p_vector, void (*free_value)(value_t));

index_t cvector_getsize(cvector *p_cvector);

void cvector_add(cvector *p_cvector, value_t value);

void cvector_addi(cvector *p_cvector, value_t value, index_t index);

void cvector_insert(cvector *p_cvector, value_t value);

value_t cvector_remove(cvector *p_cvector);

value_t cvector_removei(cvector *p_cvector, index_t index);

value_t cvector_drop(cvector *p_cvector);

void cvector_clear(cvector *p_cvector);

value_t cvector_get(cvector *p_cvector, index_t index);

value_t cvector_safeget(cvector *p_cvector, index_t index);

void cvector_set(cvector *p_cvector, value_t value, index_t index);

void cvector_safeset(cvector *p_cvector, value_t value, index_t index);

void cvector_appendto(cvector *p_cvector, cvector *p_add);

cvector *cvector_concat(cvector *p_cvector_1, cvector *p_cvector_2);

cvector *cvector_reversed(cvector *p_cvector);

hash_t cvector_hash(cvector *p_cvector, hash_t (*hash_value)(value_t));

bool cvector_equal(cvector *p_cvector_1, cvector *p_cvector_2);

bool cvector_equal_func(
    cvector *p_cvector_1, cvector *p_cvector_2,
    bool (*equal_value)(value_t, value_t));

value_t *cvector_toarray(cvector *p_cvector);

bool cvector_replace(
    cvector *p_cvector, value_t original, value_t replacement);

bool cvector_replace_func(
    cvector *p_cvector, value_t original, value_t replacement,
    bool (*equal_value)(value_t, value_t));

void cvector_sort(
    cvector *p_cvector, int (*comp_value)(const void *, const void *));

index_t cvector_indexof(cvector *p_cvector, value_t value);

index_t cvector_indexof_func(
    cvector *p_cvector, value_t value, bool (*equal_value)(value_t, value_t));

bool cvector_in(cvector *p_cvector, value_t value);

bool cvector_in_func(
    cvector *p_cvector, value_t value, bool (*equal_value)(value_t, value_t));

cvector *
cvector_slice(cvector *p_cvector, index_t from, index_t to, index_t step);

value_t *cvector_slicetoarray(
    cvector *p_cvector, index_t from, index_t to, index_t step);

