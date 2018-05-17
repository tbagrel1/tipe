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

/**
 * Sets space of the specified cvector to new_space
 * @param p_cvector a pointer to the cvector
 * @param new_space the new space for the specified cvector
 */
void __cvector_setspace(cvector *p_cvector, index_t new_space) {
    p_cvector->_space = new_space;
    p_cvector->_vector = (value_t *) (
        realloc(
            p_cvector->_vector,
            p_cvector->_space * sizeof(*p_cvector->_vector)));
}

/**
 * Shrinks the specified cvector without any check about its size.
 * @param p_cvector a pointer to the cvector
 */
void __cvector_shrink(cvector *p_cvector) {
    __cvector_setspace(
        p_cvector, ROUND_INDEX(CVECTOR_SHRINK_FACTOR * p_cvector->_space + 1));
}

/**
 * Extends the specified cvector without any check about its size.
 * @param p_cvector a pointer to the cvector
 */
void __cvector_extend(cvector *p_cvector) {
    __cvector_setspace(
        p_cvector, ROUND_INDEX(CVECTOR_EXTEND_FACTOR * p_cvector->_space + 1));
}

/**
 * Readjusts space of the specified cvector if needed, according to
 * SHRINK_THRESHOLD and EXTEND_THRESHOLD.
 * @param p_cvector a pointer to the cvector
 */
void cvector_readjust(cvector *p_cvector) {
    if (p_cvector->_size < CVECTOR_SHRINK_THRESHOLD * p_cvector->_space) {
        __cvector_shrink(p_cvector);
    } else if (p_cvector->_size >
               CVECTOR_EXTEND_THRESHOLD * p_cvector->_space) {
        __cvector_extend(p_cvector);
    }
}

/**
 * Adds space (according to the DEFAULT_ADDSPACE_FACTOR) to the specified
 * cvector.
 * @param p_cvector a pointer to the cvector to extend.
 */
void cvector_addspace(cvector *p_cvector) {
    __cvector_setspace(
        p_cvector,
        ROUND_INDEX(CVECTOR_ADDSPACE_FACTOR * p_cvector->_space + 1));
}


/**
 * Creates a new cvector which can hold at the beginning at least
 * DEFAULT_INIT_SPACE elements.
 * @return a pointer to the new cvector
 */
cvector *cvector_new() {
    cvector *p_cvector = (cvector *) (malloc(sizeof(*p_cvector)));
    p_cvector->_size = 0;
    p_cvector->_space = CVECTOR_INIT_SPACE;
    p_cvector->_vector =
        (value_t *) (malloc(sizeof(*p_cvector->_vector) * p_cvector->_space));
    return p_cvector;
}

/**
 * Creates a new cvector which can hold at the beginning at least space
 * elements.
 * @param space desired space for the new cvector
 * @return a pointer to the new cvector
 */
cvector *cvector_new_space(index_t space) {
    if (space < 0) {
        CVECTOR_ERROR(0, "InvalidSize: cannot create a cvector with a "
                         "negative size");
        return NULL;
    }
    cvector *p_cvector = (cvector *) (malloc(sizeof(*p_cvector)));
    p_cvector->_size = 0;
    p_cvector->_space = space;
    p_cvector->_vector =
        (value_t *) (malloc(sizeof(*p_cvector->_vector) * p_cvector->_space));
    return p_cvector;
}

/**
 * Creates a new cvector which is a copy of the specified one.
 * @param p_original a pointer to the cvector to copy
 * @return a pointer to the new (clone) cvector
 */
cvector *cvector_new_copy(cvector *p_original) {
    cvector *p_cvector = (cvector *) (malloc(sizeof(*p_cvector)));
    p_cvector->_size = p_original->_size;
    p_cvector->_space =
        ROUND_INDEX(CVECTOR_INIT_FACTOR * p_original->_size + 1);
    p_cvector->_vector =
        (value_t *) (malloc(sizeof(*p_cvector->_vector) * p_cvector->_space));
    memcpy(
        p_cvector->_vector, p_original->_vector,
        sizeof(*p_original->_vector) * p_original->_size);
    return p_cvector;
}

/**
 * Creates a new cvector which is a copy of the specified one and which can
 * hold at least space elements.
 * @param p_original a pointer to the cvector to copy
 * @param space desired space for the new (clone) cvector. space must be
 * greater or equal than the size of the original cvector
 * @return a pointer to the new (clone) cvector
 */
