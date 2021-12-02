#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include "jint.h"

int strtoi(const char* nptr, char** endptr, int base) {
    long parsed = strtol(nptr, endptr, base);
    if (parsed > INT_MAX) {
        errno = ERANGE;
        return INT_MAX;
    }
    if (parsed < INT_MIN) {
        errno = ERANGE;
        return INT_MIN;
    }
    return (int)parsed;
}

unsigned int strtoui(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > UINT_MAX) {
        errno = ERANGE;
        return UINT_MAX;
    }
    return (unsigned int)parsed;
}

i8 strtoi8(const char* nptr, char** endptr, int base) {
    long parsed = strtol(nptr, endptr, base);
    if (parsed > I8_MAX) {
        errno = ERANGE;
        return I8_MAX;
    }
    if (parsed < I8_MIN) {
        errno = ERANGE;
        return I8_MIN;
    }
    return (i8)parsed;
}

u8 strtou8(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U8_MAX) {
        errno = ERANGE;
        return U8_MAX;
    }
    return (u8)parsed;
}

i16 strtoi16(const char* nptr, char** endptr, int base) {
    long parsed = strtol(nptr, endptr, base);
    if (parsed > I16_MAX) {
        errno = ERANGE;
        return I16_MAX;
    }
    if (parsed < I16_MIN) {
        errno = ERANGE;
        return I16_MIN;
    }
    return (i16)parsed;
}

u16 strtou16(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U16_MAX) {
        errno = ERANGE;
        return U16_MAX;
    }
    return (u16)parsed;
}

i32 strtoi32(const char* nptr, char** endptr, int base) {
    long parsed = strtol(nptr, endptr, base);
    if (parsed > I32_MAX) {
        errno = ERANGE;
        return I32_MAX;
    }
    if (parsed < I32_MIN) {
        errno = ERANGE;
        return I32_MIN;
    }
    return (i32)parsed;
}

u32 strtou32(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U32_MAX) {
        errno = ERANGE;
        return U32_MAX;
    }
    return (u32)parsed;
}

i64 strtoi64(const char* nptr, char** endptr, int base) {
    long parsed = strtol(nptr, endptr, base);
    if (parsed > I64_MAX) {
        errno = ERANGE;
        return I64_MAX;
    }
    if (parsed < I64_MIN) {
        errno = ERANGE;
        return I64_MIN;
    }
    return (i64)parsed;
}

u64 strtou64(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U64_MAX) {
        errno = ERANGE;
        return U64_MAX;
    }
    return (u64)parsed;
}

isize strotoisize(const char* nptr, char** endptr, int base) {
    long parsed = strtol(nptr, endptr, base);
    if (parsed > ISIZE_MAX) {
        errno = ERANGE;
        return ISIZE_MAX;
    }
    if (parsed < ISIZE_MIN) {
        errno = ERANGE;
        return ISIZE_MIN;
    }
    return (i64)parsed;}

usize strotousize(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > USIZE_MAX) {
        errno = ERANGE;
        return USIZE_MAX;
    }
    return (u64)parsed;
}
