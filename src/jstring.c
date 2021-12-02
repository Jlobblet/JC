/// \file
/// Utility functions for C-style char* strings.
#include <stdio.h>
#include "jstring.h"

/// Read chars from start until reaching a newline, \0, or EOF.
/// \param start Pointer to the beginning of the string or line to read from.
/// \param end Pointer to where to write the ending pointer of the string.
/// \return On success, return how many characters are between the start and end of the line.
/// If no characters are read because start begins with \0 or EOF, return -1.
/// \remark The line is not guaranteed to be null-terminated.
/// The end of the line is bounded by the pointer written to end.
iptr advance_line(const char* start, const char** end) {
    *end = start;
    if (**end == '\0' || **end == EOF) { return -1; }
    for (; **end != '\n' && **end != '\0' && **end != EOF; (*end)++);
    (*end)++;
    return *end - start;
}
