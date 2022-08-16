#ifndef CLOCK_CLOCK_H
#define CLOCK_CLOCK_H


#ifdef __cplusplus
extern "C" {
#endif


#define SNTP_SET_SYSTEM_TIME	sntp_set_system_time
#define SNTP_HOST_NAME "raspberrypiz.local"

void sntp_sync_init(void);

void sntp_set_system_time(u32_t sec);



#ifdef __cplusplus
}
#endif

#endif // CLOCK_CLOCK_H