#include <pico/stdlib.h>
#include <pico/util/queue.h>
#include <stdio.h>

// #include "blink/blink.h"
// #include "clock/clock.h"

// #include "event_queue/queue.h"
#include "intercore_queue.h"
// #include "state_manager/state.h"
// #include "state_manager/state_manager.h"
// #include "udp_server/udp_server.h"
// #include "wifi/wifi.h"
// #include "ws2812/ws2812.h"

// #include "command/hello/hello.h"
// #include "command/tempo/tempo.h"
// #include "command/time/time.h"

void init() {}

void deinit() {
  puts("Deinit... Not sure how we got here");
  // wifi_deinit();
  queue_free(&intercore_command_queue);
}

int main(void) {

  // Shouldn't ever get here but who knows...
  deinit();
  return 0;
}
