add_library(beatled_hal_clock clock.c)

target_link_libraries(beatled_hal_clock
  pico_stdlib
  pico_time
)

