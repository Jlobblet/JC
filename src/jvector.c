/// \file
/// Dynamic array that resizes itself as necessary.

#include <assert.h>
#include <stdlib.h>
#include <memory.h>
#include "jvector.h"

/// Create a `Vector` with sensible defaults.
/// \remark `capacity` is set to `8` and `growth_factor` is set to `2`.
/// \param vec The `Vector` to initialise.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_default(Vector* vec) {
    vec->length = 0;
    vec->capacity = 8;
    vec->growth_factor = 2;
    return Vector_init(vec);
}

/// Initialise a `Vector` with a given `capacity` and `growth_factor`.
/// \param vec The `Vector` to initialise.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_init(Vector* vec) {
    assert(vec->capacity);
    assert(vec->growth_factor > 1);
    vec->length = 0;
    vec->data = (void**)calloc(vec->capacity, sizeof(void*));
    if (vec->data == NULL) {
        return -1;
    }
    return 0;
}

/// Initialise a `Vector` with a given `capacity`.
/// \remark `growth_factor` is set to `2`.
/// \param vec The `Vector` to initialise.
/// \param capacity The initial capacity of `vec`.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_with_capacity(Vector* vec, uptr capacity) {
    vec->capacity = capacity;
    vec->growth_factor = 2;
    return Vector_init(vec);
}

/// Finalise a `Vector`.
/// Frees allocated memory and resets `capacity`, `growth_factor`, and `factor` to `0`.
/// \param vec The `Vector` to finalise.
void Vector_dest(Vector* vec) {
    free(vec->data);
    vec->capacity = 0;
    vec->growth_factor = 0;
    vec->length = 0;
}

/// Create a new `Vector` from the given values.
/// \param vec The `Vector` to initialise.
/// \param count The initial number of elements
/// \param elt The elements to put in the new `Vector`.
/// \param ...
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_new(Vector* vec, uptr count, void* elt, ...) {
    if (Vector_with_capacity(vec, count)) { return -1; }
    vec->length = count;
    vec->data[0] = elt;
    va_list args;
    va_start(args, elt);
    for (uptr i = 1; i < count; i++) {
        vec->data[i] = va_arg(args, void*);
    }
    va_end(args);
    return 0;
}

/// Create a `Vector` from an array of values.
/// \param vec The `Vector` to initialise.
/// \param count The number of elements in the array.
/// \param elts The array of elements.
/// \remark The values of `elts` are copied to the `Vector`'s buffer.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_from(Vector* vec, uptr count, void** elts) {
    vec->capacity = count;
    vec->growth_factor = 2;
    vec->length = count;
    if (Vector_init(vec)) { return -1; }
    memcpy(vec->data, elts, count * sizeof(void*));
    return 0;
}

/// Grow a `Vector`.
/// \remark This multiplies `capacity` by `growth_factor` and then allocates new memory for the `Vector`'s buffer.
/// \remark Values past `length` are set to `0`.
/// \param vec The `Vector` to grow.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_grow(Vector* vec) {
    vec->capacity *= vec->growth_factor;
    vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    if (vec->data == NULL) {
        return -1;
    }
    memset(vec->data + vec->length, 0, (vec->capacity - vec->length) * sizeof(void*));
    return 0;
}

/// Grow a `Vector` if it has no free space.
/// \param vec The `Vector` to grow.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_grow_if_full(Vector* vec) {
    if (vec->length == vec->capacity) {
        return Vector_grow(vec);
    }
    return 0;
}

/// Append (push) and element to the `Vector`.
/// \remark This function grows the `Vector` if necessary.
/// \param vec The `Vector` to append to.
/// \param elt The value to append.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_push_back(Vector* vec, void* elt) {
    if (Vector_grow_if_full(vec)) { return -1; }
    vec->data[vec->length] = elt;
    vec->length++;
    return 0;
}

/// Remove the last element of the `Vector` and put it into `out`.
///
/// \param vec The `Vector` to pop the last element from.
/// \param out Pointer to where to put the last element.
void Vector_pop_back(Vector* vec, void** out) {
    *out = vec->data[vec->length];
    vec->data[vec->length] = NULL;
    vec->length -= 1;
}

/// Append many elements to a `Vector` at once.
/// \remark This function performs at most one allocation.
/// \param vec The `Vector` to append to.
/// \param count The number of elements to append.
/// \param elt The elements to append.
/// \param ...
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_append_many(Vector* vec, uptr count, void* elt, ...) {
    if (Vector_reserve(vec, vec->length + count)) { return -1; }
    vec->data[vec->length++] = elt;
    va_list args;
    va_start(args, elt);
    for (uptr i = 1; i < count; i++) {
        vec->data[vec->length++] = va_arg(args, void*);
    }
    va_end(args);
    return 0;
}

/// Append an array to a `Vector`.
/// \remark This function performs at most one allocation.
/// \remark The elements of `elts` are copied to the `Vector`'s buffer.
/// \param vec The `Vector` to append to.
/// \param count The number of elements to append.
/// \param elts The array of values to append.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_append_arr(Vector* vec, uptr count, void** elts) {
    if (Vector_reserve(vec, vec->length + count)) { return -1; }
    vec->length += count;
    memcpy(vec->data + vec->length, elts, count * sizeof(void*));
    return 0;
}

/// Reserve at least `capacity` elements of space for a `Vector`.
/// \remark Only one allocation is made.
/// \remark If `capacity` is the same as the `Vector`'s `capacity`, no allocation is made.
/// \remark `capacity` must be greater than the `Vector`'s current `capacity`.
/// \param vec The `Vector` to reserve space in.
/// \param capacity The minimum total amount of space to be reserved.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_reserve(Vector* vec, uptr capacity) {
    assert(capacity >= vec->capacity);
    if (capacity == vec->capacity) { return 0; }
    while (vec->capacity < capacity) {
        vec->capacity *= vec->growth_factor;
    }
    void** tmp = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    if (tmp == NULL) {
        return -1;
    }
    free(vec->data);
    vec->data = tmp;
    memset(vec->data + vec->length, 0, (vec->capacity - vec->length) * sizeof(void*));
    return 0;
}

/// Reserve exactly `capacity` elements of space for a `Vector`.
/// \remark Only one allocation is made.
/// \remark If `capacity` is the same as the `Vector`'s `capacity`, no allocation is made.
/// \remark `capacity` must be greater than the `Vector`'s current `capacity`.
/// \param vec The `Vector` to reserve space in.
/// \param capacity The total amount of space to be reserved.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_reserve_exact(Vector* vec, uptr capacity) {
    assert(capacity >= vec->capacity);
    if (capacity == vec->capacity) { return 0; }
    vec->capacity = capacity;
    void** tmp = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    if (tmp == NULL) { return -1; }
    free(vec->data);
    vec->data = tmp;
    return 0;
}

/// Shrink a `Vector` to fit exactly its elements.
/// \param vec The `Vector` to shrink.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_shrink_to_fit(Vector* vec) {
    vec->capacity = vec->length;
    void** tmp = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    if (tmp == NULL) { return -1; }
    free(vec->data);
    vec->data = tmp;
    return 0;
}
