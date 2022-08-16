#ifndef MANAGER_H
#define MANAGER_H

#include "pico/stdlib.h"


struct envelope {
  void* next;
  absolute_time_t time_received;	
  uint16_t message_length;
  char* message;
  uint16_t port;
};

void command_queue_init();

int command_queue_add_message(char *message, uint16_t message_length, uint16_t port);

int command_queue_pop_message(char *message, uint16_t* message_length);

#endif // MANAGER_H