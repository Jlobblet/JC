#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include "jarray.h"

/// Initialise an N-dimensional array.
///
/// \param arr The array to initialise.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr NDArray_init(NDArray* arr) {
    assert(arr->dims);
    size_t n = 1;
    for (size_t i = 0; i < arr->dims; i++) {
        assert(arr->size[i]);
        n *= arr->size[i];
    }
    arr->data = calloc(n, sizeof(NDArray_Backer));
    if (arr->data == NULL) {
        return -1;
    }
    return 0;
}

/// Clean up an NDArray.
/// \param arr The array to finalise.
void NDArray_dest(NDArray* arr) {
    free(arr->data);
}

/// Get the value in a given cell of an N-dimensional array.
/// \param arr The array to retrieve a value from.
/// \param index The index of the value to retrieve.
/// \param ...
/// \return The value.
NDArray_Backer NDArray_get(NDArray* arr, uptr index, ...) {
    assert(index < arr->size[0]);
    uptr i = index;
    va_list args;
    va_start(args, index);
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
void NDArray_set(NDArray* arr, NDArray_Backer value, uptr index, ...) {
    assert(index < arr->size[0]);
    uptr i = index;
    va_list args;
    va_start(args, index);
    for (uptr n = 1; n < arr->dims; n++) {
        uptr i_n = va_arg(args, uptr);
        assert(i_n < arr->size[n]);
        i = i * arr->size[n] + i_n;
    }
    va_end(args);
    arr->data[i] = value;
}
