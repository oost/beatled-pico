target_sources(${CURRENT_LIBRARY_NAME} PUBLIC
  registry.c
)

target_link_libraries(${CURRENT_LIBRARY_NAME}
  pico_sync
)
