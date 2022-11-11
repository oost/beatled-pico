#include <pico/unique_id.h>
#include <stdlib.h>

#include "hal/unique_id.h"

struct board_id {
  pico_unique_board_id_t impl;
};

board_id_handle_t get_unique_board_id() {
  board_id_handle_t ptr = (board_id_handle_t)malloc(sizeof(board_id_t));
  pico_get_unique_board_id(&(ptr->impl));
  return ptr;
}