#include <pico/stdlib.h>
#include <stdio.h>

#include "beatled/protocol.h"
#include "blink/blink.h"
#include "clock/clock.h"
#include "command/command.h"
#include "state_manager/state_manager.h"
#include "utils/network.h"
#include "ws2812/ws2812.h"

int command_hello() {
  puts("Hello!");
  blink(MESSAGE_BLINK_SPEED, MESSAGE_HELLO);
  return 0;
}

int command_random() {
  led_set_random_pattern();
  return 0;
}
int command_program(command_envelope_t *envelope) {
  led_update_pattern_idx(envelope->message[1]);
  return 0;
}

int command_beat(command_envelope_t *envelope) {
  puts("Beat!");
  uint64_t beat_time = 0;
  int i;
  uint64_t paquet = 0;
  for (i = 7; i >= 0; --i) {
    paquet <<= 8;
    paquet |= (uint64_t)envelope->message[1 + i];
  }

  led_beat();
  return 0;
}

int command_tempo(command_envelope_t *envelope) {
  puts("Tempo!");

  if (envelope->message_length != sizeof(tempo_msg_t)) {
    return -1;
  }

  tempo_msg_t *tempo_msg = (tempo_msg_t *)envelope->message;

  uint64_t beat_time_ref = ntohll(tempo_msg->beat_time_ref);
  uint32_t tempo_period_us = ntohl(tempo_msg->tempo_period_us);

  absolute_time_t beat_absolute_time_ref =
      server_time_to_local_time(beat_time_ref);
  printf("Updated beat ref to %llu (%llx)\n", beat_time_ref, beat_time_ref);
  printf("Updated tempo to %lu (%lx)\n", tempo_period_us, tempo_period_us);

  state_manager_set_tempo(beat_absolute_time_ref, tempo_period_us);
  return 0;
}

int command_error(command_envelope_t *envelope) {
  puts("Error");

  if (envelope->message_length != sizeof(error_msg_t)) {
    return -1;
  }

  error_msg_t *error_msg = (error_msg_t *)envelope->message;

  printf("Communication error %u\n", error_msg->error_code);
  return 0;
}

int parse_command(command_envelope_t *envelope) {
  puts("Parsing command");

  int return_value = 0;

  switch (envelope->message[0]) {
  case COMMAND_HELLO:
    return_value = command_hello();
    break;
  case COMMAND_RANDOM:
    return_value = command_random();
    break;
  case COMMAND_PROGRAM:
    return_value = command_program(envelope);
    break;
  case COMMAND_BEAT:
    return_value = command_beat(envelope);
    break;
  case COMMAND_TEMPO:
    return_value = command_tempo(envelope);
    break;
  case COMMAND_ERROR:
    return_value = command_error(envelope);
    break;
  default:
    puts("Unknown command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    return_value = 1;
  }
  command_envelope_message_free(envelope);

  return return_value;
}
