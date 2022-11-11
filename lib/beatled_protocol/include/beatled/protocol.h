#ifndef COMMAND__CONSTANTS_H
#define COMMAND__CONSTANTS_H

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
  BEATLED_MESSAGE_HELLO,
  BEATLED_MESSAGE_TEMPO,
  BEATLED_MESSAGE_TIME,
  BEATLED_MESSAGE_PROGRAM
} beatled_message_type_t;

typedef enum {
  BEATLED_ERROR_UNKNOWN = 0,
  BEATLED_ERROR_UNKNOWN_MESSAGE_TYPE,
  BEATLED_ERROR_NO_DATA
} beatled_error_codes;

typedef struct {
  uint8_t type; // beatled_message_type_t
} PACK_STRUCT_STRUCT beatled_message_t;

// Tempo message. eCommandType = BEATLED_MESSAGE_ERROR
typedef struct {
  beatled_message_t base;
  uint8_t error_code;
} PACK_STRUCT_STRUCT beatled_error_msg_t;

// Tempo message. eCommandType = BEATLED_MESSAGE_TEMPO
typedef struct {
  beatled_message_t base;
  uint64_t beat_time_ref;
  uint32_t tempo_period_us;
} PACK_STRUCT_STRUCT beatled_tempo_msg_t;

// Hello. eCommandType = BEATLED_MESSAGE_HELLO
typedef struct {
  beatled_message_t base;
  char board_id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
} PACK_STRUCT_STRUCT beatled_hello_msg_t;

typedef struct {
  beatled_message_t base;
  uint16_t pico_id;
} PACK_STRUCT_STRUCT beatled_hello_response_t;

// eCommandType = BEATLED_MESSAGE_TIME
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
} PACK_STRUCT_STRUCT beatled_time_req_msg_t;

// eCommandType = BEATLED_MESSAGE_TIME
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
  uint64_t recv_time;
  uint64_t xmit_time;
} PACK_STRUCT_STRUCT beatled_time_resp_msg_t;

// eCommandType = BEATLED_MESSAGE_PROGRAM
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
  uint32_t program_id;
} PACK_STRUCT_STRUCT beatled_program_msg_t;

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif // COMMAND__CONSTANTS_H