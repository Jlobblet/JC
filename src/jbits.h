#ifndef JC_JBITS_H
#define JC_JBITS_H

#include "jint.h"

typedef struct BitArray2d {
    uptr rows, cols;
    uptr **row_starts;
    uptr *data;
} BitArray2d;

static const uptr BITS = (sizeof(uptr) * 8);

static inline uptr get_row_width(BitArray2d* arr) {
    return (arr->cols + BITS - 1) / BITS;
}

static inline uptr get_offset(uptr col) {
    return col % BITS;
}

iptr BitArray2d_init(BitArray2d *arr);

void BitArray2d_dest(BitArray2d *arr);

uptr BitArray2d_get(BitArray2d *arr, uptr row, uptr col);

void BitArray2d_set(BitArray2d *arr, uptr row, uptr col, uptr value);

void BitArray2d_on(BitArray2d *arr, uptr row, uptr col);

void BitArray2d_off(BitArray2d *arr, uptr row, uptr col);

void BitArray2d_toggle(BitArray2d *arr, uptr row, uptr col);

#endif //JC_JBITS_H
