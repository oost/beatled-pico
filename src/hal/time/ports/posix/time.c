#include <stdint.h>
#include <time.h>

#include "hal/time.h"

uint64_t time_us_64() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * (uint64_t)1000000 + ts.tv_usec;
}