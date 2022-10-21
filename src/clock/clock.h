#ifndef CLOCK_CLOCK_H
#define CLOCK_CLOCK_H


#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include <pico/time.h>

void sntp_sync_init(void);

absolute_time_t get_absolute_time_from_server_time(uint64_t server_time);

void sntp_set_system_time(uint32_t sec, uint32_t nsec);



#ifdef __cplusplus
}
#endif

#endif // CLOCK_CLOCK_H