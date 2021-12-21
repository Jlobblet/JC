#ifndef JC_JARRAY_H
#define JC_JARRAY_H

#include <stdarg.h>
#include "jint.h"

typedef fi64 NDArrayBacker;

typedef struct NDArray {
    uptr dims;
    uptr *size;
    NDArrayBacker *data;
} NDArray;

typedef NDArrayBacker(*NDArray_mapping_t)(NDArrayBacker);
typedef NDArrayBacker(*NDArray_map2ing_t)(NDArrayBacker, NDArrayBacker);

uptr NDArray_total_size(NDArray* arr);
iptr NDArray_init(NDArray* arr);
void NDArray_dest(NDArray* arr);
NDArrayBacker* NDArray_ptr(NDArray* arr, uptr index, ...);
NDArrayBacker NDArray_get(NDArray* arr, uptr index, ...);
void NDArray_set(NDArray* arr, NDArrayBacker value, uptr index, ...);
void NDArray_map(NDArray* arr, NDArray_mapping_t mapping);
void NDArray_add(NDArray* arr, NDArrayBacker value);
void NDArray_sub(NDArray* arr, NDArrayBacker value);
void NDArray_mul(NDArray* arr, NDArrayBacker value);
void NDArray_div(NDArray* arr, NDArrayBacker value);
void NDArray_and(NDArray* arr, NDArrayBacker value);
void NDArray_or(NDArray* arr, NDArrayBacker value);
void NDArray_xor(NDArray* arr, NDArrayBacker value);
void NDArray_map2(NDArray* left, NDArray* right, NDArray_map2ing_t map2ing);
void NDArray_arr_add(NDArray* left, NDArray* right);
void NDArray_arr_sub(NDArray* left, NDArray* right);
void NDArray_arr_mul(NDArray* left, NDArray* right);
void NDArray_arr_div(NDArray* left, NDArray* right);
void NDArray_arr_and(NDArray* left, NDArray* right);
void NDArray_arr_or(NDArray* left, NDArray* right);
void NDArray_arr_xor(NDArray* left, NDArray* right);

#endif //JC_JARRAY_H
