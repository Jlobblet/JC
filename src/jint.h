#ifndef JC_JINT_H
#define JC_JINT_H

#include <stdint.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;
typedef intptr_t isize;
typedef uintptr_t usize;
typedef intmax_t imax;
typedef uintmax_t umax;

typedef int_least8_t li8;
typedef uint_least8_t lu8;
typedef int_least16_t li16;
typedef uint_least16_t lu16;
typedef int_least32_t li32;
typedef uint_least32_t lu32;
typedef int_least64_t li64;
typedef uint_least64_t lu64;

typedef int_fast8_t fi8;
typedef uint_fast8_t fu8;
typedef int_fast16_t fi16;
typedef uint_fast16_t fu16;
typedef int_fast32_t fi32;
typedef uint_fast32_t fu32;
typedef int_fast64_t fi64;
typedef uint_fast64_t fu64;

#define I8_MAX INT8_MAX
#define U8_MAX UINT8_MAX
#define I16_MAX INT16_MAX
#define U16_MAX INT16_MAX
#define I32_MAX INT32_MAX
#define U32_MAX UINT32_MAX
#define I64_MAX INT64_MAX
#define U64_MAX UINT64_MAX
#define ISIZE_MAX INTPTR_MAX
#define USIZE_MAX UINTPTR_MAX

#define I8_MIN INT8_MIN
#define I16_MIN INT16_MIN
#define I32_MIN INT32_MIN
#define I64_MIN INT64_MIN
#define ISIZE_MIN INTPTR_MIN

int strtoi(const char* nptr, char** endptr, int base);
unsigned int strtoui(const char* nptr, char** endptr, int base);

i8 strtoi8(const char* nptr, char** endptr, int base);
u8 strtou8(const char* nptr, char** endptr, int base);
i16 strtoi16(const char* nptr, char** endptr, int base);
u16 strtou16(const char* nptr, char** endptr, int base);
i32 strtoi32(const char* nptr, char** endptr, int base);
u32 strtou32(const char* nptr, char** endptr, int base);
i64 strtoi64(const char* nptr, char** endptr, int base);
u64 strtou64(const char* nptr, char** endptr, int base);
isize strotoisize(const char* nptr, char** endptr, int base);
usize strotousize(const char* nptr, char** endptr, int base);

#endif //JC_JINT_H
