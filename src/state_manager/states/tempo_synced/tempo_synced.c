#include <stdio.h>

#include "state_manager/states/tempo_synced.h"
#include "command/command.h"
#include "hal/time.h"

#define TEMPO_ALARM_DELAY_US 10000000
#define TIME_ALARM_DELAY_US 10000000
#define CHECK_ALARM_DELAY_US 60000000

hal_alarm_t *tempo_alarm;
hal_alarm_t *time_alarm;
hal_alarm_t *check_alarm;

void refresh_tempo_timer_callback(void *data) { send_tempo_request(); }

void refresh_time_timer_callback(void *data) { send_time_request(); }

// void refresh_time_timer_callback(void *data) {
//   // If not synced go back to
// }

int enter_tempo_synced_state() {
  tempo_alarm = hal_add_repeating_timer(TEMPO_ALARM_DELAY_US,
                                        &refresh_tempo_timer_callback, NULL);
  if (!tempo_alarm) {
    puts("Failed to allocate tempo alarm");
    return 1;
  }

  time_alarm = hal_add_repeating_timer(TIME_ALARM_DELAY_US,
                                       &refresh_time_timer_callback, NULL);
  if (!time_alarm) {
    puts("Failed to allocate time alarm");
    hal_cancel_repeating_timer(tempo_alarm);
    return 1;
  }

  check_alarm = hal_add_repeating_timer(CHECK_ALARM_DELAY_US,
                                        &refresh_time_timer_callback, NULL);
  if (!check_alarm) {
    puts("Failed to allocate check alarm");
    hal_cancel_repeating_timer(tempo_alarm);
    hal_cancel_repeating_timer(time_alarm);
    return 1;
  }

  return 0;
}
int exit_tempo_synced_state() {
  hal_cancel_repeating_timer(check_alarm);
  hal_cancel_repeating_timer(tempo_alarm);
  hal_cancel_repeating_timer(time_alarm);
  return 0;
}