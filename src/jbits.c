#include <assert.h>
#include <stdlib.h>
#include "jbits.h"

iptr BitArray2d_init(BitArray2d* arr) {
    assert(arr->rows > 0);
    assert(arr->cols > 0);
    arr->row_starts = calloc(arr->rows, sizeof(uptr*));
    if (arr->row_starts == NULL) {
        return -1;
    }

    uptr row_width = get_row_width(arr);
    arr->data = calloc(row_width, sizeof(uptr));
    if (arr->data == NULL) {
        return -1;
    }

    for (uptr i = 0; i < arr->rows; i++) {
        arr->row_starts[i] = &arr->data[i / row_width];
    }

    return 0;
}

void BitArray2d_dest(BitArray2d* arr) {
    free(arr->row_starts);
    free(arr->data);
}

uptr BitArray2d_get(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    uptr row_width = get_row_width(arr);
    return (arr->row_starts[row][col / row_width] >> get_offset(col)) & 1U;
}

void BitArray2d_set(BitArray2d* arr, uptr row, uptr col, uptr value) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    assert(value == 0U || value == 1U);
    uptr row_width = get_row_width(arr);
    arr->row_starts[row][col / get_row_width(arr)] =
            (arr->row_starts[row][col / row_width] & ~(1U << get_offset(col)))
            | (value << get_offset(col));
}

void BitArray2d_on(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / get_row_width(arr)] |= 1U << get_offset(col);
}

void BitArray2d_off(BitArray2d* arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / get_row_width(arr)] &= ~(1U << get_offset(col));
}

void BitArray2d_toggle(BitArray2d *arr, uptr row, uptr col) {
    assert(row < arr->rows);
    assert(col < arr->cols);
    arr->row_starts[row][col / get_row_width(arr)] ^= 1U << get_offset(col);
}
