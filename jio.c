#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include "jio.h"

usize read_all_lines(const char* filepath, char*** lines, usize** lengths, usize n) {
    // Open file
    FILE* fp = fopen(filepath, "r");
    if (fp == NULL) {
        // Don't need to set errno as fopen has
        return -1;
    }
    if (n == 0) {
        // Allocate arrays
        n = 1;
        *lines = (char**)malloc(sizeof(char*));
        if (*lines == NULL) {
            errno = ENOMEM;
            return -1;
        }
        *lengths = (usize*)malloc(sizeof(usize));
        if (*lengths == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }
    // How much the buffers grow each time they hit capacity
    const usize growth_factor = 2;
    // Current index
    usize i = 0;
    // Length of the buffer getline allocates
    usize buffer_length = 0;
    while (getline((*lines) + i, &buffer_length, fp) != -1) {
        if (i >= n) {
            n *= growth_factor;
            *lines = (char**)realloc(*lines, n * sizeof(char*));
            if (*lines == NULL) {
                errno = ENOMEM;
                return -1;
            }
            *lengths = (usize*)realloc(*lengths, n * sizeof(usize));
            if (*lengths == NULL) {
                errno = ENOMEM;
                return -1;
            }
        }
        (*lengths)[i] = buffer_length;
        // Reset buffer length so that getline can allocate again
        buffer_length = 0;
        // Increment index
        ++i;
    }
    // Shrink arrays to minimum size
    *lines = (char**)realloc(*lines, n * sizeof(char*));
    if (*lines == NULL) {
        errno = ENOMEM;
        return -1;
    }
    *lengths = (usize*)realloc(*lengths, i * sizeof(usize));
    if (*lines == NULL) {
        errno = ENOMEM;
        return -1;
    }
    return i;
}
