#include "core0.h"
#include <pico/stdlib.h>
#include <stdio.h>

#include "beatled/protocol.h"
#include "blink/blink.h"
#include "clock/clock.h"
#include "command/command.h"
#include "command_queue/queue.h"
#include "udp_server/udp_server.h"
#include "ws2812/ws2812.h"

#include "autotest/autotest.h"

void core0_init() {
  puts("Initializing core 0");

  // while (!clock_is_synced()) {
  //   sleep_ms(20);
  // }
  puts("Clock is synced via SNTP");

  init_test();
}

void core0_loop() {
  sleep_ms(2000);
  printf("Starting core 0 loop\n");
  blink(MESSAGE_BLINK_SPEED, MESSAGE_WELCOME);
  sleep_ms(1000);

  command_envelope_t envelope;

  test_tempo();

  while (1) {
    sleep_ms(20);
    while (1) {
      if (!command_queue_pop_message(&envelope)) {
        break;
      };

      puts("Got a message, going to parse it...");

      if (parse_command(&envelope)) {
        puts("Error parsing command :-(");
      }
    }
    led_update();
  }
}
