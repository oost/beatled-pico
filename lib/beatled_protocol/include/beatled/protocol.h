#ifndef COMMAND__CONSTANTS_H
#define COMMAND__CONSTANTS_H

#include <stdint.h>

#ifndef PICO_UNIQUE_BOARD_ID_SIZE_BYTES
#define PICO_UNIQUE_BOARD_ID_SIZE_BYTES 8
#endif // PICO_UNIQUE_BOARD_ID_SIZE_BYTES

#ifdef __cplusplus
extern "C" {
#endif

// Code for header body
#define COMMAND_HELLO 'a'
#define COMMAND_RANDOM 'b'
#define COMMAND_PROGRAM 'c'
#define COMMAND_BEAT 'd'
#define COMMAND_TEMPO 't'
#define COMMAND_TIME 'T'
#define COMMAND_ERROR 'E'

// Tempo message. Command = 'E'
typedef struct {
  uint8_t command;
  uint8_t error_code;
} __attribute__((packed)) error_msg_t;

// Tempo message. Command = 't'
typedef struct {
  uint8_t command;
  uint64_t beat_time_ref;
  uint32_t tempo_period_us;
} __attribute__((packed)) tempo_msg_t;

// Hello. Command = 'h'
typedef struct {
  uint8_t command;
  char board_id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
} __attribute__((packed)) hello_msg_t;

// Command = 'T'
typedef struct {
  uint8_t command;
  uint64_t orig_time;
} __attribute__((packed)) time_req_msg_t;

// Command = 'T'
typedef struct {
  uint8_t command;
  uint64_t orig_time;
  uint64_t rec_time;
  uint64_t xmit_time;
} __attribute__((packed)) time_resp_msg_t;

// Command = 'P'
typedef struct {
  uint8_t command;
  uint64_t orig_time;
  uint32_t program_id;
} __attribute__((packed)) program_msg_t;

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif // COMMAND__CONSTANTS_H