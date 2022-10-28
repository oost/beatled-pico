#include <pico/stdlib.h>
#include <stdio.h>

#include "beatled/protocol.h"
#include "blink/blink.h"
#include "clock/clock.h"
#include "command/command.h"
#include "intercore_queue.h"
#include "state_manager/state.h"
#include "utils/network.h"
#include "ws2812/ws2812.h"

bool check_size(size_t data_length, size_t expected_length) {
  if (data_length != expected_length) {
    printf("Sizes don't match %u <> %u", data_length, expected_length);
    return false;
  }
  return true;
}

int command_hello(beatled_message_t *server_msg, size_t data_length) {
  puts("Hello!");
  if (!check_size(data_length, sizeof(beatled_hello_msg_t))) {
    return 1;
  }
  beatled_hello_msg_t *hello_msg = (beatled_hello_msg_t *)server_msg;
  blink(MESSAGE_BLINK_SPEED, MESSAGE_HELLO);
  return 0;
}

int command_program(beatled_message_t *server_msg, size_t data_length) {
  puts("Program!");
  if (!check_size(data_length, sizeof(beatled_program_msg_t))) {
    return 1;
  }
  beatled_program_msg_t *program_msg = (beatled_program_msg_t *)server_msg;

  led_update_pattern_idx(program_msg->program_id);
  return 0;
}

int command_tempo(beatled_message_t *server_msg, size_t data_length) {
  puts("Tempo!");
  if (!check_size(data_length, sizeof(beatled_tempo_msg_t))) {
    return 1;
  }
  beatled_tempo_msg_t *tempo_msg = (beatled_tempo_msg_t *)server_msg;

  uint64_t beat_time_ref = ntohll(tempo_msg->beat_time_ref);
  uint32_t tempo_period_us = ntohl(tempo_msg->tempo_period_us);

  absolute_time_t beat_absolute_time_ref =
      server_time_to_local_time(beat_time_ref);
  printf("Updated beat ref to %llu (%llx)\n", beat_time_ref, beat_time_ref);
  printf("Updated tempo to %lu (%lx)\n", tempo_period_us, tempo_period_us);

  state_update_t state_update = {.tempo_time_ref = beat_time_ref,
                                 .tempo_period_us = tempo_period_us};

  if (!queue_try_add(&intercore_command_queue, &state_update)) {
    puts("Intercore queue is FULL!!!");
    return 1;
  }

  return 0;
}

int command_time(beatled_message_t *server_msg, size_t data_length) {
  puts("Time!");
  if (!check_size(data_length, sizeof(beatled_time_resp_msg_t))) {
    return 1;
  }
  beatled_time_resp_msg_t *time_resp_msg =
      (beatled_time_resp_msg_t *)server_msg;

  return 0;
}

int command_error(beatled_message_t *server_msg, size_t data_length) {
  puts("Error");
  if (!check_size(data_length, sizeof(beatled_error_msg_t))) {
    return 1;
  }
  beatled_error_msg_t *error_msg = (beatled_error_msg_t *)server_msg;

  printf("Communication error %u\n", error_msg->error_code);
  return 0;
}

int handle_server_message(void *event_data, size_t data_length) {
  if (sizeof(beatled_message_t) >= data_length) {
    return 1;
  }
  beatled_message_t *server_msg = (beatled_message_t *)event_data;

  int err = 0;
  switch (server_msg->type) {
  case eBeatledHello:
    err = command_hello(server_msg, data_length);
    break;

  case eBeatledProgram:
    err = command_program(server_msg, data_length);
    break;

  case eBeatledTempo:
    err = command_tempo(server_msg, data_length);
    break;

  case eBeatledTime:
    err = command_time(server_msg, data_length);
    break;

  case eBeatledError:
    err = command_error(server_msg, data_length);
    break;

  default:
    puts("Unknown command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
  }

  return err;
}

int handle_event(event_t *event) {
  puts("Handling event");

  int err;
  switch (event->event_type) {
  case event_server_message:
    err = handle_server_message(event->data, event->data_length);
    break;

  case event_error:
    puts("Error event command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
    break;

  default:
    puts("Unknown command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    err = 1;
  }
  if (event->data != NULL) {
    free(event->data);
  }
  return err;
}
