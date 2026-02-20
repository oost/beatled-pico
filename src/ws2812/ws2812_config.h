#ifndef WS2812_CONFIG_H
#define WS2812_CONFIG_H

#define IS_RGBW false

#ifndef NUM_PIXELS
#error "NUM_PIXELS is not defined. Pass it at configure time: cmake -DNUM_PIXELS=30 .."
#endif

#ifdef PICO_DEFAULT_WS2812_PIN
#define WS2812_PIN PICO_DEFAULT_WS2812_PIN
#else
// default to pin 2 if the board doesn't have a default WS2812 pin defined
// #define WS2812_PIN 2
#define WS2812_PIN 0

#endif

#endif // WS2812_CONFIG_H