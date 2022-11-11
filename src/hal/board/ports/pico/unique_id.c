#include <pico/unique_id.h>
#include <stdlib.h>

#include "hal/unique_id.h"

void get_unique_board_id(board_id_t *board_id) {
  pico_get_unique_board_id((pico_unique_board_id_t *)board_id);
}
