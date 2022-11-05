#include "hal/unique_id.h"
#include <pico/unique_id.h>

typedef pico_unique_board_id_t board_id_t;

board_id_handle_t get_unique_board_id(board_id_handle_t board_id) {
  board_id_handle_t ptr = board_id_handle_t malloc(sizeof(board_id_t));
  pico_get_unique_board_id(ptr);
  return ptr;
}