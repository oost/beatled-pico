#ifndef COMMAND__CONSTANTS_H
#define COMMAND__CONSTANTS_H

#include <stdint.h>

#ifndef PICO_UNIQUE_BOARD_ID_SIZE_BYTES
#define PICO_UNIQUE_BOARD_ID_SIZE_BYTES 8
#endif // PICO_UNIQUE_BOARD_ID_SIZE_BYTES

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  eBeatledError = 0,
  eBeatledHello,
  eBeatledTempo,
  eBeatledTime,
  eBeatledProgram
} beatled_message_type_t;

typedef enum {
  unknownError = 0,
  unknownMessageType,
  noData
} beatled_error_codes;

typedef struct {
  uint8_t type; // beatled_message_type_t
} __attribute__((packed)) beatled_message_t;

// Tempo message. eCommandType = eBeatledError
typedef struct {
  beatled_message_t base;
  uint8_t error_code;
} __attribute__((packed)) beatled_error_msg_t;

// Tempo message. eCommandType = eBeatledTempo
typedef struct {
  beatled_message_t base;
  uint64_t beat_time_ref;
  uint32_t tempo_period_us;
} __attribute__((packed)) beatled_tempo_msg_t;

// Hello. eCommandType = eBeatledHello
typedef struct {
  beatled_message_t base;
  char board_id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];
} __attribute__((packed)) beatled_hello_msg_t;

typedef struct {
  beatled_message_t base;
  uint16_t pico_id;
} __attribute__((packed)) beatled_hello_response_t;

// eCommandType = eBeatledTime
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
} __attribute__((packed)) beatled_time_req_msg_t;

// eCommandType = eBeatledTime
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
  uint64_t recv_time;
  uint64_t xmit_time;
} __attribute__((packed)) beatled_time_resp_msg_t;

// eCommandType = eBeatledProgram
typedef struct {
  beatled_message_t base;
  uint64_t orig_time;
  uint32_t program_id;
} __attribute__((packed)) beatled_program_msg_t;

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif // COMMAND__CONSTANTS_H