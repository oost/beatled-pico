#ifndef SRC__HAL__RUNTIME__PORTS__POSIX__LED_BUFFER__H_
#define SRC__HAL__RUNTIME__PORTS__POSIX__LED_BUFFER__H_

#include <cstdint>
#include <memory>
#include <vector>

struct LEDColor {
  uint8_t blue;
  uint8_t red;
  uint8_t green;
};

class LEDBuffer {
public:
  using Ptr = std::shared_ptr<LEDBuffer>;
  LEDBuffer(LEDColor color, std::size_t num_led) : stream_(num_led) {
    for (int i = 0; i < num_led; i++) {
      stream_[i] = color;
    }
  }

  LEDBuffer(uint32_t *stream, std::size_t num_led) : stream_(num_led) {
    for (int i = 0; i < num_led; i++) {
      stream_[i] = LEDColor{.blue = (uint8_t)(stream[i] >> 8),
                            .red = (uint8_t)(stream[i] >> 16),
                            .green = (uint8_t)(stream[i] >> 24)};
    }
  }

  static Ptr load_instance();
  static void create_instance(uint32_t *stream, uint16_t num_pixel);

  const LEDColor &at(std::size_t idx) { return stream_[idx]; }

private:
  std::vector<LEDColor> stream_;
};

#endif // SRC__HAL__RUNTIME__PORTS__POSIX__LED_BUFFER__H_