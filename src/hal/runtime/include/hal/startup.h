#ifndef SRC__RUNTIME__INCLUDE__STARTUP__H_
#define SRC__RUNTIME__INCLUDE__STARTUP__H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*startup_main_t)();

void startup(startup_main_t startup_main);

#ifdef POSIX_PORT
void push_color_stream(uint32_t *stream, uint16_t num_pixel);
#endif

#ifdef __cplusplus
}
#endif

#endif // SRC__RUNTIME__INCLUDE__STARTUP__H_