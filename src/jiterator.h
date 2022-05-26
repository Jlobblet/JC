#ifndef JC_JITERATOR_H
#define JC_JITERATOR_H

#include <stdbool.h>
#include "jvector.h"

typedef struct jiterator jiterator;

typedef bool jiterator_next(jiterator* self);

typedef struct jiterator {
    /// The current element in the iterator.
    void *current;
    /// A function to advance the iterator to the next element.
    jiterator_next *next;
    /// Any data that the iterator needs to keep track of.
    void *state;
} jiterator;

typedef struct pair {
    void *first;
    void *second;
} pair;

void jiterator_collect(jiterator *source, Vector *vec);

void jiterator_malloc_dest(jiterator *self);

void jiterator_vector_reset(jiterator *self);
void jiterator_vector_init(jiterator *self, Vector *vec);

typedef void *jiterator_map_fn(void *element);
typedef void *jiterator_map2_fn(void *a, void *b);
void jiterator_map_init(jiterator *self, jiterator *source, jiterator_map_fn *map);

typedef void jiterator_action_fn(void *element);
void jiterator_action_init(jiterator *self, jiterator *source, jiterator_action_fn *action);

typedef bool jiterator_filter_fn(void *element);
void jiterator_filter_init(jiterator *self, jiterator *source, jiterator_filter_fn *filter);

void jiterator_iota_init(jiterator *self, iptr start, iptr step);
void jiterator_take_init(jiterator *self, jiterator *source, uptr count);
void jiterator_drop_init(jiterator *self, jiterator *source, uptr count);
void jiterator_take_while_init(jiterator *self, jiterator *source, jiterator_filter_fn *filter);
void jiterator_drop_while_init(jiterator *self, jiterator *source, jiterator_filter_fn *filter);
void jiterator_zip_init(jiterator *self, jiterator *source1, jiterator *source2);
void jiterator_enumerate_init(jiterator *self, jiterator *source);
void jiterator_pairwise_init(jiterator *self, jiterator *source);

#endif //JC_JITERATOR_H
