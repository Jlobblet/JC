#ifndef JC_JTIME_H
#define JC_JTIME_H

#include <time.h>

clock_t elapsed_ns(clock_t start, clock_t end);
clock_t elapsed_us(clock_t start, clock_t end);
clock_t elapsed_ms(clock_t start, clock_t end);
clock_t elapsed_s(clock_t start, clock_t end);

#endif //JC_JTIME_H
