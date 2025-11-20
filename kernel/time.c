#include "time.h"

void irq_tick() {
    ticks++;
}

uint64_t get_ticks() {
    return ticks;
}
float get_milis() {
    return (float)(ticks)*MS_PER_TICK;
}

void wait(float millis) {
    float start_time = get_milis();
    while (1) {
        float current_time = get_milis();
        // if (current_time - start_time < 0) { // Overflow in ticks
        //     millis -= (0xffffffff * MS_PER_TICK) - start_time;
        //     start_time = current_time;
        // }

        if (current_time - start_time > millis) {
            break;
        }
    }
}
