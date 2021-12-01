#include "jtime.h"

clock_t elapsed_us(clock_t start, clock_t end) { return ((end - start) * 1000000) / CLOCKS_PER_SEC; }