cvector *cvector_new_copy_space(cvector *p_original, index_t space) {
    if (space < 0) {
        CVECTOR_ERROR(0, "InvalidSize: cannot create a cvector with a "
                         "negative size");
        return NULL;
    }
    if (space < p_original->_size) {
        CVECTOR_ERROR(0, "InvalidSize: cannot clone a cvector in a "
                         "recipient with less space than the original size");
        return NULL;
    }
    cvector *p_cvector = (cvector *) (malloc(sizeof(*p_cvector)));
    p_cvector->_size = p_original->_size;
    p_cvector->_space =
        ROUND_INDEX(CVECTOR_INIT_FACTOR * p_original->_size + 1);
    p_cvector->_vector =
        (value_t *) (malloc(sizeof(*p_cvector->_vector) * p_cvector->_space));
    memcpy(
        p_cvector->_vector, p_original->_vector,
        sizeof(*p_original->_vector) * p_original->_size);
    return p_cvector;
}

/**
 * Frees the specified cvector.
 * @param p_cvector a pointer to the cvector to free
 */
void cvector_free(cvector *p_cvector) {
    free(p_cvector->_vector);
    free(p_cvector);
}

/**
 * Applies the specified free function of each value of the cvector, and then
 * frees it too.
 * @param p_vector a pointer to the cvector to free
 * @param free_value the function to free each value of the cvector
 */
void cvector_free_func(cvector *p_vector, void (*free_value)(value_t)) {
    for (index_t i = 0; i < p_vector->_size; i++) {
        free_value(p_vector->_vector[i]);
    }
    free(p_vector->_vector);
    free(p_vector);
}

/**
 * Size getter. Returns the size of the cvector.
 * @param p_cvector a pointer to the cvector
 * @return the size of the cvector
 */
index_t cvector_getsize(cvector *p_cvector) {
    return p_cvector->_size;
}

/**
 * Adds the specified element at the end of the cvector.
 * @param p_cvector a pointer to the cvector
 * @param value the value to push at the end of the cvector
 */
void cvector_add(cvector *p_cvector, value_t value) {
    if (p_cvector->_size < p_cvector->_space) {
        p_cvector->_vector[p_cvector->_size] = value;
        p_cvector->_size++;
    } else {
        cvector_addspace(p_cvector);
        cvector_add(p_cvector, value);
    }
}

/**
 * Adds the specified element a the position index in the cvector, and shift
 * following elements to the right.
 * @param p_cvector a pointer to the cvector
 * @param value the value to push a the position index in the cvector
 * @param index the index where the specified value will be inserted
 */
void cvector_addi(cvector *p_cvector, value_t value, index_t index) {
    if (index < 0) {
        index = p_cvector->_size - index;
    }
    if (index > p_cvector->_size) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: cannot add an item after the end "
                         "of the array");
        return;
    }
    if (index < 0) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (negative) index");
    }
    if (p_cvector->_size < p_cvector->_space) {
        if (index == p_cvector->_size) {
            cvector_add(p_cvector, value);
        } else {
            for (index_t i = p_cvector->_size - 1; i >= index; i--) {
                p_cvector->_vector[i + 1] = p_cvector->_vector[i];
            }
            p_cvector->_size++;
            p_cvector->_vector[index] = value;
        }
    } else {
        cvector_addspace(p_cvector);
        cvector_addi(p_cvector, value, index);
    }
}

/**
 * Adds the specified value at the beginning of the cvector, and shift
 * following elements to the right.
 * @param p_cvector a pointer to the cvector
 * @param value the value to add at the beginning of the cvector
 */
void cvector_insert(cvector *p_cvector, value_t value) {
    cvector_addi(p_cvector, value, 0);
}

/**
 * Removes the last element of the cvector and returns it. If the cvector is
 * empty, prints an error and returns DEFAULT_VALUE.
 * @param p_cvector a pointer to the cvector
 * @return The last value of the cvector if it is not empty, DEFAULT_VALUE
 * otherwise
 */
value_t cvector_remove(cvector *p_cvector) {
    if (p_cvector->_size <= 0) {
        CVECTOR_ERROR(0, "Empty: unable to remove the last item of an "
                         "empty cvector");
        return CVECTOR_DEFAULT_VALUE;
    }
    value_t value = p_cvector->_vector[p_cvector->_size - 1];
    p_cvector->_size--;
    cvector_readjust(p_cvector);
    return value;
}

