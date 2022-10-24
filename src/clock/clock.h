#ifndef CLOCK__CLOCK_H
#define CLOCK__CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <pico/time.h>
#include <stdint.h>

void sntp_sync_init(void);

bool clock_is_synced();
uint64_t get_sntp_server_time_ref_us();

absolute_time_t server_time_to_local_time(uint64_t server_time);

void sntp_set_system_time(uint32_t sec, uint32_t nsec);

#ifdef __cplusplus
}
#endif

#endif // CLOCK__CLOCK_H