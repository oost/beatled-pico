#ifndef COMMAND__CONSTANTS_H
#define COMMAND__CONSTANTS_H

#include <stdbool.h>
#include <stdint.h>

#include "macros.h"

#ifndef PICO_UNIQUE_BOARD_ID_SIZE_BYTES
#define PICO_UNIQUE_BOARD_ID_SIZE_BYTES 8
#endif // PICO_UNIQUE_BOARD_ID_SIZE_BYTES

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  BEATLED_MESSAGE_ERROR = 0,
  BEATLED_MESSAGE_HELLO_REQUEST,
  BEATLED_MESSAGE_HELLO_RESPONSE,
  BEATLED_MESSAGE_TEMPO_REQUEST,
  BEATLED_MESSAGE_TEMPO_RESPONSE,
  BEATLED_MESSAGE_TIME_REQUEST,
  BEATLED_MESSAGE_TIME_RESPONSE,
  BEATLED_MESSAGE_PROGRAM,
  BEATLED_MESSAGE_NEXT_BEAT,
  BEATLED_MESSAGE_LAST_VALUE
} beatled_message_type_t;

typedef enum {
  BEATLED_ERROR_UNKNOWN = 0,
  BEATLED_ERROR_UNKNOWN_MESSAGE_TYPE,
  BEATLED_ERROR_NO_DATA
} beatled_error_codes;

typedef struct {
  uint8_t type; // beatled_message_type_t
} __attribute__((__packed__)) beatled_message_t;

// Tempo message. eCommandType = BEATLED_MESSAGE_ERROR
typedef struct {
  beatled_message_t base;
  uint8_t error_code;
} __attribute__((__packed__)) beatled_message_error_t;

// Tempo message. eCommandType = BEATLED_MESSAGE_TEMPO_REQUEST
typedef struct {
  beatled_message_t base;
  uint64_t beat_time_ref;
  uint32_t tempo_period_us;
} __attribute__((__packed__)) beatled_message_tempo_request_t;

// Tempo message. eCommandType = BEATLED_MESSAGE_TEMPO_RESPONSE
typedef struct {
  beatled_message_t base;
  uint64_t beat_time_ref;
  uint32_t tempo_period_us;
  uint16_t program_id;
} __attribute__((__packed__)) beatled_message_tempo_response_t;

// Hello. eCommandType = BEATLED_MESSAGE_HELLO_REQUEST
typedef struct {
  beatled_message_t base;
  uint8_t board_id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
} __attribute__((__packed__)) beatled_message_hello_request_t;

// Hello. eCommandType = BEATLED_MESSAGE_HELLO_RESPONSE
typedef struct {
  beatled_message_t base;
  uint16_t client_id;
} __attribute__((__packed__)) beatled_message_hello_response_t;

// eCommandType = BEATLED_MESSAGE_TIME_REQUEST
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
} __attribute__((__packed__)) beatled_message_time_request_t;

// eCommandType = BEATLED_MESSAGE_TIME_RESPONSE
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
  uint64_t recv_time;
  uint64_t xmit_time;
} __attribute__((__packed__)) beatled_message_time_response_t;

// eCommandType = BEATLED_MESSAGE_PROGRAM
typedef struct {
  beatled_message_t base;
  uint16_t program_id;
} __attribute__((__packed__)) beatled_message_program_t;

typedef struct {
  beatled_message_t base;
  uint64_t next_beat_time_ref;
  uint32_t tempo_period_us;
  uint32_t beat_count;
  uint16_t program_id;
} __attribute__((__packed__)) beatled_message_next_beat_t;

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif // COMMAND__CONSTANTS_H