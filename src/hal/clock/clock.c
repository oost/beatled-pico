#include <pico/time.h>

#include "clock.h"

uint64_t get_local_time_us() { return time_us_64(); }
