#include "time_synced.h"
#include "command.h"

int enter_time_synced_state() {
  send_tempo_request();
  return 0;
}
int exit_time_synced_state() { return 0; }