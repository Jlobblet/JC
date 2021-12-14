#ifndef JC_JPRINT_H
#define JC_JPRINT_H

#include <stdio.h>

#define format_specifier(x) _Generic((x), \
    char: "%c", \
    signed char: "%hhd", \
    unsigned char: "%hhu", \
    signed short: "%hd", \
    unsigned short: "%hu", \
    signed int: "%d", \
    unsigned int: "%u", \
    long int: "%ld", \
    unsigned long int: "%lu", \
    long long int: "%lld", \
    unsigned long long int: "%llu", \
    float: "%f", \
    double: "%lf", \
    long double: "%Lf", \
    char*: "%s", \
    void*: "%p", \
    volatile char: "%c", \
    volatile signed char: "%hhd", \
    volatile unsigned char: "%hhu", \
    volatile signed short: "%hd", \
    volatile unsigned short: "%hu", \
    volatile signed int: "%d", \
    volatile unsigned int: "%u", \
    volatile long int: "%ld", \
    volatile unsigned long int: "%lu", \
    volatile long long int: "%lld", \
    volatile unsigned long long int: "%llu", \
    volatile float: "%f", \
    volatile double: "%lf", \
    volatile long double: "%Lf", \
    volatile char*: "%s", \
    volatile void*: "%p", \
    const char: "%c", \
    const signed char: "%hhd", \
    const unsigned char: "%hhu", \
    const signed short: "%hd", \
    const unsigned short: "%hu", \
    const signed int: "%d", \
    const unsigned int: "%u", \
    const long int: "%ld", \
    const unsigned long int: "%lu", \
    const long long int: "%lld", \
    const unsigned long long int: "%llu", \
    const float: "%f", \
    const double: "%lf", \
    const long double: "%Lf", \
    const char*: "%s", \
    const void*: "%p", \
    const volatile char: "%c", \
    const volatile signed char: "%hhd", \
    const volatile unsigned char: "%hhu", \
    const volatile signed short: "%hd", \
    const volatile unsigned short: "%hu", \
    const volatile signed int: "%d", \
    const volatile unsigned int: "%u", \
    const volatile long int: "%ld", \
    const volatile unsigned long int: "%lu", \
    const volatile long long int: "%lld", \
    const volatile unsigned long long int: "%llu", \
    const volatile float: "%f", \
    const volatile double: "%lf", \
    const volatile long double: "%Lf", \
    const volatile char*: "%s", \
    const volatile void*: "%p", \
    default: "%d")

#if defined(_WIN32) || defined(__WIN32) || defined(WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64) || defined(WIN64) || defined(__WIN64__) || defined(__WINNT) || defined(__WINNT__) || defined(WINNT)
#define endl "\r\n"
#else
#define endl "\n"
#endif

#define putv(x) printf(format_specifier((x)), (x))
#define putvln(x) printf(format_specifier((x)), (x)); printf(endl)
#define fputv(x, f) fprintf((f), format_specifier((x)), (x))
#define fputvln(x, f) fprintf((f), format_specifier((x)), (x)); fprintf((f), endl)
#define eputv(x) eprintf(format_specifier((x)), (x))
#define eputvln(x) eprintf(format_specifier((x)), (x)); eprintf(endl)

#endif //JC_JPRINT_H
