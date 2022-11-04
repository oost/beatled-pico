#ifndef HAL__QUEUE__QUEUE_H
#define HAL__QUEUE__QUEUE_H

typedef int (*core_loop_fn)();

void start_core0(core_loop_fn core_loop);
void start_core1(core_loop_fn core_loop);

#endif // HAL__QUEUE__QUEUE_H