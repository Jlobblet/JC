/// \file
/// Dynamic array that resizes itself as necessary.

#ifndef JC_JVECTOR_H
#define JC_JVECTOR_H

#include <stdarg.h>
#include <stdbool.h>
#include "jint.h"

typedef struct Vector {
    uptr length;
    uptr capacity;
    uptr growth_factor;
    void** data;
} Vector;

struct Vector_minmax {
    const void *min, *max;
};

typedef void* vector_mapping_t(const void*, ...);
typedef bool vector_filter_t(const void*, ...);
typedef const void* vector_reduce_t(const void*, const void*, ...);
typedef const void* vector_fold_t(const void*, const void*, ...);
typedef int vector_compare_t(const void*, const void*, ...);

iptr Vector_default(Vector* vec);
iptr Vector_init(Vector* vec);
iptr Vector_with_capacity(Vector* vec, uptr capacity);
void Vector_dest(Vector* vec);
iptr Vector_new(Vector* vec, uptr count, void* elt, ...);
iptr Vector_from(Vector* vec, uptr count, void** elts);
iptr Vector_grow(Vector* vec);
iptr Vector_grow_if_full(Vector* vec);
iptr Vector_push_back(Vector* vec, void* elt);
void Vector_pop_back(Vector* vec, void** out);
iptr Vector_append_many(Vector* vec, uptr count, void* elt, ...);
iptr Vector_append_arr(Vector* vec, uptr count, void** elts);
iptr Vector_reserve(Vector* vec, uptr capacity);
iptr Vector_reserve_exact(Vector* vec, uptr capacity);
iptr Vector_shrink_to_fit(Vector* vec);
void Vector_map(Vector* vec, vector_mapping_t mapping, ...);
void Vector_filter(Vector* vec, vector_filter_t predicate, ...);
const void* Vector_reduce(Vector* vec, vector_reduce_t reduce, ...);
const void* Vector_fold(Vector* vec, vector_fold_t fold, const void* initial, ...);
const void* Vector_max(Vector* vec, vector_compare_t compare, ...);
const void* Vector_min(Vector* vec, vector_compare_t compare, ...);
struct Vector_minmax Vector_minmax(Vector* vec, vector_compare_t compare, ...);

#endif //JC_JVECTOR_H
