#ifndef STATE_MANAGER__STATE_MANAGER_H
#define STATE_MANAGER__STATE_MANAGER_H

#include <pico/time.h>

void state_manager_init();
void state_manager_get_tempo(absolute_time_t *time_ref, uint32_t *tempo);
void state_manager_set_tempo(absolute_time_t time_ref, uint32_t tempo);

const char *state_manager_get_unique_board_id();

#endif // STATE_MANAGER__STATE_MANAGER_H