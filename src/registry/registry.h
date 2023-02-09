#ifndef REGISTRY__REGISTRY_H
#define REGISTRY__REGISTRY_H

typedef struct registry {
  uint64_t time_offset;
  uint64_t time_offset_update_timestamp;
  uint64_t tempo_time_ref;
  uint32_t tempo_period_us;
  uint64_t tempo_time_update_timestamp;
  uint16_t program_id;
} registry_t;

typedef enum {
  REGISTRY_UPDATE_TIME_OFFSET = 0,
  REGISTRY_UPDATE_TEMPO,
  REGISTRY_UPDATE_PROGRAM
} registry_update_fields_t;

typedef struct registry_update {
  uint64_t update_timestamp;
  uint64_t time_offset;
  uint64_t tempo_time_ref;
  uint32_t tempo_period_us;
  uint16_t program_id;
  uint8_t registry_update_fields;
} registry_update_t;

extern registry_t registry;

void registry_init();

void registry_set_time_offset(uint64_t time_offset, uint64_t update_timestamp);

void registry_set_tempo(uint64_t tempo_time_ref, uint32_t tempo_period_us,
                        uint16_t program_id, uint64_t update_timestamp);

void registry_set_program(uint8_t program_id);

#endif // REGISTRY__REGISTRY_H