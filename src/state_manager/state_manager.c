#include "state_manager.h"
#include <pico/sync.h>
#include <pico/unique_id.h>

static char board_id[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

volatile static uint32_t _tempo_period_us;
volatile static absolute_time_t _time_ref;

static critical_section_t crit_sec;

void state_manager_init() {
  critical_section_init(&crit_sec);

  pico_get_unique_board_id_string(board_id, count_of(board_id));
}

void state_manager_get_tempo(absolute_time_t *time_ref,
                             uint32_t *tempo_period_us) {
  // critical_section_enter_blocking(&crit_sec);
  *time_ref = _time_ref;
  *tempo_period_us = _tempo_period_us;
  // critical_section_exit(&crit_sec);
}

void state_manager_set_tempo(absolute_time_t time_ref,
                             uint32_t tempo_period_us) {
  // critical_section_enter_blocking(&crit_sec);
  _time_ref = time_ref;
  _tempo_period_us = tempo_period_us;
  // critical_section_exit(&crit_sec);
}

const char *state_manager_get_unique_board_id() { return board_id; }