#include <time.h>
#include <unistd.h>

#include "hal/process.h"

void sleep_ms(uint32_t duration) { usleep(((uint64_t)duration) * 1000); }