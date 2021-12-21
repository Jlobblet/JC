#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "jarray.h"

inline uptr NDArray_total_size(NDArray* arr) {
    assert(arr->dims);
    uptr size = 1;
    for (uptr i = 0; i < arr->dims; i++) {
        uptr s = arr->size[i];
        assert(s);
        size *= s;
    }
    return size;
}

/// Initialise an N-dimensional array.
///
/// \param arr The array to initialise.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr NDArray_init(NDArray* arr) {
    uptr n = NDArray_total_size(arr);
    arr->data = calloc(n, sizeof(NDArrayBacker));
    if (arr->data == NULL) {
        return -1;
    }
    return 0;
}

/// Clean up an NDArray.
/// \param arr The array to finalise.
void NDArray_dest(NDArray* arr) {
    free(arr->data);
    arr->data = NULL;
}

/// Get a pointer the the value in a given cell of an N-dimensional array.
/// \param arr The array to retrieve a pointer from.
/// \param index The index of the value to retrieve a pointer to.
/// \param ...
/// \return A pointer to that value.
NDArrayBacker* NDArray_ptr(NDArray* arr, uptr index, ...) {
    va_list args;
    va_start(args, index);
    uptr i = index;
    for (uptr n = 1; n < arr->dims; n++) {
        uptr i_n = va_arg(args, uptr);
        assert(i_n < arr->size[n]);
        i = i * arr->size[n] + i_n;
    }
    va_end(args);
    return &arr->data[i];
}

/// Get the value in a given cell of an N-dimensional array.
/// \param arr The array to retrieve a value from.
/// \param index The index of the value to retrieve.
/// \param ...
/// \return The value.
NDArrayBacker NDArray_get(NDArray* arr, uptr index, ...) {
    va_list args;
    va_start(args, index);
    uptr i = index;
    for (uptr n = 1; n < arr->dims; n++) {
        uptr i_n = va_arg(args, uptr);
        assert(i_n < arr->size[n]);
        i = i * arr->size[n] + i_n;
    }
    va_end(args);
    return arr->data[i];
}

/// Set the value in a given cell of an N-dimensional array.
/// \param arr The array to set a value in.
/// \param value The value.
/// \param index The index to set the value of.
/// \param ...
void NDArray_set(NDArray* arr, NDArrayBacker value, uptr index, ...) {
    va_list args;
    va_start(args, index);
    uptr i = index;
    for (uptr n = 1; n < arr->dims; n++) {
        uptr i_n = va_arg(args, uptr);
        assert(i_n < arr->size[n]);
        i = i * arr->size[n] + i_n;
    }
    va_end(args);
    arr->data[i] = value;
}

void NDArray_map(NDArray* arr, NDArray_mapping_t mapping) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] = mapping(arr->data[i]);
    }
}

void NDArray_add(NDArray* arr, NDArrayBacker value) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] += value;
    }
}

void NDArray_sub(NDArray* arr, NDArrayBacker value) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] -= value;
    }
}

void NDArray_mul(NDArray* arr, NDArrayBacker value) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] *= value;
    }
}

void NDArray_div(NDArray* arr, NDArrayBacker value) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] /= value;
    }
}

void NDArray_and(NDArray* arr, NDArrayBacker value) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] &= value;
    }
}

void NDArray_or(NDArray* arr, NDArrayBacker value) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] |= value;
    }
}

void NDArray_xor(NDArray* arr, NDArrayBacker value) {
    uptr n = NDArray_total_size(arr);
    for (uptr i = 0; i < n; i++) {
        arr->data[i] ^= value;
    }
}

static void NDArray_assert2(NDArray* left, NDArray* right) {
#ifndef NDEBUG
    assert(left->dims == right->dims);
    for (uptr i = 0; i < left->dims; i++) {
        assert(left->size[i] == right->size[i]);
    }
#endif //NDEBUG
}

void NDArray_map2(NDArray* left, NDArray* right, NDArray_map2ing_t map2ing) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = map2ing(left->data[i], right->data[i]);
    }
}

void NDArray_arr_add(NDArray* left, NDArray* right) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = left->data[i] + right->data[i];
    }
}

void NDArray_arr_sub(NDArray* left, NDArray* right) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = left->data[i] - right->data[i];
    }
}

void NDArray_arr_mul(NDArray* left, NDArray* right) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = left->data[i] * right->data[i];
    }
}

void NDArray_arr_div(NDArray* left, NDArray* right) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = left->data[i] / right->data[i];
    }
}

void NDArray_arr_and(NDArray* left, NDArray* right) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = left->data[i] & right->data[i];
    }
}

void NDArray_arr_or(NDArray* left, NDArray* right) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = left->data[i] | right->data[i];
    }
}

void NDArray_arr_xor(NDArray* left, NDArray* right) {
    NDArray_assert2(left, right);
    uptr n = NDArray_total_size(left);
    for (uptr i = 0; i < n; i++) {
        left->data[i] = left->data[i] ^ right->data[i];
    }
}
