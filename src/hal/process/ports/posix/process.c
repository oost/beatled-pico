#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "hal/process.h"

pthread_t core0_thread;
pthread_t core1_thread;

void start_core0(core_loop_fn core_loop) {
  pthread_create(&core0_thread, NULL, core_loop, NULL);
}
void start_core1(core_loop_fn core_loop) {
  pthread_create(&core1_thread, NULL, core_loop, NULL);
}

void join_cores() {
  pthread_join(core0_thread, NULL);
  pthread_join(core1_thread, NULL);
}