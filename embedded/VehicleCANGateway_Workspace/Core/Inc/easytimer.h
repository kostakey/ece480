#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

typedef struct {
    uint32_t start_time;
    uint32_t interval;
} EasyTimer_t;

// Function to check if the timer has expired
bool Timer_HasElapsed(EasyTimer_t *timer) {
    if ((HAL_GetTick() - timer->start_time) >= timer->interval) {
        timer->start_time = HAL_GetTick(); // Reset the timer start point
        return true;
    }
    return false;
}

#endif
