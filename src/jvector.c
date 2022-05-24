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
    vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    if (vec->data == NULL) {
        return -1;
    }
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
    vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    if (vec->data == NULL) { return -1; }
    return 0;
}

/// Shrink a `Vector` to fit exactly its elements.
/// \param vec The `Vector` to shrink.
/// \return On success, `0`. On error, `-1` and `errno` is set to indicate the error.
iptr Vector_shrink_to_fit(Vector* vec) {
    vec->capacity = vec->length;
    vec->data = (void**)realloc(vec->data, vec->capacity * sizeof(void*));
    if (vec->data == NULL) { return -1; }
    return 0;
}

/// Map a function over a `Vector`.
/// \remark The function is called for each element in the `Vector`, in order.
/// \param vec The `Vector` to map over.
/// \param mapping The function to call for each element.
/// \param ... Extra arguments to pass to `mapping`.
void Vector_map(Vector* vec, vector_mapping_t mapping, ...) {
    va_list args;
    va_start(args, mapping);
    for (uptr i = 0; i < vec->length; i++) {
        vec->data[i] = mapping(vec->data[i], args);
    }
    va_end(args);
}

/// Filter a `Vector` by removing elements for which the predicate returns `false`.
/// \remark The predicate is called for each element in the `Vector`, in reverse order.
/// \param vec The `Vector` to predicate.
/// \param predicate The predicate to call for each element.
/// \param ... Extra arguments to pass to `predicate`.
void Vector_filter(Vector* vec, vector_filter_t predicate, ...) {
    va_list args;
    va_start(args, predicate);
    for (uptr i = 0; i < vec->length; i++) {
        if (predicate(vec->data[i], args)) { continue; }
        // Shift all elements after `i` to the left.
        // This is done in reverse order to avoid shifting elements that are not yet visited.
        // This removes the ith element.
        memcpy(vec->data + i, vec->data + i + 1, (vec->length - i - 1) * sizeof(void*));
        // Decrement the length.
        vec->length--;
        // Decrement `i` to account for the shift.
        i--;
    }
    va_end(args);
}

/// Apply a function to each element of a `Vector`, threading an accumulator value through.
/// Apply the function to the first two elements of the vector.
/// Then feed the result of the first application into the second application, and so on.
/// \param vec The `Vector` to apply the function to.
/// \param reduce The function to apply to each element.
/// \param ... Extra arguments to pass to `reduce`.
/// \return The final value of the accumulator.
const void* Vector_reduce(Vector* vec, vector_reduce_t reduce, ...) {
    assert(vec->length > 0);
    va_list args;
    va_start(args, reduce);
    const void* result = vec->data[0];
    for (uptr i = 1; i < vec->length; i++) {
        result = reduce(result, vec->data[i], args);
    }
    va_end(args);
    return result;
}

/// Apply a function to each element of a `Vector`, threading an accumulator value through.
/// Apply the function to the initial value and the first element of the vector.
/// Then feed the result of the first application into the second application, and so on.
/// \param vec The `Vector` to apply the function to.
/// \param fold The function to apply to each element.
/// \param initial The initial value to thread through.
/// \param ... Extra arguments to pass to `fold`.
/// \return The final value of the accumulator.
const void* Vector_fold(Vector* vec, vector_fold_t fold, const void* initial, ...) {
    va_list args;
    va_start(args, initial);
    for (uptr i = 0; i < vec->length; i++) {
        initial = fold(initial, vec->data[i], args);
    }
    va_end(args);
    return initial;
}

static const void* keep_greater(const void* a, const void* b, ...) {
    va_list args;
    va_start(args, b);
    vector_compare_t* compare = va_arg(args, vector_compare_t*);
    return compare(a, b, args) < 0 ? b : a;
}

static const void* keep_lesser(const void* a, const void* b, ...) {
    va_list args;
    va_start(args, b);
    vector_compare_t* compare = va_arg(args, vector_compare_t*);
    return compare(a, b, args) >= 0 ? b : a;
}

// We violate the `const` rule here because it's the easiest way to pass around a struct with two pointers.
static const void* keep_min_and_max(const void* a, const void* b, ...) {
    struct Vector_minmax* minmax = (struct Vector_minmax*)a;
    va_list args;
    va_start(args, b);
    vector_compare_t* compare = va_arg(args, vector_compare_t*);
    if (compare(b, minmax->min, args) < 0) {
        minmax->min = b;
    }
    if (compare(b, minmax->max, args) > 0) {
        minmax->max = b;
    }
    return minmax;
}

/// Find the maximum element of a `Vector`.
/// \param vec The `Vector` to find the maximum element of.
/// \param compare The function to compare elements.
/// \remark `compare` should return a negative value if the first argument is less than the second,
/// a positive value if the first argument is greater than the second, and 0 if the arguments are equal.
/// \remark If two elements are equal, the first one is retained.
/// \return The maximum element of `vec`.
const void* Vector_max(Vector* vec, vector_compare_t compare, ...) {
    va_list args;
    va_start(args, compare);
    const void* max = Vector_reduce(vec, keep_greater, compare, args);
    va_end(args);
    return max;
}

/// Find the minimum element of a `Vector`.
/// \param vec The `Vector` to find the minimum element of.
/// \param compare The function to compare elements.
/// \remark `compare` should return a negative value if the first argument is less than the second,
/// a positive value if the first argument is greater than the second, and 0 if the arguments are equal.
/// \remark If two elements are equal, the first one is retained.
/// \return The minimum element of `vec`.
const void* Vector_min(Vector* vec, vector_compare_t compare, ...) {
    va_list args;
    va_start(args, compare);
    const void* min = Vector_reduce(vec, keep_lesser, compare, args);
    va_end(args);
    return min;
}

struct Vector_minmax Vector_minmax(Vector* vec, vector_compare_t compare, ...) {
    va_list args;
    va_start(args, compare);
    struct Vector_minmax minmax = {};
    minmax = *(struct Vector_minmax*)Vector_fold(vec, keep_min_and_max, &minmax, compare, args);
    va_end(args);
    return minmax;
}
