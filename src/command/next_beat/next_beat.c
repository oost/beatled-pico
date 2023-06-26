#include <stdlib.h>

#include "beatled/protocol.h"
#include "clock.h"
#include "command/utils.h"
#include "hal/network.h"
#include "hal/registry.h"
#include "next_beat.h"
#include "process/intercore_queue.h"
#include "state_manager/state_manager.h"

int process_next_beat_msg(beatled_message_t *server_msg, size_t data_length) {

  if (!check_size(data_length, sizeof(beatled_message_next_beat_t))) {
    return 1;
  }

  int current_state = state_manager_get_state();
  if (current_state != STATE_TIME_SYNCED &&
      current_state != STATE_TEMPO_SYNCED) {
    // printf("Can't set next beat while in state %d\n",
    //        state_manager_get_state());
    return 0;
  }

  beatled_message_next_beat_t *next_beat_msg =
      (beatled_message_next_beat_t *)server_msg;

  // printf("%llu\n", next_beat_msg->next_beat_time_ref);
  uint64_t next_beat_time_ref =
      server_time_to_local_time(ntohll(next_beat_msg->next_beat_time_ref));
  uint32_t tempo_period_us = ntohl(next_beat_msg->tempo_period_us);
  uint32_t beat_count = ntohl(next_beat_msg->beat_count);
  uint16_t program_id = ntohs(next_beat_msg->program_id);

  printf("Got next beat time ref %llu (%llx), tempo ref %u (%x), program_id %d "
         "(%x), time to next beat %d\n",
         next_beat_time_ref, next_beat_time_ref, tempo_period_us,
         tempo_period_us, program_id, program_id,
         (signed)(next_beat_time_ref - time_us_64()));

  if (next_beat_time_ref < time_us_64()) {
    puts("Next beat is in the past...");
  }

  if (state_manager_get_state() != STATE_TEMPO_SYNCED) {
    if (!schedule_state_transition(STATE_TEMPO_SYNCED)) {
      puts("- Can't schedule transition to initialized state. Fatal error.");
      exit(1);
    }
  }

  registry_lock_mutex();
  registry.next_beat_time_ref = next_beat_time_ref;
  registry.tempo_period_us = tempo_period_us;
  registry.beat_count = beat_count;
  registry.program_id = program_id;
  registry.update_timestamp = time_us_64();
  registry_unlock_mutex();

  return 0;
}
