
add_library(beatled_process)

# generate the header file into the source tree as it is included in the RP2040 datasheet

target_sources(beatled_process PUBLIC 
  core0.c
  core1.c
)

target_link_libraries(beatled_process PUBLIC
  beatled_hal_process
  beatled_hal_queue
  beatled_commands
  beatled_autotest
  beatled_state_manager
  beatled_ws2812

)

target_include_directories(beatled_process INTERFACE ${CMAKE_CURRENT_LIST_DIR})

