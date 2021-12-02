#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include "jio.h"

uptr read_all_lines(const char* filepath, char*** lines, uptr** lengths, uptr n) {
    if (lines == NULL) {
        errno = EINVAL;
        return -1;
    }
    if (lengths == NULL) {
        errno = EINVAL;
        return -1;
    }
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
        *lengths = (uptr*)malloc(sizeof(uptr));
        if (*lengths == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }
    // How much the buffers grow each time they hit capacity
    const uptr growth_factor = 2;
    // Current index
    uptr i = 0;
    // Length of the buffer getline allocates
    uptr buffer_length = 0;
    while (getline((*lines) + i, &buffer_length, fp) != -1) {
        if (i >= n) {
            n *= growth_factor;
            *lines = (char**)realloc(*lines, n * sizeof(char*));
            if (*lines == NULL) {
                errno = ENOMEM;
                return -1;
            }
            *lengths = (uptr*)realloc(*lengths, n * sizeof(uptr));
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
    fclose(fp);
    // Shrink arrays to minimum size
    *lines = (char**)realloc(*lines, n * sizeof(char*));
    if (*lines == NULL) {
        errno = ENOMEM;
        return -1;
    }
    *lengths = (uptr*)realloc(*lengths, i * sizeof(uptr));
    if (*lines == NULL) {
        errno = ENOMEM;
        return -1;
    }
    return i;
}
