/// \file
/// Bit manipulation

#include <assert.h>
#include <stdlib.h>
#include "jbits.h"

#define popcount_body \
uptr sum = 0;         \
while(x) {            \
    sum += x & 1U;    \
    x >>= 1;          \
}                     \
return sum;

#define pop0count_body \
x = ~x;                \
popcount_body

uptr popcount8(u8 x) {
    popcount_body
}

uptr popcount16(u16 x) {
    popcount_body
}

uptr popcount32(u32 x) {
    popcount_body
}

uptr popcount64(u64 x) {
    popcount_body
}

uptr pop0count8(u8 x) {
    pop0count_body
}

uptr pop0count16(u16 x) {
    pop0count_body
}

uptr pop0count32(u32 x) {
    pop0count_body
}

uptr pop0count64(u64 x) {
    pop0count_body
}

iptr BitArray_init(BitArray *arr) {
    assert(arr->length > 0);
    arr->data = calloc((arr->length + BitArray_BACKER_BITS - 1) / BitArray_BACKER_BITS, sizeof(BitArrayBacker));
    if (arr->data == NULL) {
        return -1;
    }
    return 0;
}

void BitArray_dest(BitArray *arr) {
    free(arr->data);
}

uptr BitArray_get(BitArray *arr, uptr index) {
    assert(index < arr->length);
    return (arr->data[index / BitArray_BACKER_BITS] >> BitArray_offset(index)) & 1U;
}

void BitArray_set(BitArray *arr, uptr index, BitArrayBacker value) {
    assert(index < arr->length);
    assert(value == 0U || value == 1U);
    arr->data[index / BitArray_BACKER_BITS] =
            (arr->data[index / BitArray_BACKER_BITS] & ~(1U << BitArray_offset(index)))
            | (value << BitArray_offset(index));
}

void BitArray_on(BitArray *arr, uptr index) {
    assert(index < arr->length);
    arr->data[index / BitArray_BACKER_BITS] |= 1U << BitArray_offset(index);
}

void BitArray_off(BitArray *arr, uptr index) {
    assert(index < arr->length);
    arr->data[index / BitArray_BACKER_BITS] &= ~(1U << BitArray_offset(index));
}

void BitArray_toggle(BitArray *arr, uptr index) {
    assert(index < arr->length);
    arr->data[index / BitArray_BACKER_BITS] ^= 1U << BitArray_offset(index);
}

void BitArray_flip(BitArray *arr) {
    for (uptr i = 0; i < arr->length / BitArray_BACKER_BITS; i++) {
        arr->data[i] = ~arr->data[i];
    }
    for (uptr i = BitArray_BACKER_BITS * (arr->length / BitArray_BACKER_BITS); i < arr->length; i++) {
        BitArray_off(arr, i);
    }
}

void BitArray_and(BitArray *left, BitArray *right) {
    assert(left->length == right->length);
    for (uptr i = 0; i < (left->length + BitArray_BACKER_BITS - 1) / BitArray_BACKER_BITS; i++) {
        left->data[i] &= right->data[i];
    }
}

uptr BitArray_popcount(BitArray *arr) {
    uptr sum = 0;
    for (uptr i = 0; i < (arr->length + BitArray_BACKER_BITS - 1) / BitArray_BACKER_BITS; i++) {
        sum += popcount(arr->data[i]);
    }
    return sum;
}

/// Initialise a `BitArray2d` struct.
///
/// This function allocates two arrays - one to store `row_starts`, which contains
/// pointers to the start of each row, and `data`, which contains each row.
/// \param arr The array to initialise.
/// \return On success, `0`. On error, `-1`, and `errno` is set to indicate the error.
iptr BitArray2d_init(BitArray2d* arr) {
    assert(arr->rows > 0);
    assert(arr->cols > 0);
    arr->row_starts = calloc(arr->rows, sizeof(BitArray2dBacker*));
    if (arr->row_starts == NULL) {
        return -1;
    }

    uptr row_width = BitArray2d_row_width(arr);
    arr->data = calloc(row_width, sizeof(BitArray2dBacker));
    if (arr->data == NULL) {
        return -1;
    }

    for (uptr i = 0; i < arr->rows; i++) {
        arr->row_starts[i] = &arr->data[i / row_width];
    }

    return 0;
}

/// Handle cleaning up a `BitArray2d`.
///
/// \param arr The `BitArray2d` to clean up (by pointer).
void BitArray2d_dest(BitArray2d* arr) {
    free(arr->row_starts);
    free(arr->data);
}