/**
 * Removes the element located at the specified index, and returns it. If the
 * cvector is empty or if the index is incorrect, prints an error and returns
 * DEFAULT_VALUE.
 * @param p_cvector a pointer to the cvector
 * @param index the index where the element will be removed
 * @return the removed element or DEFAULT_VALUE if an error occurs
 */
value_t cvector_removei(cvector *p_cvector, index_t index) {
    if (p_cvector->_size <= 0) {
        CVECTOR_ERROR(0, "Empty: unable to remove the last item of an "
                         "empty cvector");
        return CVECTOR_DEFAULT_VALUE;
    }
    if (index < 0) {
        index += p_cvector->_size;
    }
    if (index < 0) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (negative) index");
        return CVECTOR_DEFAULT_VALUE;
    }
    if (index > p_cvector->_size - 1) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: index greater than the size "
                         "of the cvector");
        return CVECTOR_DEFAULT_VALUE;
    }
    if (index == p_cvector->_size - 1) {
        return cvector_remove(p_cvector);
    } else {
        value_t value = p_cvector->_vector[index];
        for (index_t i = index; i < p_cvector->_size - 1; i++) {
            p_cvector->_vector[i] = p_cvector->_vector[i + 1];
        }
        p_cvector->_size--;
        cvector_readjust(p_cvector);
        return value;
    }
}

/**
 * Removes the first element of the cvector. If the cvector is empty, prints an
 * error anrd returns DEFAULT_VALUE.
 * @param p_cvector a pointer to the cvector
 * @return the remove (first) element, or DEFAULT_VALUE if an error occurs
 */
value_t cvector_drop(cvector *p_cvector) {
    return cvector_removei(p_cvector, 0);
}

/**
 * Removes all elements of the cvector without changing its space (that is to
 * say without calling cvector_readjust).
 * @param p_cvector a pointer to the cvector
 */
void cvector_clear(cvector *p_cvector) {
    p_cvector->_size = 0;
}

/**
 * Returns the value at the specified index in the cvector. Prints an error
 * message and returns DEFAULT_VALUE if the specified index is invalid.
 * @param p_cvector a pointer to the cvector
 * @param index the index of the value to get
 * @return the desired value if the index is correct, DEFAULT_VALUE otherwise
 */
value_t cvector_get(cvector *p_cvector, index_t index) {
    if (index < 0) {
        index += p_cvector->_size;
    }
    if (index < 0) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (negative) index");
        return CVECTOR_DEFAULT_VALUE;
    }
    if (index > p_cvector->_size - 1) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: index greater than "
                         "the size of the cvector");
        return CVECTOR_DEFAULT_VALUE;
    }
    return p_cvector->_vector[index];
}

/**
 * Returns the value at the specified index in the cvector. Only prints a
 * warning and returns DEFAULT_VALUE if the specified index is invalid.
 * @param p_cvector a pointer to the cvector
 * @param index the index of the value to get
 * @return the desired value if the index is correct, DEFAULT_VALUE otherwise
 */
value_t cvector_safeget(cvector *p_cvector, index_t index) {
    if (index < 0) {
        index += p_cvector->_size;
    }
    if (index < 0) {
        CVECTOR_ERROR(1, "IndexOutOfBounds: invalid (negative) index");
        return CVECTOR_DEFAULT_VALUE;
    }
    if (index > p_cvector->_size - 1) {
        CVECTOR_ERROR(1, "IndexOutOfBounds: index strictly greater than "
                         "the size of the array");
        return CVECTOR_DEFAULT_VALUE;
    }
    return p_cvector->_vector[index];
}

/**
 * Sets the value of the element located at the specified index. Raises error
 * if the specified index is invalid.
 * @param p_cvector a pointer to the cvector
 * @param value the value which will be placed at the index position
 * @param index the index where the value will be set
 */
void cvector_set(cvector *p_cvector, value_t value, index_t index) {
    if (index < 0) {
        index += p_cvector->_size;
    }
    if (index < 0) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (negative) index");
        return;
    }
    if (index > p_cvector->_size - 1) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: index is strictly greater than "
                         "the size of the array");
    }
    p_cvector->_vector[index] = value;
}

/**
 * Sets the value of the element located at the specified position. Only
 * raises warning if the index is invalid, or extends the cvector to be able
 * to set the value at the specified index.
 * @param p_cvector a pointer to the cvector
 * @param value the value which will be inserted at the index position
 * @param index the index where the value will be set
 */
