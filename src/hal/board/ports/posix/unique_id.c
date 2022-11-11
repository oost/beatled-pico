#include <stdlib.h>

#include "hal/unique_id.h"

struct board_id {
  unsigned int id;
};

board_id_handle_t get_unique_board_id() {
  board_id_handle_t ptr = (board_id_handle_t)malloc(sizeof(board_id_t));
  ptr->id = 1234;
  return ptr;
}

void hal_stdio_init() {}