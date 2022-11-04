target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
    queue.c
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
  pico_stdlib
  pico_util
)
