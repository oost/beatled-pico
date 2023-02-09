// #include <pico/sync.h>
#include "hal/unique_id.h"
#include <stdio.h>

#include "state_manager.h"
#include "states/states.h"

typedef struct state_manager_internal_state {
  state_manager_state_t current_state;
  int64_t last_tempo_sync_time;
} state_manager_internal_state_t;

state_manager_internal_state_t internal_state = {.current_state = 0,
                                                 .last_tempo_sync_time = 0};
exit_state_fn exit_current_state;

// Each element in the matrix is a mask determining the states that can be
// transitioned to from a given state.
uint16_t transition_matrix[] = {
    0x01 << STATE_STARTED,      // STATE_UNKNOWN
    0x01 << STATE_INITIALIZED,  // STATE_STARTED
    0x01 << STATE_REGISTERED,   // STATE_INITIALIZED
    0x01 << STATE_TIME_SYNCED,  // STATE_REGISTERED
    0x01 << STATE_TEMPO_SYNCED, // STATE_TIME_SYNCED
    0x01 << STATE_TIME_SYNCED   // STATE_TIME_SYNCED
};

void state_manager_init() {}

state_manager_state_t state_manager_get_state() {
  return internal_state.current_state;
}

int transition_state(state_manager_state_t new_state) {
  state_manager_state_t old_state = internal_state.current_state;

  if (old_state == new_state) {
    puts("Transitioning to the same state... Noop");
    return 1;
  }

  int err = 0;

  if (((transition_matrix[internal_state.current_state] &
        (0x01 << new_state)) == 0)) {
    printf("Transition not allowed from %d to %d\n", old_state, new_state);
    return 2;
  }

  printf("Transitioning from %d to %d\n", old_state, new_state);

  if (exit_current_state) {
    err = exit_current_state();
  }

  internal_state.current_state = new_state;

  switch (new_state) {
  case STATE_STARTED:
    enter_started_state();
    exit_current_state = &exit_started_state;
    break;
  case STATE_INITIALIZED:
    enter_initialized_state();
    exit_current_state = &exit_initialized_state;
    break;

  case STATE_REGISTERED:
    enter_registered_state();
    exit_current_state = &exit_registered_state;
    break;

  case STATE_TIME_SYNCED:
    enter_time_synced_state();
    exit_current_state = &exit_time_synced_state;
    break;

  default:
    puts("Unknown state...");
  }

  return err;
}

int state_manager_set_state(state_manager_state_t state) {
  return transition_state(state);
}
