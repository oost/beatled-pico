add_library(event_queue INTERFACE)

target_sources(
    event_queue INTERFACE 
    queue.c
)

target_link_libraries(event_queue INTERFACE
    pico_util
    beatled_hal_queue
)


get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()