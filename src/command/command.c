#include "command/command.h"
#include "command/constants.h"
#include "blink/blink.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "ws2812/ws2812.h"

int command_hello() 
{
  puts("Hello!");
  blink(MESSAGE_BLINK_SPEED, MESSAGE_HELLO);
  return 0;
}

int command_random() 
{
  led_set_random_pattern();
  return 0;
}
int command_program(char * command, uint16_t message_length)
{
  led_update_pattern_idx(command[1]);
  return 0;
}

int command_beat(char * command, uint16_t message_length)
{
  puts("Beat!");
  uint64_t beat_time = 0;
  int i;
  uint64_t paquet = 0;
  for( i = 7; i >= 0; --i )
  {
      paquet <<= 8;
      paquet |= (uint64_t)command[1+i];
  }

  led_beat();
  return 0;
}

int parse_command(char * command, uint16_t message_length)
{
  puts("Parsing command");
  // fwrite(command, 1, message_length, stdout);


  switch (command[0])
  {
  case COMMAND_HELLO:
    return command_hello();
  case COMMAND_RANDOM:
    return command_random();  
  case COMMAND_PROGRAM:
    return command_program(command, message_length);
  case COMMAND_BEAT:
    return command_beat(command, message_length);
  default:
    puts("Unknown command...");
    blink(ERROR_BLINK_SPEED, ERROR_COMMAND);
    return 1;
  }
}