void cvector_safeset(cvector *p_cvector, value_t value, index_t index) {
    if (index < 0) {
        index += p_cvector->_size;
    }
    if (index < 0) {
        CVECTOR_ERROR(1, "IndexOutOfBounds: invalid (negative) index");
        return;
    }
    if (index > p_cvector->_size - 1) {
        CVECTOR_ERROR(2, "IndexOutOfBounds: the cvector will be extended "
                         "to be able to store an element at the specified index");
        if (index > p_cvector->_space - 1) {
            __cvector_setspace(p_cvector, index + 1);
        }
        for (index_t i = p_cvector->_size; i < index; i++) {
            p_cvector->_vector[i] = CVECTOR_DEFAULT_VALUE;
        }
        p_cvector->_vector[index] = value;
        p_cvector->_size = index + 1;
    } else {
        p_cvector->_vector[index] = value;
    }
}

/**
 * Appends element of the cvector pointed by p_add at the end of the cvector
 * pointed by p_cvector.
 * @param p_cvector a pointer to the cvector where elements will be appended
 * @param p_add a pointer to the cvector containing elements to copy
 */
void cvector_appendto(cvector *p_cvector, cvector *p_add) {
    if (p_cvector->_space >= p_cvector->_size + p_add->_size) {
        memcpy(
            p_cvector->_vector + p_cvector->_size, p_add->_vector,
            sizeof(*p_cvector->_vector) * p_add->_size);
        p_cvector->_size += p_add->_size;
    } else {
        __cvector_setspace(
            p_cvector, ROUND_INDEX(CVECTOR_INIT_FACTOR * (
                p_cvector->_size + p_add->_size)) + 1);
        cvector_appendto(p_cvector, p_add);
    }
}

/**
 * Returns a new cvector which is the concatenation of the two specified
 * cvectors
 * @param p_cvector_1 a pointer to the first cvector to concatenate
 * @param p_cvector_2 a pointer to the first cvector to concatenate
 * @return a pointer to the resulting cvector
 */
cvector *cvector_concat(cvector *p_cvector_1, cvector *p_cvector_2) {
    cvector *p_result = cvector_new_space(ROUND_INDEX(CVECTOR_INIT_FACTOR * (
        p_cvector_1->_size + p_cvector_2->_size) + 1));
    memcpy(
        p_result->_vector, p_cvector_1->_vector,
        sizeof(*p_cvector_1->_vector) * p_cvector_1->_size);
    memcpy(
        p_result->_vector + p_cvector_1->_size, p_cvector_2->_vector,
        sizeof(*p_cvector_2->_vector) * p_cvector_2->_size);
    p_result->_size = p_cvector_1->_size + p_cvector_2->_size;
    return p_result;
}

/**
 * Returns a cvector which contains the same elements as the specified one, but
 * in a reversed order.
 * @param p_cvector a pointer to the original cvector
 * @return the resulting cvector, containing elements of the specified
 * cvector in a reverse order
 */
cvector *cvector_reversed(cvector *p_cvector) {
    cvector
        *p_result =
        cvector_new_space(
            ROUND_INDEX(CVECTOR_INIT_FACTOR * p_cvector->_size + 1));
    for (index_t i = 0; i < p_cvector->_size; i++) {
        p_result->_vector[p_cvector->_size - 1 - i] = p_cvector->_vector[i];
    }
    p_result->_size = p_cvector->_size;
    return p_result;
}

/**
 * Returns the hash of the specified cvector, using djb2 algorithm by Dan
 * Bernstein, according to the specified hash function for values of the
 * cvector.
 * @param p_cvector a pointer to the cvector to hash
 * @param hash_value hash function for values of the cvector. Signature of
 * the hash value function must be
 *     hash_t hash_value(value_t value)
 * @return the computed hash of the specified cvector
 */
hash_t cvector_hash(cvector *p_cvector, hash_t (*hash_value)(value_t)) {
    hash_t hash = 5381;
    for (index_t i = 0; i < p_cvector->_size; i++) {
        hash = ((hash << 5) + hash) + hash_value(p_cvector->_vector[i]);
    }
    return hash;
}

/**
 * Returns true iif both specified cvectors are equal.
 * @param p_cvector_1 a pointer to the first cvector to test
 * @param p_cvector_2 a pointer to the second cvector to test
 * @return true if both specified cvectors are equal, false otherwise
 */
