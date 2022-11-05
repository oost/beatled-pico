#include <stdio.h>
#include <stdlib.h>

#include "intercore_queue.h"

void init() {}

void deinit() {
  puts("Deinit... Not sure how we got here");
  // wifi_deinit();
  hal_queue_free(intercore_command_queue);
}

int main(void) {

  // Shouldn't ever get here but who knows...
  deinit();
  return 0;
}
