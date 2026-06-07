# Beatled Pico — moved to [`oost/beatled/controller/`](https://github.com/oost/beatled/tree/master/controller)

> **This repository is a read-only mirror.**
>
> Active firmware development has moved into the main
> [`oost/beatled`](https://github.com/oost/beatled) monorepo, under the
> [`controller/`](https://github.com/oost/beatled/tree/master/controller)
> path. The wire protocol, server, React client, iOS/macOS app, build
> scripts, and CI all live in one place now — keeping the firmware
> in-tree eliminated the protocol-header drift that the prior split caused.
>
> The history here is preserved up to commit
> [`2d36f04`](https://github.com/oost/beatled-pico/commit/2d36f04)
> (protocol v2). Everything after that — including the actual git-subtree
> import that moved the tree — lives in `oost/beatled`. Please open
> issues and pull requests against that repo.

---

## Original README (preserved)

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

Copy the env template and fill in your values:

```bash
cp .env.pico.template .env.pico   # Pico W / POSIX
cp .env.esp32.template .env.esp32 # ESP32
```

### macOS (posix port)

```bash
source .env.pico
cmake -DPORT=posix \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DNUM_PIXELS=$NUM_PIXELS -DWS2812_PIN=$WS2812_PIN \
  -B build
cmake --build build
./build/src/pico_w_beatled.app/Contents/MacOS/pico_w_beatled
```

### Pico W

```bash
source .env.pico
cmake -DPORT=pico -DPICO_BOARD=pico_w \
  -DNUM_PIXELS=$NUM_PIXELS -DWS2812_PIN=$WS2812_PIN \
  -B build-pico
cmake --build build-pico
cp build-pico/src/pico_w_beatled.uf2 /Volumes/RPI-RP2/
```

### ESP32

```bash
# uses scripts/beatled.sh from the beatled repo (reads .env.esp32)
scripts/beatled.sh flash-esp32

# or manually:
source .env.esp32
cd esp32
WIFI_SSID="$WIFI_SSID" WIFI_PASSWORD="$WIFI_PASSWORD" \
  BEATLED_SERVER_NAME="$BEATLED_SERVER_NAME" \
  NUM_PIXELS="$NUM_PIXELS" WS2812_PIN="$WS2812_PIN" \
  idf.py set-target $ESP32_TARGET && idf.py build flash monitor -p $ESP32_PORT
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
