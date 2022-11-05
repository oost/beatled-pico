#ifndef HAL__TIME_H
#define HAL__TIME_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

uint64_t time_us_64(void);
uint64_t get_local_time_us();

#ifdef __cplusplus
}
#endif

#endif // HAL__TIME_H