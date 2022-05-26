#include <stdlib.h>
#include "jiterator.h"
#include "jvector.h"

void jiterator_collect(jiterator *source, Vector *vec) {
    Vector_default(vec);
    while (source->next(source)) {
        Vector_grow_if_full(vec);
        vec->data[vec->length] = source->current;
        vec->length++;
    }
}

void jiterator_alloc_dest(jiterator *self) {
    free(self->state);
}

typedef struct jiterator_vector_state {
    Vector *vec;
    uptr current_index;
    bool started;
} jiterator_vector_state;

bool jiterator_vector_next(jiterator *self) {
    jiterator_vector_state *state = self->state;

    if (!state->started) {
        state->started = true;
    } else {
        state->current_index++;
    }

    if (state->current_index >= state->vec->length) {
        return false;
    }

    self->current = state->vec->data[state->current_index];
    return true;
}

void jiterator_vector_reset(jiterator *self) {
    jiterator_vector_state *state = self->state;
    state->current_index = 0;
    state->started = false;
}

void jiterator_vector_init(jiterator *self, Vector *vec) {
    self->state = calloc(1, sizeof(jiterator_vector_state));
    jiterator_vector_state* state = (jiterator_vector_state*) self->state;
    state->vec = vec;
    state->current_index = 0;
    self->next = jiterator_vector_next;
}

typedef struct jiterator_map_state {
    jiterator_map_fn *map;
    jiterator *it;
} jiterator_map_state;

bool map_iterator_next(jiterator *self) {
    jiterator_map_state *state = (jiterator_map_state *)self->state;

    if (!state->it->next(state->it)) {
        return false;
    }

    self->current = state->map(state->it->current);
    return true;
}

void jiterator_map_init(jiterator *self, jiterator *source, jiterator_map_fn *map) {
    self->state = calloc(1, sizeof(jiterator_map_state));
    jiterator_map_state* state = (jiterator_map_state*) self->state;
    state->map = map;
    state->it = source;
    self->next = map_iterator_next;
}

typedef struct jiterator_action_state {
    jiterator *it;
    jiterator_action_fn *action;
} jiterator_action_state;

bool jiterator_action_next(jiterator *self) {
    jiterator_action_state *state = (jiterator_action_state *)self->state;

    if (!state->it->next(state->it)) {
        return false;
    }

    state->action(state->it->current);
    self->current = state->it->current;
    return true;
}

void jiterator_action_init(jiterator *self, jiterator *source, jiterator_action_fn *action) {
    self->state = calloc(1, sizeof(jiterator_action_state));
    jiterator_action_state* state = (jiterator_action_state*) self->state;
    state->it = source;
    state->action = action;
    self->next = jiterator_action_next;
}

typedef struct jiterator_filter_state {
    jiterator_predicate_fn *filter;
    jiterator *it;
} jiterator_filter_state;

bool filter_iterator_next(jiterator *self) {
    jiterator_filter_state *state = (jiterator_filter_state *)self->state;

    while (1) {
        if (!state->it->next(state->it)) {
            return false;
        }

        if (state->filter(state->it->current)) {
            self->current = state->it->current;
            return true;
        }
    }
}

void jiterator_filter_init(jiterator *self, jiterator *source, jiterator_predicate_fn *filter) {
    self->state = calloc(1, sizeof(jiterator_filter_state));
    jiterator_filter_state* state = (jiterator_filter_state*) self->state;
    state->filter = filter;
    state->it = source;
    self->next = filter_iterator_next;
}

bool jiterator_reduce(jiterator *source, jiterator_reduce_fn *reduce, void **result) {
    if (!source->next(source)) {
        return false;
    }
    *result = source->current;
    while (source->next(source)) {
        *result = reduce(*result, source->current);
    }
    return true;
}

void *jiterator_fold(jiterator *source, jiterator_fold_fn *fold, void *initial) {
    while (source->next(source)) {
        initial = fold(initial, source->current);
    }
    return initial;
}

typedef struct jiterator_scan_state {
    jiterator_fold_fn *scan;
    jiterator *it;
    void *accumulator;
} jiterator_scan_state;

bool jiterator_scan_next(jiterator *self) {
    jiterator_scan_state *state = (jiterator_scan_state *)self->state;

    if (!state->it->next(state->it)) {
        return false;
    }

    state->accumulator = state->scan(state->accumulator, state->it->current);
    self->current = state->accumulator;
    return true;
}

