target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    blink.c
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
  pico_stdlib
  ${PICO_WIFI_LIBRARY}
)