bool cvector_equal(cvector *p_cvector_1, cvector *p_cvector_2) {
    if (p_cvector_1->_size != p_cvector_2->_size) {
        return false;
    }
    for (index_t i = 0; i < p_cvector_1->_size; i++) {
        if (p_cvector_1->_vector[i] != p_cvector_2->_vector[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Returns true iif both specified cvectors are equal according to the
 * specified test function for values.
 * @param p_cvector_1 a pointer to the first cvector to test
 * @param p_cvector_2 a pointer to the second cvector to test
 * @param equal_value the test function for values. Its signature must be
 *     bool equal_value(value_t value_1, value_t value_2)
 * @return true if both specified cvectors are equal according to the test
 * function, false otherwise
 */
bool cvector_equal_func(
    cvector *p_cvector_1, cvector *p_cvector_2,
    bool (*equal_value)(value_t, value_t)) {
    if (p_cvector_1->_size != p_cvector_2->_size) {
        return false;
    }
    for (index_t i = 0; i < p_cvector_1->_size; i++) {
        if (!equal_value(p_cvector_1->_vector[i], p_cvector_2->_vector[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Returns a pointer to a c-style array holding the same elements as the
 * specified cvector.
 * @param p_cvector a pointer to the cvector
 * @return a c-style malloc-ed array holding the same elements as the specified
 * cvector, which must be freed after use
 */
value_t *cvector_toarray(cvector *p_cvector) {
    value_t
        *p_array = (value_t *) (malloc(sizeof(*p_array) * p_cvector->_size));
    memcpy(
        p_array, p_cvector->_vector,
        sizeof(*p_cvector->_vector) * p_cvector->_size);
    return p_array;
}

/**
 * Replace specified elements in the cvector and returns true if at least one
 * change was made.
 * @param p_cvector a pointer to the cvector
 * @param original original value to replace
 * @param replacement replacement value for original
 * @return true if at least one replacement was made, false otherwise
 */
bool
cvector_replace(cvector *p_cvector, value_t original, value_t replacement) {
    bool replaced = false;
    for (index_t i = 0; i < p_cvector->_size; i++) {
        if (p_cvector->_vector[i] == original) {
            p_cvector->_vector[i] = replacement;
            replaced = true;
        }
    }
    return replaced;
}

/**
 * Replace specified elements in the cvector and returns true if at least one
 * change was made. Test between elements of the cvector and original are made
 * with the specified function.
 * @param p_cvector a pointer to the cvector
 * @param original original value to replace
 * @param replacement replacement value for original
 * @param equal_value test function used to compare cvector elements and
 * original. Its signature must be
 *     bool equal_value(value_t value_1, value_t value_2)
 * @return true if at least one replacement was made, false otherwise
 */
bool cvector_replace_func(
    cvector *p_cvector, value_t original, value_t replacement,
    bool (*equal_value)(value_t, value_t)) {
    bool replaced = false;
    for (index_t i = 0; i < p_cvector->_size; i++) {
        if (equal_value(p_cvector->_vector[i], original)) {
            p_cvector->_vector[i] = replacement;
            replaced = true;
        }
    }
    return replaced;
}

/**
 * Sorts the elements in the cvector according to the specified comparison
 * function.
 * @param p_cvector a pointer to the cvector
 * @param comp_value a comparison function which must have the signature
 *     int comp_value(const void *p_a, const void *p_b)
 * and which must
 *     - return -1 if element a should be placed before element b
 *     - return 0 if element a and b could be placed at the same position
 *     - return 1 if element a should be placed after element b
 */
void cvector_sort(
    cvector *p_cvector, int (*comp_value)(const void *, const void *)) {
    qsort(
        p_cvector->_vector, p_cvector->_size, sizeof(*p_cvector->_vector),
        comp_value);
}

/**
 * Returns the first index where the specified value is found in the cvector.
 * If the value is not found, returns NOT_FOUND_INDEX value.
 * @param p_cvector a pointer to the cvector
 * @param value the value to found
 * @return the first index where the specified value was found, or
 * NOT_FOUND_INDEX if it was not found
 */
index_t cvector_indexof(cvector *p_cvector, value_t value) {
    for (index_t i = 0; i < p_cvector->_size; i++) {
        if (p_cvector->_vector[i] == value) {
            return i;
        }
    }
    return NOT_FOUND_INDEX;
}

/**
 * Returns the first index where the specified value is found, according to the
 * specified test function. Il the value is not found, returns NOT_FOUND_INDEX
 * value.
 * @param p_cvector a pointer to the cvector
 * @param value the value to found
 * @param equal_value the test function to check equality between values. Its
 * signature must be
 *     bool equal_value(value_t value_1, value_t value_2)
 * @return the first index where the specified value was found, or
 * NOT_FOUND_INDEX if it was not found
 */
index_t cvector_indexof_func(
    cvector *p_cvector, value_t value, bool (*equal_value)(value_t, value_t)) {
    for (index_t i = 0; i < p_cvector->_size; i++) {
        if (equal_value(p_cvector->_vector[i], value)) {
            return i;
        }
    }
    return NOT_FOUND_INDEX;
}

/**
 * Returns true iif the specified value was found in the cvector.
 * @param p_cvector a pointer to the cvector
 * @param value the value to found
 * @return true if the value was found, false otherwise
 */
bool cvector_in(cvector *p_cvector, value_t value) {
    return (cvector_indexof(p_cvector, value) != NOT_FOUND_INDEX);
}

/**
 * Returns true iif the specified value was found in the cvector according to
 * the specified test function.
 * @param p_cvector a pointer to the cvector
 * @param value the value to found
 * @param equal_value the test function to check equality between values. Its
 * signature must be
 *     bool equal_value(value_t value_1, value_t value_2)
 * @return true if the value was found, false otherwise
 */
bool cvector_in_func(
    cvector *p_cvector, value_t value, bool (*equal_value)(value_t, value_t)) {
    return (
        cvector_indexof_func(p_cvector, value, equal_value) !=
        NOT_FOUND_INDEX);
}

/**
 * Returns the slice [|from:to[| of the specified cvector. Prints an error and
 * return NULL if indexes are incorrect.
 * @param p_cvector a pointer to the cvector
 * @param from index of the begin of the slice, included
 * @param to index of the end of the slice, excluded
 * @param step step of the slice
 * @return the corresponding (cvector) slice
 */
cvector *
cvector_slice(cvector *p_cvector, index_t from, index_t to, index_t step) {
    if (from < 0) {
        from += p_cvector->_size;
    }
    if (to < 0) {
        to += p_cvector->_size;
    }
    if (from < 0 || to < 0) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (negative) index");
        return NULL;
    }
    if (from > p_cvector->_size - 1 || to > p_cvector->_size) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (out of bounds) index");
        return NULL;
    }
    if (from > to) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid index, from > to");
        return NULL;
    }
    if (step <= 0) {
        CVECTOR_ERROR(0, "InvalidStep: invalid (negative or null) step");
        return NULL;
    }
    index_t size;
    if (to == from) {
        size = 0;
    } else {
        size = 1 + ((to - from - 1) / step);  // Integer division there
    }
    cvector
        *p_result =
        cvector_new_space(ROUND_INDEX(CVECTOR_INIT_FACTOR * size + 1));
    for (index_t i = 0; i < size; i++) {
        p_result->_vector[i] = p_cvector->_vector[from + i * step];
    }
    p_result->_size = size;
    return p_result;
}

/**
 * Returns the slice [|from:to[| of the specified cvector as a c-style array.
 * Prints an error and return NULL if indexes are incorrect.
 * @param p_cvector a pointer to the cvector
 * @param from index of the begin of the slice, included
 * @param to index of the end of the slice, excluded
 * @param step step of the slice
 * @return the corresponding (c-style array) slice
 */
value_t *cvector_slicetoarray(
    cvector *p_cvector, index_t from, index_t to, index_t step) {
    if (from < 0) {
        from += p_cvector->_size;
    }
    if (to < 0) {
        to += p_cvector->_size;
    }
    if (from < 0 || to < 0) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (negative) index");
        return NULL;
    }
    if (from > p_cvector->_size - 1 || to > p_cvector->_size) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid (out of bounds) index");
        return NULL;
    }
    if (from > to) {
        CVECTOR_ERROR(0, "IndexOutOfBounds: invalid index, from > to");
        return NULL;
    }
    if (step <= 0) {
        CVECTOR_ERROR(0, "InvalidStep: invalid (negative or null) step");
        return NULL;
    }
    index_t size;
    if (to == from) {
        size = 0;
    } else {
        size = 1 + ((to - from - 1) / step);  // Integer division there
    }
    value_t *p_result = (value_t *) (malloc(sizeof(*p_result) * size));
    for (index_t i = 0; i < size; i++) {
        p_result[i] = p_cvector->_vector[from + i * step];
    }
    return p_result;
}