void jiterator_scan_init(jiterator *self, jiterator *source, jiterator_fold_fn *scan, void *initial) {
    self->state = calloc(1, sizeof(jiterator_scan_state));
    jiterator_scan_state* state = (jiterator_scan_state*) self->state;
    state->scan = scan;
    state->it = source;
    state->accumulator = initial;
    self->next = jiterator_scan_next;
}

bool jiterator_min_max(jiterator *source, jiterator_compare_fn *compare, pair *result) {
    if (!source->next(source)) {
        return false;
    }
    result->first = source->current;
    result->second = source->current;
    while (source->next(source)) {
        if (compare(source->current, result->first) < 0) {
            result->first = source->current;
        }
        if (compare(source->current, result->second) > 0) {
            result->second = source->current;
        }
    }
    return true;
}

bool jiterator_min(jiterator *source, jiterator_compare_fn *compare, void **result) {
    pair pair;
    bool ret;
    ret = jiterator_min_max(source, compare, &pair);
    if (ret) {
        *result = pair.first;
    }
    return ret;
}

bool jiterator_max(jiterator *source, jiterator_compare_fn *compare, void **result) {
    pair pair;
    bool ret;
    ret = jiterator_min_max(source, compare, &pair);
    if (ret) {
        *result = pair.second;
    }
    return ret;
}

bool jiterator_all(jiterator *source, jiterator_predicate_fn *predicate) {
    while (source->next(source)) {
        if (!predicate(source->current)) {
            return false;
        }
    }
    return true;
}

bool jiterator_any(jiterator *source, jiterator_predicate_fn *predicate) {
    while (source->next(source)) {
        if (predicate(source->current)) {
            return true;
        }
    }
    return false;
}

typedef struct jiterator_iota_state {
    iptr current;
    iptr step;
    bool started;
} jiterator_iota_state;

bool jiterator_iota_next(jiterator *self) {
    jiterator_iota_state *state = (jiterator_iota_state *)self->state;

    if (!state->started) { state->started = true; }
    else { state->current += state->step; }

    self->current = (void *)state->current;
    return true;
}

void jiterator_iota_init(jiterator *self, iptr start, iptr step) {
    self->state = calloc(1, sizeof(jiterator_iota_state));
    jiterator_iota_state* state = (jiterator_iota_state*) self->state;
    state->current = start;
    state->step = step;
    state->started = false;
    self->next = jiterator_iota_next;
}

typedef struct jiterator_take_state {
    jiterator *it;
    uptr remaining;
} jiterator_take_state;

bool jiterator_take_next(jiterator *self) {
    jiterator_take_state *state = (jiterator_take_state *)self->state;
    if (state->remaining == 0) {
        return false;
    }
    state->remaining--;
    if (state->it->next(state->it)) {
        self->current = state->it->current;
        return true;
    } else {
        return false;
    }
}

void jiterator_take_init(jiterator *self, jiterator *source, uptr count) {
    self->state = calloc(1, sizeof(jiterator_take_state));
    jiterator_take_state* state = (jiterator_take_state*) self->state;
    state->it = source;
    state->remaining = count;
    self->current = state->it->current;
    self->next = jiterator_take_next;
}

typedef jiterator_take_state jiterator_drop_state;

bool jiterator_drop_next(jiterator *self) {
    jiterator_drop_state *state = (jiterator_drop_state *)self->state;

    while (state->remaining > 0) {
        if (!state->it->next(state->it)) {
            return false;
        }
        state->remaining--;
    }

    if (state->it->next(state->it)) {
        self->current = state->it->current;
        return true;
    } else {
        return false;
    }
}

void jiterator_drop_init(jiterator *self, jiterator *source, uptr count) {
    self->state = calloc(1, sizeof(jiterator_drop_state));
    jiterator_drop_state* state = (jiterator_drop_state*) self->state;
    state->it = source;
    state->remaining = count;
    self->current = state->it->current;
    self->next = jiterator_drop_next;
}

typedef struct jiterator_take_while_state {
    jiterator_predicate_fn *filter;
    jiterator *it;
    bool finished;
} jiterator_take_while_state;

bool jiterator_take_while_next(jiterator *self) {
    jiterator_take_while_state *state = (jiterator_take_while_state *)self->state;
    if (state->finished) { return false; }
    if (!state->it->next(state->it)) {
        state->finished = true;
        return false;
    }
    if (state->filter(state->it->current)) {
        self->current = state->it->current;
        return true;
    } else {
        state->finished = true;
        return false;
    }
}

