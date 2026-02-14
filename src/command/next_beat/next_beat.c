#include <stdlib.h>

#include "beatled/protocol.h"
#include "config/constants.h"
#include "clock/clock.h"
#include "command/next_beat.h"
#include "command/utils.h"
#include "hal/network.h"
#include "hal/registry.h"
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

  // printf("Next beat: %llu\n", ntohll(next_beat_msg->next_beat_time_ref));
  uint64_t next_beat_time_ref =
      server_time_to_local_time(ntohll(next_beat_msg->next_beat_time_ref));
  uint32_t tempo_period_us = ntohl(next_beat_msg->tempo_period_us);
  uint32_t beat_count = ntohl(next_beat_msg->beat_count);
  uint16_t program_id = ntohs(next_beat_msg->program_id);

#if BEATLED_VERBOSE_LOG
  printf("[CMD] Next beat: ref=%llu (in %d us), tempo=%u, program=%d\n",
         next_beat_time_ref, (signed)(next_beat_time_ref - time_us_64()),
         tempo_period_us, program_id);

  if (next_beat_time_ref < time_us_64()) {
    puts("[CMD] Next beat is in the past");
  }
#endif

  if (state_manager_get_state() != STATE_TEMPO_SYNCED) {
    if (!schedule_state_transition(STATE_TEMPO_SYNCED)) {
      BEATLED_FATAL("Failed to schedule transition to tempo synced state");
      return 1;
    }
  }

  registry_lock_mutex();
  registry.next_beat_time_ref = next_beat_time_ref;
  registry.tempo_period_us = tempo_period_us;
  registry.beat_count = beat_count;
  registry.program_id = program_id;
  registry.update_timestamp = time_us_64();
  registry_unlock_mutex();

  intercore_message_t msg = {.message_type = 0x01 << intercore_time_ref_update |
                                             0x01 << intercore_tempo_update |
                                             0x01 << intercore_program_update};

  if (!hal_queue_add_message(intercore_command_queue, &msg)) {
    BEATLED_FATAL("Intercore queue full");
    return 1;
  }

  return 0;
}
