
#include <string.h>

#include "autotest.h"
#include "command/constants.h"
#include "command_queue/queue.h"
#include "utils/network.h"

static uint64_t server_time_ref_us;

void init_test() { server_time_ref_us = get_sntp_server_time_ref_us(); }

void test_tempo() {
  float tempo = 120;

  tempo_msg_t tempo_msg;
  tempo_msg.command = COMMAND_TEMPO;

  printf("Time since boot: %llu (%llx)\n", server_time_ref_us,
         server_time_ref_us);
  tempo_msg.beat_time_ref = htonll(server_time_ref_us);
  printf("Time since boot big endian: %llu (%llx)\n", tempo_msg.beat_time_ref,
         tempo_msg.beat_time_ref);

  uint32_t tempo_period_us = 60 * 1000000UL / tempo;
  tempo_msg.tempo_period_us = htonl(tempo_period_us);
  printf("TTempo period: %lu (%lx)\n", tempo_period_us, tempo_period_us);
  printf("TTempo period big endian: %lu (%lx)\n", tempo_msg.tempo_period_us,
         tempo_msg.tempo_period_us);

  command_envelope_t envelope;
  command_envelope_message_alloc(&envelope, 13);

  memcpy(envelope.message, (void *)&tempo_msg, envelope.message_length);

  if (!command_queue_add_message(&envelope)) {
    puts("Couldn't add message");
  }
}