void jiterator_take_while_init(jiterator *self, jiterator *source, jiterator_predicate_fn *filter) {
    self->state = calloc(1, sizeof(jiterator_take_while_state));
    jiterator_take_while_state* state = (jiterator_take_while_state*) self->state;
    state->filter = filter;
    state->it = source;
    state->finished = false;
    self->next = jiterator_take_while_next;
}

typedef jiterator_take_while_state jiterator_drop_while_state;

bool jiterator_drop_while_next(jiterator *self) {
    // NB here "finished" means "finished dropping"
    jiterator_drop_while_state *state = (jiterator_drop_while_state *)self->state;
    while (!state->finished) {
        if (!state->it->next(state->it)) {
            state->finished = true;
            return false;
        }
        if (!state->filter(state->it->current)) {
            self->current = state->it->current;
            state->finished = true;
            return true;
        }
    }

    if (state->it->next(state->it)) {
        self->current = state->it->current;
        return true;
    } else {
        return false;
    }
}

void jiterator_drop_while_init(jiterator *self, jiterator *source, jiterator_predicate_fn *filter) {
    self->state = calloc(1, sizeof(jiterator_drop_while_state));
    jiterator_drop_while_state* state = (jiterator_drop_while_state*) self->state;
    state->filter = filter;
    state->it = source;
    state->finished = false;
    self->next = jiterator_drop_while_next;
}

typedef struct jiterator_zip_state {
    jiterator *source1;
    jiterator *source2;
    pair current;
} jiterator_zip_state;

bool jiterator_zip_next(jiterator *self) {
    jiterator_zip_state *state = (jiterator_zip_state *)self->state;
    if (!state->source1->next(state->source1)) {
        return false;
    }
    if (!state->source2->next(state->source2)) {
        return false;
    }
    state->current.first = state->source1->current;
    state->current.second = state->source2->current;
    self->current = &state->current;
    return true;
}

void jiterator_zip_init(jiterator *self, jiterator *source1, jiterator *source2) {
    self->state = calloc(1, sizeof(jiterator_zip_state));
    jiterator_zip_state* state = (jiterator_zip_state*) self->state;
    state->source1 = source1;
    state->source2 = source2;
    state->current.first = NULL;
    state->current.second = NULL;
    self->next = jiterator_zip_next;
}

typedef struct jiterator_enumerate_state {
    jiterator *source;
    uptr index;
    pair current;
    bool started;
} jiterator_enumerate_state;

bool jiterator_enumerate_next(jiterator *self) {
    jiterator_enumerate_state *state = (jiterator_enumerate_state *)self->state;
    if (!state->source->next(state->source)) {
        return false;
    }
    if (!state->started) {
        state->started = true;
    } else {
        state->index++;
    }

    state->current.first = (void *)state->index;
    state->current.second = state->source->current;
    self->current = &state->current;

    return true;
}

void jiterator_enumerate_init(jiterator *self, jiterator *source) {
    self->state = calloc(1, sizeof(jiterator_enumerate_state));
    jiterator_enumerate_state* state = (jiterator_enumerate_state*) self->state;
    state->source = source;
    state->index = 0;
    state->current.first = NULL;
    state->current.second = NULL;
    state->started = false;
    self->next = jiterator_enumerate_next;
}

typedef struct jiterator_pairwise_state {
    jiterator *source;
    pair current;
    bool started;
} jiterator_pairwise_state;

bool jiterator_pairwise_next(jiterator *self) {
    jiterator_pairwise_state *state = (jiterator_pairwise_state *)self->state;
    if (!state->started) {
        state->started = true;
        if (!state->source->next(state->source)) {
            return false;
        }
        state->current.first = state->source->current;
    }

    if (!state->source->next(state->source)) {
        return false;
    }
    state->current.first = state->current.second;
    state->current.second = state->source->current;
    self->current = &state->current;
    return true;
}

void jiterator_pairwise_init(jiterator *self, jiterator *source) {
    self->state = calloc(1, sizeof(jiterator_pairwise_state));
    jiterator_pairwise_state* state = (jiterator_pairwise_state*) self->state;
    state->source = source;
    state->current.first = NULL;
    state->current.second = NULL;
    state->started = false;
    self->next = jiterator_pairwise_next;
}
