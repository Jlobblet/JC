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

iptr NDArray_init(NDArray* arr);
NDArray_Backer NDArray_get(NDArray* arr, uptr index, ...);
void NDArray_set(NDArray* arr, NDArray_Backer value, uptr index, ...);

#endif //JC_JARRAY_H
