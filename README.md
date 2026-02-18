# Beatled Pico

Embedded C firmware for beat-synchronized LED control. Connects to a [Beatled server](https://github.com/oost/beatled) over WiFi and drives WS2812 LED strips in time with the music.

Runs on **Raspberry Pi Pico W**, **ESP32** (S3, C3), and as a native **macOS** simulator for development.

## Supported Platforms

| Port | Target | LED Driver | Build |
|------|--------|------------|-------|
| `pico` | Pico W (RP2040) | PIO + DMA | CMake + Pico SDK |
| `pico_freertos` | Pico W (RP2040) | PIO + DMA | CMake + Pico SDK |
| `posix` | macOS / Linux | Metal simulation | CMake |
| `posix_freertos` | macOS / Linux | Metal simulation | CMake |
| `esp32` | ESP32-S3, C3 | RMT peripheral | ESP-IDF |

A 10-module [Hardware Abstraction Layer](https://oost.github.io/beatled/pico.html) keeps application code (state machine, commands, clock, patterns) platform-independent.

## Quick Start

```bash
git clone https://github.com/oost/beatled-pico.git
cd beatled-pico
git submodule update --init
```

### macOS (posix port)

```bash
cmake -DPORT=posix \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -B build
cmake --build build
./build/src/pico_w_beatled.app/Contents/MacOS/pico_w_beatled
```

### Pico W

```bash
cmake -DPORT=pico -DPICO_BOARD=pico_w -B build-pico
cmake --build build-pico
cp build-pico/src/pico_w_beatled.uf2 /Volumes/RPI-RP2/
```

### ESP32

```bash
cd esp32
WIFI_SSID=MyNet WIFI_PASSWORD=MyPass BEATLED_SERVER_NAME=192.168.1.100 \
  idf.py set-target esp32s3 && idf.py build
idf.py flash monitor -p /dev/ttyUSB0
```

## Tests

```bash
cmake --build build
./build/tests/posix/integration/test_integration
./build/tests/posix/command/test_command
./build/tests/posix/clock/test_clock
./build/tests/posix/queue/test_queue
```

## Documentation

Full docs including build instructions, HAL reference, and architecture diagrams: [oost.github.io/beatled/pico.html](https://oost.github.io/beatled/pico.html)
