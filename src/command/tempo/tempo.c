#include "tempo.h"
#include "beatled/protocol.h"
#include "clock/clock.h"
#include "command/utils.h"
#include "hal/network.h"
#include "hal/udp.h"
#include "intercore_queue.h"
#include "state_manager/state.h"
#include "state_manager/state_manager.h"

int prepare_tempo_request(void *buffer_payload, size_t buf_len) {
  if (buf_len != sizeof(beatled_tempo_msg_t)) {
    printf("Error sizes don't match %zu, %zu", buf_len,
           sizeof(beatled_tempo_msg_t));
    return 1;
  }

  beatled_tempo_msg_t *msg = buffer_payload;
  msg->base.type = eBeatledTempo;
  return 0;
}

int send_tempo_request() {
  return send_udp_request(sizeof(beatled_tempo_msg_t), prepare_tempo_request);
}

int process_tempo_msg(beatled_message_t *server_msg, size_t data_length) {
  puts("Tempo!");
  if (!check_size(data_length, sizeof(beatled_tempo_msg_t))) {
    return 1;
  }
  beatled_tempo_msg_t *tempo_msg = (beatled_tempo_msg_t *)server_msg;

  uint64_t beat_time_ref = ntohll(tempo_msg->beat_time_ref);
  uint32_t tempo_period_us = ntohl(tempo_msg->tempo_period_us);

  uint64_t beat_local_time_ref = server_time_to_local_time(beat_time_ref);
  // printf("Updated beat ref to %llu (%llx)\n", beat_time_ref, beat_time_ref);
  // printf("Updated tempo to %lu (%lx)\n", tempo_period_us, tempo_period_us);

  state_update_t state_update = {.tempo_time_ref = beat_local_time_ref,
                                 .tempo_period_us = tempo_period_us};

  state_manager_set_state(STATE_TIME_SYNCED);

  if (!hal_queue_add_message(intercore_command_queue, &state_update)) {
    puts("Intercore queue is FULL!!!");
    return 1;
  }

  return 0;
}