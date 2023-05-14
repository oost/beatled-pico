target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    dns.c
    udp_sender.c
    udp_server.c
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
  pico_stdlib
  pico_cyw43_arch_lwip_threadsafe_background
)
# For lwipopts.h
target_include_directories(${CURRENT_LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../../..)
