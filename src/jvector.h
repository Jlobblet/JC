/// \file
/// Dynamic array that resizes itself as necessary.

#ifndef JC_JVECTOR_H
#define JC_JVECTOR_H

#include <stdarg.h>
#include "jint.h"

typedef struct Vector {
    uptr length;
    uptr capacity;
    uptr growth_factor;
    void** data;
} Vector;

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

#endif //JC_JVECTOR_H
