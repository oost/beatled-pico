#ifndef SRC__RUNTIME__INCLUDE__STARTUP__H_
#define SRC__RUNTIME__INCLUDE__STARTUP__H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*startup_main_t)();

void startup(startup_main_t startup_main);

#ifdef __cplusplus
}
#endif

#endif // SRC__RUNTIME__INCLUDE__STARTUP__H_