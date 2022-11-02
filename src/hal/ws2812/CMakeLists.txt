add_library(beatled_hal_ws2812
    ws2812.c
    ws2812_dma.c
)

pico_generate_pio_header(
  beatled_hal_ws2812 
  ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio 
  OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated
)


target_link_libraries(beatled_hal_ws2812
  pico_stdlib 
  hardware_pio 
  hardware_dma
)
