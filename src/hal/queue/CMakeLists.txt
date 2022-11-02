add_library(beatled_hal_queue
    queue.c
)

target_link_libraries(beatled_hal_queue
  pico_stdlib
  pico_util
)
