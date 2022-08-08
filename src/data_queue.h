#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#include <stdio.h>
#include "pico/stdlib.h"

typedef struct
{
    void *func;
    int32_t data;
} queue_entry_t;

extern queue_t call_queue;
extern queue_t results_queue;

#endif // !DATA_QUEUE_H


