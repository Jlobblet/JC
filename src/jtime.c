#include "jtime.h"

clock_t elapsed_ns(clock_t start, clock_t end) { return ((end - start) * 1000000000) / CLOCKS_PER_SEC; }
clock_t elapsed_us(clock_t start, clock_t end) { return ((end - start) * 1000000) / CLOCKS_PER_SEC; }
clock_t elapsed_ms(clock_t start, clock_t end) { return ((end - start) * 1000) / CLOCKS_PER_SEC; }
clock_t elapsed_s(clock_t start, clock_t end) { return (end - start) / CLOCKS_PER_SEC; }
