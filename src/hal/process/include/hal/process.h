#ifndef HAL__PROCESS__H
#define HAL__PROCESS__H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void sleep_ms(uint32_t duration);

typedef void (*core_loop_fn)();

void start_core0(core_loop_fn core_loop);
void start_core1(core_loop_fn core_loop);

#ifdef __cplusplus
}
#endif
#endif // HAL__PROCESS__H