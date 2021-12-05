/// \file
/// Bit manipulation

#include <assert.h>
#include <stdlib.h>
#include "jbits.h"

#define popcount_body \
uptr sum = 0;         \
while(x) {            \
    sum += sum & 1U;  \
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

    uptr row_width = get_row_width(arr);
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
    uptr row_width = get_row_width(arr);
    return (arr->row_starts[row][col / row_width] >> get_offset(col)) & 1U;
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
    uptr row_width = get_row_width(arr);
    arr->row_starts[row][col / get_row_width(arr)] =
            (arr->row_starts[row][col / row_width] & ~(1U << get_offset(col)))
            | (value << get_offset(col));
}

/// Set the value in a given cell of a `BitArray2d` to `1`.
/// \param arr The `BitArray2d` to set a bit in.
/// \param row The row index (0-based) of the bit to set.
/// \param col The column index (0-based) of the bit to set.
void BitArray2d_on(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / get_row_width(arr)] |= 1U << get_offset(col);
}

/// Set the value in a given cell of a `BitArray2d` to `0`.
/// \param arr The `BitArray2d` to set a bit in.
/// \param row The row index (0-based) of the bit to set.
/// \param col The column index (0-based) of the bit to set.
void BitArray2d_off(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / get_row_width(arr)] &= ~(1U << get_offset(col));
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
    arr->row_starts[row][col / get_row_width(arr)] ^= 1U << get_offset(col);
}

uptr BitArray2d_count_on(BitArray2d* arr) {
    uptr sum = 0;
    for (uptr i = 0; i < arr->rows; i++) {
        for (uptr j = 0; j < get_row_width(arr)
#ifndef NDEBUG
        - 1
#endif //NDEBUG
        ; j++) {
            sum += popcount(arr->row_starts[i][j]);
        }
#ifndef NDEBUG
        for (uptr c = BITS * (arr->cols / BITS); c < arr->cols; c++) {
            sum += BitArray2d_get(arr, i, c);
        }
#endif // NDEBUG
    }
    return sum;
}

uptr BitArray2d_count_off(BitArray2d* arr) {
    uptr sum = 0;
    for (uptr i = 0; i < arr->rows; i++) {
        for (uptr j = 0; j < get_row_width(arr)
#ifndef NDEBUG
        - 1
#endif // NDEBUG
        ; j++) {
            sum += pop0count(arr->row_starts[i][j]);
        }
#ifndef NDEBUG
        for (uptr c = BITS * (arr->cols / BITS); c < arr->cols; c++) {
            sum += BitArray2d_get(arr, i, c);
        }
#endif
    }
    return sum;
}

void BitArray2d_flip(BitArray2d* arr) {
    for (uptr i = 0; i < arr->rows; i++) {
        for (uptr j = 0; j < get_row_width(arr)
#ifndef NDEBUG
        - 1
#endif // NDEBUG
        ; j++) {
            arr->row_starts[i][j] = ~arr->row_starts[i][j];
        }
#ifndef NDEBUG
        for (uptr c = BITS * (arr->cols / BITS); c < arr->cols; c++) {
            BitArray2d_off(arr, i, c);
        }
#endif // NDEBUG
    }
}
