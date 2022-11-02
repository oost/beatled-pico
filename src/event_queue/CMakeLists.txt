add_library(event_queue INTERFACE)

target_sources(
    event_queue INTERFACE 
    queue.c
)

target_link_libraries(event_queue INTERFACE
    pico_util
)
