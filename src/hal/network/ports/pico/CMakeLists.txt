target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    dns.c
    udp_sender.c
    udp_server.c
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
  pico_stdlib
  pico_cyw43_arch_lwip_threadsafe_background
)
