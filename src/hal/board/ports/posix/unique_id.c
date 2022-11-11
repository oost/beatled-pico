#include <stdlib.h>

#include "hal/unique_id.h"

void get_unique_board_id(board_id_t *board_id) { board_id->id[0] = 12u; }

void hal_stdio_init() {}