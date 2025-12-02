#ifndef TIME_H
#define TIME_H
#include <stdint.h>

#define MS_PER_TICK 54.9254

void irq_tick();

uint64_t get_ticks();
float get_milis();

void wait(float millis);

#endif
