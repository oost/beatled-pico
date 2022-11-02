add_library(beatled_hal_udp 
    udp.c
)

target_link_libraries(beatled_hal_udp
  pico_stdlib
  pico_cyw43_arch_lwip_threadsafe_background
)

target_include_directories(beatled_hal_udp PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/..
)
