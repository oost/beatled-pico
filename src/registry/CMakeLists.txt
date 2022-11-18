add_library(beatled_registry INTERFACE)

target_sources(
  beatled_registry INTERFACE 
      registry.c
)

target_include_directories(beatled_registry INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(beatled_registry INTERFACE
    # beatled_hal_wifi
    # beatled_hal_board
    
    # beatled_commands
    # beatled_ws2812
)

