#include <stdio.h>

#include "command/command.h"
#include "hal/process.h"
#include "hal/time.h"
#include "initialized.h"
#include "process/core0.h"
#include "process/core1.h"
#include "process/isr_thread.h"

#define HELLO_ALARM_DELAY_US 10000000

hal_alarm_t *hello_alarm;

void hello_timer_callback(void *data) { send_hello_request(); }

int enter_initialized_state() {
  // Launch core 1
  start_core1(&core1_entry);
  start_isr_thread(&isr_thread_entry);

  hello_alarm = hal_add_repeating_timer(HELLO_ALARM_DELAY_US,
                                        &hello_timer_callback, NULL);

  return 0;
}
int exit_initialized_state() {
  hal_cancel_repeating_timer(hello_alarm);
  return 0;
}
