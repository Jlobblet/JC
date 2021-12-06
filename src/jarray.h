#ifndef JC_JARRAY_H
#define JC_JARRAY_H

#include <stdarg.h>
#include "jint.h"

typedef struct NDArray {
    uptr dims;
    uptr *size;
    fi64 *data;
} NDArray;

iptr NDArray_init(NDArray* arr);
fi64 NDArray_get(NDArray* arr, uptr index, ...);
void NDArray_set(NDArray* arr, fi64 value, uptr index, ...);

#endif //JC_JARRAY_H
