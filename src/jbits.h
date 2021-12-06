#ifndef JC_JBITS_H
#define JC_JBITS_H

#include "jint.h"

uptr popcount8(u8 x);
uptr popcount16(u16 x);
uptr popcount32(u32 x);
uptr popcount64(u64 x);

#define popcount(x) _Generic((x),      \
u8 : popcount8,                        \
u16: popcount16,                       \
u32: popcount32,                       \
u64: popcount64)                       \
(x)

uptr pop0count8(u8 x);
uptr pop0count16(u16 x);
uptr pop0count32(u32 x);
uptr pop0count64(u64 x);

#define pop0count(x) _Generic((x),      \
u8 : pop0count8,                        \
u16: pop0count16,                       \
u32: pop0count32,                       \
u64: pop0count64)                       \
(x)

typedef fu8 BitArrayBacker;

static const uptr BitArray_BACKER_BITS = (sizeof(BitArrayBacker) * 8);

typedef struct BitArray {
    uptr length;
    BitArrayBacker* data;
} BitArray;

static inline uptr BitArray_offset(uptr index) {
    return index % BitArray_BACKER_BITS;
}

iptr BitArray_init(BitArray *arr);
void BitArray_dest(BitArray *arr);
uptr BitArray_get(BitArray *arr, uptr index);
void BitArray_set(BitArray *arr, uptr index, BitArrayBacker value);
void BitArray_on(BitArray *arr, uptr index);
void BitArray_off(BitArray *arr, uptr index);
void BitArray_toggle(BitArray *arr, uptr index);
void BitArray_flip(BitArray *arr);
void BitArray_and(BitArray *left, BitArray *right);

typedef uptr BitArray2dBacker;

typedef struct BitArray2d {
    uptr rows, cols;
    uptr **row_starts;
    BitArray2dBacker *data;
} BitArray2d;

static const uptr BitArray2d_BACKER_BITS = (sizeof(BitArray2dBacker) * 8);

static inline uptr BitArray2d_row_width(BitArray2d *arr) {
    return (arr->cols + BitArray2d_BACKER_BITS - 1) / BitArray2d_BACKER_BITS;
}

static inline uptr BitArray2d_offset(uptr col) {
    return col % BitArray2d_BACKER_BITS;
}

iptr BitArray2d_init(BitArray2d *arr);
void BitArray2d_dest(BitArray2d *arr);
uptr BitArray2d_get(BitArray2d *arr, uptr row, uptr col);
void BitArray2d_set(BitArray2d *arr, uptr row, uptr col, BitArray2dBacker value);
void BitArray2d_on(BitArray2d *arr, uptr row, uptr col);
void BitArray2d_off(BitArray2d *arr, uptr row, uptr col);
void BitArray2d_toggle(BitArray2d *arr, uptr row, uptr col);
uptr BitArray2d_count_on(BitArray2d* arr);
uptr BitArray2d_count_off(BitArray2d* arr);
void BitArray2d_flip(BitArray2d* arr);

#endif //JC_JBITS_H
