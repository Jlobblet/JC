#ifndef JC_JBITS_H
#define JC_JBITS_H

#include "jint.h"

typedef uptr BitArray2dBacker;

typedef struct BitArray2d {
    uptr rows, cols;
    uptr **row_starts;
    BitArray2dBacker *data;
} BitArray2d;

static const uptr BITS = (sizeof(BitArray2dBacker) * 8);

static inline uptr get_row_width(BitArray2d *arr) {
    return (arr->cols + BITS - 1) / BITS;
}

static inline uptr get_offset(uptr col) {
    return col % BITS;
}

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

iptr BitArray2d_init(BitArray2d *arr);
void BitArray2d_dest(BitArray2d *arr);
uptr BitArray2d_get(BitArray2d *arr, uptr row, uptr col);
void BitArray2d_set(BitArray2d *arr, uptr row, uptr col, BitArray2dBacker value);
void BitArray2d_on(BitArray2d *arr, uptr row, uptr col);
void BitArray2d_off(BitArray2d *arr, uptr row, uptr col);
void BitArray2d_toggle(BitArray2d *arr, uptr row, uptr col);

#endif //JC_JBITS_H
