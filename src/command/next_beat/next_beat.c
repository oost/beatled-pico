#include <stdlib.h>

#include "beatled/protocol.h"
#include "command/utils.h"
#include "next_beat.h"

int process_next_beat_msg(beatled_message_t *server_msg, size_t data_length) {
  puts("Next beat!");
  if (!check_size(data_length, sizeof(beatled_message_next_beat_t))) {
    return 1;
  }

  beatled_message_next_beat_t *next_beat_msg =
      (beatled_message_next_beat_t *)server_msg;

  uint64_t next_beat_time_ref = ntohll(next_beat_msg->next_beat_time_ref);
  uint32_t tempo_period_us = ntohl(next_beat_msg->tempo_period_us);
  uint16_t program_id = ntohs(next_beat_msg->program_id);

  // uint64_t beat_local_time_ref = server_time_to_local_time(beat_time_ref);
  printf("Got next beat time ref %llu (%llx)\n", next_beat_time_ref,
         next_beat_time_ref);
  printf("Got tempo ref %u (%x)\n", tempo_period_us, tempo_period_us);
  // printf("Updated tempo period to %u (%x), equivalent tempo %f\n",
  //  tempo_period_us, tempo_period_us, 1000000.0 * 60 / tempo_period_us);
  // uint16_t client_id = ntohs(next_beat_msg->client_id);
  // printf("Registered with client id: %d", client_id);
  return 0;
}
