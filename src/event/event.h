#ifndef EVENT__EVENT_H
#define EVENT__EVENT_H

#include <stdint.h>

typedef enum { event_error = 0, event_server_message } event_type_t;

typedef struct {
  event_type_t event_type;
  uint64_t time;
  size_t data_length;
  void *data;
} event_t;

#endif // EVENT__EVENT_H