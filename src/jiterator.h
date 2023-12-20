#ifndef JC_JITERATOR_H
#define JC_JITERATOR_H

#include <stdbool.h>
#include "jvector.h"

typedef struct jiterator jiterator;

typedef bool jiterator_next(jiterator* self);

/// A struct that exposes two functions to allow iteration of elements, one at a time.
typedef struct jiterator {
    /// The current element in the iterator.
    void *current;
    /// A function to advance the iterator to the next element. It should also update the value in `current` if applicable.
    jiterator_next *next;
    /// Any data that the iterator needs to keep track of.
    void *state;
} jiterator;

typedef struct pair {
    void *first;
    void *second;
} pair;

void jiterator_collect(jiterator *source, Vector *vec);

void jiterator_alloc_dest(jiterator *self);

void jiterator_vector_reset(jiterator *self);
void jiterator_vector_init(jiterator *self, Vector *vec);

typedef void *jiterator_map_fn(void *element);
typedef void *jiterator_map2_fn(void *a, void *b);
void jiterator_map_init(jiterator *self, jiterator *source, jiterator_map_fn *map);

typedef void jiterator_action_fn(void *element);
void jiterator_action_init(jiterator *self, jiterator *source, jiterator_action_fn *action);

typedef bool jiterator_predicate_fn(void *element);
void jiterator_filter_init(jiterator *self, jiterator *source, jiterator_predicate_fn *filter);

typedef void *jiterator_reduce_fn(void *acc, void *elt);
bool jiterator_reduce(jiterator *source, jiterator_reduce_fn *reduce, void **result);

typedef void *jiterator_fold_fn(void *acc, void *elt);
void *jiterator_fold(jiterator *source, jiterator_fold_fn *fold, void *initial);
void jiterator_scan_init(jiterator *self, jiterator *source, jiterator_fold_fn *scan, void *initial);

typedef int jiterator_compare_fn(void *a, void *b);
bool jiterator_min_max(jiterator *source, jiterator_compare_fn *compare, pair *result);
bool jiterator_min(jiterator *source, jiterator_compare_fn *compare, void **result);
bool jiterator_max(jiterator *source, jiterator_compare_fn *compare, void **result);
bool jiterator_all(jiterator *source, jiterator_predicate_fn *predicate);
bool jiterator_any(jiterator *source, jiterator_predicate_fn *predicate);

void jiterator_iota_init(jiterator *self, iptr start, iptr step);
void jiterator_take_init(jiterator *self, jiterator *source, uptr count);
void jiterator_drop_init(jiterator *self, jiterator *source, uptr count);
void jiterator_take_while_init(jiterator *self, jiterator *source, jiterator_predicate_fn *filter);
void jiterator_drop_while_init(jiterator *self, jiterator *source, jiterator_predicate_fn *filter);
void jiterator_zip_with_init(jiterator *self, jiterator *source1, jiterator *source2, jiterator_map2_fn *zipper);
void jiterator_zip_init(jiterator *self, jiterator *source1, jiterator *source2);
void jiterator_enumerate_init(jiterator *self, jiterator *source);
void jiterator_pairwise_init(jiterator *self, jiterator *source);

typedef void *jiterator_generator_fn(void);
void jiterator_once_init(jiterator *self, void *value);
void jiterator_once_with_init(jiterator *self, jiterator_generator_fn *action);
void jiterator_repeat_init(jiterator *self, void *action);
void jiterator_repeat_with_init(jiterator *self, jiterator_generator_fn *action);

void jiterator_concat_init(jiterator *self, jiterator *source1, jiterator *source2);

#endif //JC_JITERATOR_H