/// Get the value in a given cell of a `BitArray2d`.
///
/// \param arr The `BitArray2d` to retrieve a bit from.
/// \param row The row index (0-based) to retrieve the bit from.
/// \param col The column index (0-based) to retrieve the bit from.
/// \return `0` or `1`.
BitArray2dBacker BitArray2d_get(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    uptr row_width = BitArray2d_row_width(arr);
    return (arr->row_starts[row][col / row_width] >> BitArray2d_offset(col)) & 1U;
}

/// Set the value in a given cell of a `BitArray2d`.
///
/// \param arr The `BitArray2d` to set a bit in.
/// \param row The row index (0-based) of the bit to set.
/// \param col The column index (0-based) of the bit to set.
/// \param value The value to set at the given coordinates, `0` or `1`.
void BitArray2d_set(BitArray2d* arr, uptr row, uptr col, uptr value) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    assert(value == 0U || value == 1U);
    uptr row_width = BitArray2d_row_width(arr);
    arr->row_starts[row][col / row_width] =
            (arr->row_starts[row][col / row_width] & ~(1U << BitArray2d_offset(col)))
            | (value << BitArray2d_offset(col));
}

/// Set the value in a given cell of a `BitArray2d` to `1`.
/// \param arr The `BitArray2d` to set a bit in.
/// \param row The row index (0-based) of the bit to set.
/// \param col The column index (0-based) of the bit to set.
void BitArray2d_on(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / BitArray2d_row_width(arr)] |= 1U << BitArray2d_offset(col);
}

/// Set the value in a given cell of a `BitArray2d` to `0`.
/// \param arr The `BitArray2d` to set a bit in.
/// \param row The row index (0-based) of the bit to set.
/// \param col The column index (0-based) of the bit to set.
void BitArray2d_off(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / BitArray2d_row_width(arr)] &= ~(1U << BitArray2d_offset(col));
}

/// Toggle value in a given cell of a `BitArray2d`.
///
/// Cells with a `0` in them will be set to `1`, and cells with a `1` in them
/// will be set to `0`.
/// \param arr The `BitArray2d` to set a bit in.
/// \param row The row index (0-based) of the bit to set.
/// \param col The column index (0-based) of the bit to set.
void BitArray2d_toggle(BitArray2d *arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / BitArray2d_row_width(arr)] ^= 1U << BitArray2d_offset(col);
}

uptr BitArray2d_count_on(BitArray2d* arr) {
    uptr sum = 0;
    for (uptr i = 0; i < arr->rows; i++) {
        for (uptr j = 0; j < BitArray2d_row_width(arr)
                             #ifndef NDEBUG
                             - 1
#endif //NDEBUG
        ; j++) {
            sum += popcount(arr->row_starts[i][j]);
        }
#ifndef NDEBUG
        for (uptr c = BitArray2d_BACKER_BITS * (arr->cols / BitArray2d_BACKER_BITS); c < arr->cols; c++) {
            sum += BitArray2d_get(arr, i, c);
        }
#endif // NDEBUG
    }
    return sum;
}

uptr BitArray2d_count_off(BitArray2d* arr) {
    uptr sum = 0;
    for (uptr i = 0; i < arr->rows; i++) {
        for (uptr j = 0; j < BitArray2d_row_width(arr)
                             #ifndef NDEBUG
                             - 1
#endif // NDEBUG
        ; j++) {
            sum += pop0count(arr->row_starts[i][j]);
        }
#ifndef NDEBUG
        for (uptr c = BitArray2d_BACKER_BITS * (arr->cols / BitArray2d_BACKER_BITS); c < arr->cols; c++) {
            sum += ~BitArray2d_get(arr, i, c);
        }
#endif
    }
    return sum;
}

void BitArray2d_flip(BitArray2d* arr) {
    for (uptr i = 0; i < arr->rows; i++) {
        for (uptr j = 0; j < BitArray2d_row_width(arr)
                             #ifndef NDEBUG
                             - 1
#endif // NDEBUG
        ; j++) {
            arr->row_starts[i][j] = ~arr->row_starts[i][j];
        }
#ifndef NDEBUG
        for (uptr c = BitArray2d_BACKER_BITS * (arr->cols / BitArray2d_BACKER_BITS); c < arr->cols; c++) {
            BitArray2d_off(arr, i, c);
        }
#endif // NDEBUG
    }
}
