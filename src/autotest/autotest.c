
#include <string.h>

#include "autotest.h"
#include "beatled/protocol.h"
#include "event_queue/queue.h"
#include "utils/network.h"

static uint64_t server_time_ref_us;

void init_test() { server_time_ref_us = get_sntp_server_time_ref_us(); }

void test_tempo() {
  float tempo = 60;

  beatled_tempo_msg_t *tempo_msg =
      (beatled_tempo_msg_t *)malloc(sizeof(beatled_tempo_msg_t));

  tempo_msg->base.type = eBeatledTempo;

  printf("Time since boot: %llu (%llx)\n", server_time_ref_us,
         server_time_ref_us);
  tempo_msg->beat_time_ref = htonll(server_time_ref_us);
  printf("Time since boot big endian: %llu (%llx)\n", tempo_msg->beat_time_ref,
         tempo_msg->beat_time_ref);

  uint32_t tempo_period_us = 60 * 1000000UL / tempo;
  tempo_msg->tempo_period_us = htonl(tempo_period_us);
  printf("Tempo period: %lu (%lx)\n", tempo_period_us, tempo_period_us);
  printf("Tempo period big endian: %lu (%lx)\n", tempo_msg->tempo_period_us,
         tempo_msg->tempo_period_us);

  size_t data_length = sizeof(beatled_tempo_msg_t);

  if (!event_queue_add_message(event_server_message, (void *)tempo_msg,
                               data_length)) {
    puts("Couldn't add message");
  }
}