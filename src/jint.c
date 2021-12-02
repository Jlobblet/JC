/// \file
/// Functions for integral types.

#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include "jint.h"

/// \intconv{int,strtol,INT_MAX,INT_MIN}
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

/// \uintconv{unsigned int,strtoul,UINT_MAX}
unsigned int strtoui(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > UINT_MAX) {
        errno = ERANGE;
        return UINT_MAX;
    }
    return (unsigned int)parsed;
}

/// \intconv{i8,strtol,I8_MAX,I8_MIN}
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

/// \uintconv{u8,strtoul,U8_MAX}
u8 strtou8(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U8_MAX) {
        errno = ERANGE;
        return U8_MAX;
    }
    return (u8)parsed;
}

/// \intconv{i16,strtol,I16_MAX,I16_MIN}
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

/// \uintconv{u16,strtoul,U16_MAX}
u16 strtou16(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U16_MAX) {
        errno = ERANGE;
        return U16_MAX;
    }
    return (u16)parsed;
}

/// \intconv{i32,strtol,I32_MAX,I32_MIN}
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

/// \uintconv{u32,strtoul,U32_MAX}
u32 strtou32(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U32_MAX) {
        errno = ERANGE;
        return U32_MAX;
    }
    return (u32)parsed;
}

/// \intconv{i64,strtol,I64_MAX,I32_MIN}
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

/// \uintconv{u64,strtoul,U64_MAX}
u64 strtou64(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > U64_MAX) {
        errno = ERANGE;
        return U64_MAX;
    }
    return (u64)parsed;
}

/// \intconv{iptr,strtol,IPTR_MAX,IPTR_MIN}
iptr strtoiptr(const char* nptr, char** endptr, int base) {
    long parsed = strtol(nptr, endptr, base);
    if (parsed > IPTR_MAX) {
        errno = ERANGE;
        return IPTR_MAX;
    }
    if (parsed < IPTR_MIN) {
        errno = ERANGE;
        return IPTR_MIN;
    }
    return (iptr)parsed;
}

/// \uintconv{uptr,strtoul,UPTR_MAX}
uptr strtouptr(const char* nptr, char** endptr, int base) {
    unsigned long parsed = strtoul(nptr, endptr, base);
    if (parsed > UPTR_MAX) {
        errno = ERANGE;
        return UPTR_MAX;
    }
    return (uptr)parsed;
}

/// \intconv{imax,strtoll,IMAX_MAX,IMAX_MIN}
imax strtoimax(const char* nptr, char** endptr, int base) {
    long long parsed = strtoll(nptr, endptr, base);
    if (parsed > IMAX_MAX) {
        errno = ERANGE;
        return IMAX_MAX;
    }
    if (parsed < IMAX_MIN) {
        errno = ERANGE;
        return IMAX_MIN;
    }
    return (imax)parsed;}

/// \uintconv{umax,strtoull,UMAX_MAX}
umax strtoumax(const char* nptr, char** endptr, int base) {
    unsigned long long parsed = strtoull(nptr, endptr, base);
    if (parsed > UMAX_MAX) {
        errno = ERANGE;
        return UMAX_MAX;
    }
    return (umax)parsed;
}
