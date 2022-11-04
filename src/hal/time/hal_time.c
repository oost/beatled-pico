#include <pico/time.h>
#include <time.h>

#include "hal_time.h"

// void print_current_time_with_ms (void)
// {
//     long            ms; // Milliseconds
//     time_t          s;  // Seconds
//     struct timespec spec;

//     clock_gettime(CLOCK_REALTIME, &spec);

//     s  = spec.tv_sec;
//     ms = round(spec.tv_nsec / 1.0e6); // Convert nanoseconds to milliseconds
//     if (ms > 999) {
//         s++;
//         ms = 0;
//     }

//     printf("Current time: %"PRIdMAX".%03ld seconds since the Epoch\n",
//            (intmax_t)s, ms);
// }
