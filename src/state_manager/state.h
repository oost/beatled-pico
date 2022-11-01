#ifndef STATE_MANAGER__STATE_H
#define STATE_MANAGER__STATE_H

typedef struct state_update {
  uint64_t update_timestamp;
  uint64_t time_offset;
  uint64_t tempo_time_ref;
  uint32_t tempo_period_us;
  uint8_t program_idx;
} state_update_t;

#endif // STATE_MANAGER__STATE_H