#ifndef COMMAND__CONSTANTS_H
#define COMMAND__CONSTANTS_H

#include <stdint.h>

#define COMMAND_MAX_LEN 128

// Code for header body
#define COMMAND_HELLO 'a'
#define COMMAND_RANDOM 'b'
#define COMMAND_PROGRAM 'c'
#define COMMAND_BEAT 'd'
#define COMMAND_TEMPO 't'

// Tempo message
struct tempo_msg_struct {
  uint8_t command;
  uint64_t beat_time_ref;
  uint32_t tempo_period_us;
} __attribute__((packed));

typedef struct tempo_msg_struct tempo_msg_t;

// #define TEMPO_MSG_LEN sizeof(tempo_msg)
#define TEMPO_MSG_LEN 13

#endif // COMMAND__CONSTANTS_H