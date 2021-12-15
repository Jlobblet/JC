#ifndef JC_JARRAY_H
#define JC_JARRAY_H

#include <stdarg.h>
#include "jint.h"

typedef fi64 NDArray_Backer;

typedef struct NDArray {
    uptr dims;
    uptr *size;
    NDArray_Backer *data;
} NDArray;

typedef NDArray_Backer(*NDArray_mapping_t)(NDArray_Backer);
typedef NDArray_Backer(*NDArray_map2ing_t)(NDArray_Backer, NDArray_Backer);

uptr NDArray_total_size(NDArray* arr);
iptr NDArray_init(NDArray* arr);
void NDArray_dest(NDArray* arr);
NDArray_Backer* NDArray_ptr(NDArray* arr, uptr index, ...);
NDArray_Backer NDArray_get(NDArray* arr, uptr index, ...);
void NDArray_set(NDArray* arr, NDArray_Backer value, uptr index, ...);
void NDArray_map(NDArray* arr, NDArray_mapping_t mapping);
void NDArray_add(NDArray* arr, NDArray_Backer value);
void NDArray_sub(NDArray* arr, NDArray_Backer value);
void NDArray_mul(NDArray* arr, NDArray_Backer value);
void NDArray_div(NDArray* arr, NDArray_Backer value);
void NDArray_and(NDArray* arr, NDArray_Backer value);
void NDArray_or(NDArray* arr, NDArray_Backer value);
void NDArray_xor(NDArray* arr, NDArray_Backer value);
void NDArray_map2(NDArray* left, NDArray* right, NDArray_map2ing_t map2ing);
void NDArray_arr_add(NDArray* left, NDArray* right);
void NDArray_arr_sub(NDArray* left, NDArray* right);
void NDArray_arr_mul(NDArray* left, NDArray* right);
void NDArray_arr_div(NDArray* left, NDArray* right);
void NDArray_arr_and(NDArray* left, NDArray* right);
void NDArray_arr_or(NDArray* left, NDArray* right);
void NDArray_arr_xor(NDArray* left, NDArray* right);

#endif //JC_JARRAY_H
