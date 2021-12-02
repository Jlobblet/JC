/// \file
/// Utility functions for dealing with input/output.
#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include "jio.h"

/// Read all lines from a file into an array of null-terminal strings.
///
/// The read lines include newline character(s), and are null terminated.
/// \param filepath Name of the file to open.
/// \param lines Pointer to an array to store read lines in.
/// \param lengths Pointer to an array to store the length of read lines in.
/// \param n The length of the `lines` and `lengths` arrays, which must be equal.
/// If `n` is `0`, `lines` and `lengths` will be allocated memory to begin with.
/// \return On success, returns the number of lines read (and hence the length of `lines` and `lengths`).
/// On failure, returns `-1` and `errno` is set to indicate the error.
/// \remark Lines and lengths should either be large enough to fit the entire file or able to be
/// dynamically reallocated.
/// \remark This function allocates a `char*` array for each line in the file. The programmer must
/// free each entry in the `lines` array, then `lines` and `lengths`, when freeing allocated memory.
/// \remark All allocations made are shrunk to the minimum size after reading in the file.
/// \errors
/// `errno`  | Description
/// -------- | -----------
/// `EINVAL` | `lines` or `lengths` is a null pointer.
/// `ENOMEM` | There was insufficient memory to expand either `lines` or `lengths`.
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
        *lines = (char**)malloc(n *sizeof(char*));
        if (*lines == NULL) {
            return -1;
        }
        *lengths = (uptr*)malloc(n * sizeof(uptr*));
        if (*lengths == NULL) {
            return -1;
        }
    }
    // How much the buffers grow each time they hit capacity
    const uptr growth_factor = 2;
    uptr i;
    // Length of the buffer getline allocates
    uptr buffer_length = 0;
    for (i = 0; getline((*lines) + i, &buffer_length, fp) != -1; i++) {
        if (i >= n) {
            n *= growth_factor;
            *lines = (char**)realloc(*lines, n * sizeof(char*));
            if (*lines == NULL) {
                return -1;
            }
            *lengths = (uptr*)realloc(*lengths, n * sizeof(uptr*));
            if (*lengths == NULL) {
                return -1;
            }
        }
        (*lengths)[i] = buffer_length;
        // Reset buffer length so that getline can allocate again
        buffer_length = 0;
    }
    fclose(fp);
    // Shrink arrays to minimum size
    *lines = (char**)realloc(*lines, i * sizeof(char*));
    if (*lines == NULL) {
        return -1;
    }
    *lengths = (uptr*)realloc(*lengths, i * sizeof(uptr*));
    if (*lines == NULL) {
        return -1;
    }
    return i;
}
